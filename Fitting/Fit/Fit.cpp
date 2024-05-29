

#include "Fit.h"

/**
 * @brief Do a fitting using a skimmed data from Skimming Package
 * 
 * @param inputfilename path to input root file to fit
 * @param cutfilename path to cut configuration file used to skim input data (.cutconf).
 * @param outfilename path to a directory where to save results (4 files).
 * @param fitconfigname path to fit configuration file (.fitconf).
 */
void Fit(const char* inputfilename, const char* cutfilename, const char* fitconfigname, const char* outfilename, const char* weightfilename)
{
    std::cout << "\nFITTING\n";
    std::cout << "Reading input file: " << inputfilename <<'\n';
    std::cout << "Reading input file: " << cutfilename <<'\n';
    std::cout << "Reading input file: " << fitconfigname <<'\n';
    std::cout << "Writing output file: " << outfilename <<'\n';

    //input file
    TFile* inputfile = OpenFile(inputfilename, "READ");

    TFile* weightfile = nullptr;
    if(weightfilename)
    {
        std::cout << "Reading weight input file: " << weightfilename <<'\n';
        weightfile = OpenFile(weightfilename, "READ");
    }

    //output file
    TFile* outputfile = OpenFile(outfilename,"CREATE");

    //fit configuration file
    ParameterGroup config;
    config.deserialize(fitconfigname);

    //read tree
    TTree *tree_skimmed = (TTree *)inputfile->Get(oniaTreeName);

    if(weightfile)
    {
        TTree *tree_weight = (TTree *)weightfile->Get(oniaTreeName);
        if(tree_weight==nullptr)
        {
            throw std::runtime_error("Tree not found "+std::string(oniaTreeName));
        }
        tree_skimmed->AddFriend(tree_weight);
    }

    std::unique_ptr<OniaMassFitter> massFitter=createMassFitter(tree_skimmed,&config);

    //copy fit and cut config files, same filename but with correct extension
    CopyFile(fitconfigname, ReplaceExtension(outfilename,".fitconf").data());
    CopyFile(cutfilename,ReplaceExtension(outfilename,".cutconf").data() );

    //Fit
    RooAbsReal* fittedFunc = massFitter->fit();
    
    ParameterGroup fParams = massFitter->getFitParams();

    //output fit result file
    fParams.serialize(ReplaceExtension(outfilename,".fit").data());

    massFitter->getResults()->Print();

    outputfile->cd();
    massFitter->getDataset()->Write();
    massFitter->getResults()->Write("fitresults");
    fittedFunc->Write();
    massFitter->getVar()->Write("mass");

    //clean up
    inputfile->Close();
    outputfile->Close();
    delete inputfile;
    delete outputfile;
}

