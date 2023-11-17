
#include"GenerateToys.h"

#include "../Common/AccEffAux.h"
#include"../../Utils/Helpers/Helpers.h"

#include"TFile.h"
#include"TH2.h"
#include"TRandom3.h"

TH2* generate_Toys(TFile* file,const char* th2Name, const char* th2DenName, TRandom3* rand);

void generateToys(const char* accFilename, const char* effFilename, const char* outputpath, const char* numoftoys)
{
    const int toyn= atoi(numoftoys);
    const UInt_t defaultseed= 4357U;

    if(toyn <=0)
    {
        std::cout << "Error: " << numoftoys << "is a invalid number of toys";
        return;
    }

    //input file
    std::cout << "Reading input file acc: " << accFilename <<'\n';
    TFile* accFile = OpenFile(accFilename,"READ");

    std::cout << "Reading input file eff: " << effFilename <<'\n';
    TFile* effFile = OpenFile(effFilename,"READ");

    TRandom3* rnd = new TRandom3(defaultseed);

    for(int i=0;i<toyn;i++)
    {
        const std::string effname="hGenUpsilonPtvsRapNominal";
        const std::string effdenname="hGenUpsilonEffDen";
        const std::string filename = std::string(outputpath)+"/OutEff2D_"+std::to_string(i)+".root";

        TFile* outfile =OpenFile(filename.data(),"CREATE");

        TH2* neweff =generate_Toys(effFile,effname.data(),effdenname.data(),rnd);
        neweff->Write(effname.data());
        outfile->Close();
    }

    for(int i=0;i<toyn;i++)
    {
        const std::string accname="hGenUpsilonPtvsRapSlide5";
        const std::string accdenname="hGenUpsilonAccDen";
        const std::string filename = std::string(outputpath)+"/OutAcc2D_"+std::to_string(i)+".root";

        TFile* outfile =OpenFile(filename.data(),"CREATE");

        TH2* newacc =generate_Toys(accFile,accname.data(),accdenname.data(),rnd);
        newacc->Write(accname.data());
        outfile->Close();
    }

}

TH2* generate_Toys(TFile* file,const char* th2Name, const char* th2DenName, TRandom3* rand)
{
    TH2* eff = dynamic_cast<TH2*> (file->Get(th2Name));
    TH2* tot = dynamic_cast<TH2*> (file->Get(th2DenName));

    if ((eff== nullptr)||(tot ==nullptr))
    {
        throw std::invalid_argument("TH2 not found");
    }

    TH2* eff_toy = dynamic_cast<TH2*>(eff->Clone((std::string(eff->GetName())+"_toy").data()));

    int nbinsx= eff->GetNbinsX();
    int nbinsy= eff->GetNbinsY();

    for(int i=1;i<=nbinsx;i++)
    {
        for(int j=1;j<=nbinsy;j++)
        {
            int tbin = tot->GetBin(i,j);
            int bin= eff->GetBin(i,j);

            Double_t total = tot->GetBinContent(tbin);

            Double_t oldvalue = eff->GetBinContent(bin);
            Double_t newvalue = rand->Binomial( total, oldvalue ) / total;
            eff_toy->SetBinContent(bin, newvalue );
        }
    }

    return eff_toy;
}