
#include"ClosureTest.h"

#include "../Common/AccEffAux.h"
#include"../../Utils/Helpers/Helpers.h"
#include "../../Drawing/Drawing.h"

#include<iostream>

#include"TEfficiency.h"
#include"TH1.h"

constexpr std::array<double,5> ZBins { 0.0f,0.25f,0.50f,0.75f,1.0f};
void processZ(TTree* tree, WeightFunc2D* acc, WeightFunc2D* eff, TH1D* output);
void processZ2(TTree* tree, WeightFunc2D* acc, WeightFunc2D* eff, TH1D* output);
TH1D* executeZ(TFile* inputFile ,TFile* accFile, TFile* effFile, const char* th2Name, const char* outputname, const char* th1Name);
TH1D* executeZ2(TFile* inputFile ,TFile* accFile, TFile* effFile, const char* th2Name, const char* outputname, const char* th1Name);

void ClosureTestJet(const char* inputFilename, const char* accFilename, const char* effFilename, const char* accWFilename, const char* effWFilename, const char* outputname)
{
    std::cout << "\nCLOSURE TEST\n";

    //input file
    std::cout << "Reading input file: " << inputFilename <<'\n';
    TFile* inputFile = OpenFile(inputFilename,"READ");

    //input file
    std::cout << "Reading Acc file: " << accFilename <<'\n';
    TFile* accFile = OpenFile(accFilename,"READ");

    //input file
    std::cout << "Reading Eff file: " << effFilename <<'\n';
    TFile* effFile = OpenFile(effFilename,"READ");

    //input file
    std::cout << "Reading weighted Acc file: " << accWFilename <<'\n';
    TFile* accWFile = OpenFile(accWFilename,"READ");

    //input file
    std::cout << "Reading weighted Eff file: " << effWFilename <<'\n';
    TFile* effWFile = OpenFile(effWFilename,"READ");

    //output file
    std::cout << "Writing to file: " << outputname <<'\n';
    TFile* outFile = OpenFile(outputname,"RECREATE");

    for(int i=3;i<7;i++)
    {
        const std::string numStr=std::to_string(i);
        const std::string thName="hGenUpsilonPtvsRapSlide"+numStr;
        TH1D* outputUnweighted=executeZ(inputFile,accFile,effFile,thName.data(),outputname,("yieldZ_Unweighted"+numStr).data());
        TH1D* outputWeighted=executeZ(inputFile,accWFile,effWFile,(thName+"Weight").data(),outputname,("yieldZ_Weighted"+numStr).data());
        TH1D closure= (*outputWeighted)/(*outputUnweighted);

        const std::string nameStr="yieldsRatioSlide"+numStr;
        closure.SetName(nameStr.data());
        closure.SetTitle("accxeff corrected yields weighted/unweighted ratio vs z");
        closure.GetXaxis()->SetTitle("z");
        closure.GetYaxis()->SetTitle("ratio");

        writeToCanvas(&closure,"z","ratio",ReplaceExtension(outputname,""));

        outputUnweighted->Write();
        outputWeighted->Write();
        closure.Write();
    }

    outFile->Close();
    accFile->Close();
    inputFile->Close();
    
}

TH1D* executeZ(TFile* inputFile ,TFile* accFile, TFile* effFile, const char* th2Name, const char* outputname, const char* th1Name)
{
    TH2* acc = dynamic_cast<TH2*> (accFile->Get(th2Name));

    if (acc==nullptr)
    {
        throw std::invalid_argument("acceptancy TH2 not found");
    }

    TH2* eff = dynamic_cast<TH2*> (effFile->Get(th2Name));

    if (eff== nullptr)
    {
        throw std::invalid_argument("efficiency TH2 not found");
    }

    std::string outputbasename=ReplaceExtension(outputname,"");

    WeightFuncTH2 accWeight(acc);
    WeightFuncTH2 effWeight(eff);

    TTree* tree= GetTree(inputFile,"onia_skimmed");

    TH1D* output = new TH1D(th1Name," yield vs z;z",ZBins.size()-1,ZBins.data());

    processZ(tree,&accWeight,&effWeight,output);

    return output;
}

TH1D* executeZ2(TFile* inputFile ,TFile* accFile, TFile* effFile, const char* th2Name, const char* outputname, const char* th1Name)
{
    TH2* acc = dynamic_cast<TH2*> (accFile->Get(th2Name));

    if (acc==nullptr)
    {
        throw std::invalid_argument("acceptancy TH2 not found");
    }

    TH2* eff = dynamic_cast<TH2*> (effFile->Get(th2Name));

    if (eff== nullptr)
    {
        throw std::invalid_argument("efficiency TH2 not found");
    }

    std::string outputbasename=ReplaceExtension(outputname,"");

    WeightFuncTH2 accWeight(acc);
    WeightFuncTH2 effWeight(eff);

    TTree* tree= GetTree(inputFile,"onia_skimmed");

    TH1D* output = new TH1D(th1Name," yield vs z^2;z^2",ZBins.size()-1,ZBins.data());

    processZ2(tree,&accWeight,&effWeight,output);

    return output;
}


void processZ2(TTree* tree, WeightFunc2D* acc, WeightFunc2D* eff, TH1D* output)
{
    OniaReader<OniaJetQQMC> oniaReader(tree);
    const int size= oniaReader.getEntries();

    for(int i=0; i< size;i++)
    {
        auto data = oniaReader.getData(i);
        const double pt=data->recoQQOut.pT;
        const double y=data->recoQQOut.y;
        const double z=data->jetOut.z;
        const double weight= acc->getWeight(pt,y) * eff->getWeight(pt,y);

        output->Fill(z*z,weight);

    }
}

void processZ(TTree* tree, WeightFunc2D* acc, WeightFunc2D* eff, TH1D* output)
{
    OniaReader<OniaJetQQMC> oniaReader(tree);
    const int size= oniaReader.getEntries();

    for(int i=0; i< size;i++)
    {
        auto data = oniaReader.getData(i);
        const double pt=data->recoQQOut.pT;
        const double y=data->recoQQOut.y;
        const double z=data->jetOut.z;
        const double weight= acc->getWeight(pt,y) * eff->getWeight(pt,y);

        output->Fill(z,weight);

    }
}