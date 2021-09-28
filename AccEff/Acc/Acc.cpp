

#include "TTree.h"
#include "TFile.h"

#include "../../Utils/Helpers/Helpers.h"
#include "AccAnalyzer.h"

#include <iostream>

void AccTestUnWeighted(TTree *myTree, const std::string& outfilename);
void AccTestWeighted1D(TTree *myTree, const std::string& outfilename, WeightFunc* weights);
void AccTestWeighted2D(TTree *myTree, const std::string& outfilename, WeightFunc2D* weights);

/**
 * @brief Acceptancy test function
 * 
 * @param filename root file path for input.
 * @param outputfilename output root file path with passing onia.
 * @param configname 
 */
void AccTest(const char* filename,const char* outputfilename, const char* yieldfitfuncFilename)
{
    std::cout << "\nACCEPTANCY TEST\n";

    //input file
    std::cout << "Reading input file: " << filename <<'\n';
    std::unique_ptr<TFile> file(OpenFile(filename,"READ"));

    std::unique_ptr<TFile> yieldFitFile;
    if(yieldfitfuncFilename!=nullptr)
    {
        std::cout << "Reading yield weight function input file: " << yieldfitfuncFilename <<'\n';
        yieldFitFile.reset(OpenFile(yieldfitfuncFilename,"READ"));
    }

    //output file
    std::string outfilename=outputfilename;
    std::cout << "Writing to output file: " << outfilename <<'\n';
    std::unique_ptr<TFile> outputfile(OpenFile(outfilename.data(), "RECREATE"));

    TTree *myTree = GetTree(file.get(),"hionia/myTree");

    if (yieldFitFile!=nullptr)
    {
        RooAbsReal* rooAbs = dynamic_cast<RooAbsReal*>( yieldFitFile->Get(weightFuncRooAbsName) );
        if (rooAbs!=nullptr)
        {
            WeightFuncRooAbs weightFunc(rooAbs);
            AccTestWeighted1D(myTree,outfilename,&weightFunc);
        }
        else
        {
            TH2F* th2 = dynamic_cast<TH2F*>( yieldFitFile->Get(weightFuncTH2Name) );
            if (th2!=nullptr)
            {
                WeightFuncTH2 weightFunc(th2);
                AccTestWeighted2D(myTree,outfilename,&weightFunc);
            }
            else
            {
                std::cout << "Error: No Weight function found in weight file\n";
                std::cout << "names can be:\n";
                std::cout << "RooAbs: " <<  weightFuncRooAbsName << "\n";
                std::cout << "TH2" << weightFuncTH2Name << "\n";
            }
        }
    }
    else
    {
        AccTestUnWeighted(myTree,outfilename);
    }
}

void AccTestUnWeighted(TTree *myTree, const std::string& outfilename)
{
    AccAnalyzer accAnalyzer(myTree,"DetectableOnia");

    //write plots to same folder as outputfilename and prefixed with outputfilename
    std::string outputfilesBasename=ReplaceExtension(outfilename.data(),"");

    //Run acceptancy test
    accAnalyzer.Test(outputfilesBasename);
    accAnalyzer.getHists()->Write();

    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
}

void AccTestWeighted1D(TTree *myTree, const std::string& outfilename, WeightFunc* weights)
{
    AccAnalyzer accAnalyzer(myTree,"DetectableOnia", weights);
    //write plots to same folder as outputfilename and prefixed with outputfilename
    std::string outputfilesBasename=ReplaceExtension(outfilename.data(),"");

    //Run acceptancy test
    accAnalyzer.Test(outputfilesBasename);
    accAnalyzer.getHists()->Write();

    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
}

void AccTestWeighted2D(TTree *myTree, const std::string& outfilename, WeightFunc2D* weights)
{
    AccAnalyzer accAnalyzer(myTree,"DetectableOnia", weights);
    //write plots to same folder as outputfilename and prefixed with outputfilename
    std::string outputfilesBasename=ReplaceExtension(outfilename.data(),"");

    //Run acceptancy test
    accAnalyzer.Test(outputfilesBasename);
    accAnalyzer.getHists()->Write();

    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
}