
#include"AddWeights.h"

#include "../Common/AccEffAux.h"
#include"../../Utils/Helpers/Helpers.h"
#include "../../Drawing/Drawing.h"

#include<iostream>

#include"TEfficiency.h"
#include"TH1.h"


TTree* process(TTree* tree, WeightFunc2D* acc, WeightFunc2D* eff);
TTree* execute(TFile* inputFile ,TFile* accFile, TFile* effFile, const char* th2AccName, const char* th2EffName);

void AddWeights(const char* inputFilename, const char* accFilename, const char* effFilename, const char* outputname)
{
    std::cout << "\nWEIGHT ADD FOR ACCXEFF\n";

    //input file
    std::cout << "Reading input file: " << inputFilename <<'\n';
    TFile* inputFile = OpenFile(inputFilename,"READ");

    //input file
    std::cout << "Reading Acc file: " << accFilename <<'\n';
    TFile* accFile = OpenFile(accFilename,"READ");

    //input file
    std::cout << "Reading Eff file: " << effFilename <<'\n';
    TFile* effFile = OpenFile(effFilename,"READ");

    const std::string prefix="hGenUpsilonPtvsRap";
    const std::vector<std::string> effs
    {
        "Nominal",
        "HSPlus1SystErr",
        "HSMinus1SystErr",
        "HSPlus1StatErr",
        "HSMinus1StatErr",
        "GMPlus1SystErr",
        "GMMinus1SystErr",
        "GMPlus1StatErr",
        "GMMinus1StatErr"
    };
    for(const auto& str : effs)
    {
        //output file
        const std::string thEffName=prefix+str;
        const std::string thAccName="hGenUpsilonPtvsRapSlide5";
        const std::string of= std::string( ReplaceExtension(outputname,"_"))+str+".root";
        std::cout << "Writing to file: " << of <<'\n';
        TFile* outFile = OpenFile(of.data(),"CREATE");
        TTree* output=execute(inputFile,accFile,effFile,thAccName.data(),thEffName.data());

        output->Write(0,TObject::kOverwrite );
        outFile->Close();
    }

    accFile->Close();
    inputFile->Close();
    
}

TTree* execute(TFile* inputFile ,TFile* accFile, TFile* effFile, const char* th2AccName, const char* th2EffName)
{
    TH2* acc = dynamic_cast<TH2*> (accFile->Get(th2AccName));

    if (acc==nullptr)
    {
        throw std::invalid_argument("acceptancy TH2 not found");
    }

    TH2* eff = dynamic_cast<TH2*> (effFile->Get(th2EffName));

    if (eff== nullptr)
    {
        throw std::invalid_argument("efficiency TH2 not found");
    }

    WeightFuncTH2 accWeight(acc);
    WeightFuncTH2 effWeight(eff);

    TTree* tree= GetTree(inputFile,"onia_skimmed");

    return process(tree,&accWeight,&effWeight);
}



TTree* process(TTree* tree, WeightFunc2D* acc, WeightFunc2D* eff)
{
    OniaReader<OniaJetQQRealData> oniaReader(tree);
    OniaWriter<OniaJetQQRealDataW> oniaWriter(tree->GetName());

    const int size= oniaReader.getEntries();

    for(int i=0; i< size;i++)
    {
        auto data = oniaReader.getData(i);
        const double pt=data->recoQQOut.pT;
        const double y=data->recoQQOut.y;
        const double weight= acc->getWeight(pt,y) * eff->getWeight(pt,y);

        auto out= oniaWriter.getDataBuffer();
        out->jetOut= data->jetOut;
        out->oniaInfoOut= data->oniaInfoOut;
        out->oniaMuOut= data->oniaMuOut;
        out->recoQQOut= data->recoQQOut;
        out->weight.accxeff= 1.0f/ weight;

        oniaWriter.writeData();
    }
    return oniaWriter.getTree();
}