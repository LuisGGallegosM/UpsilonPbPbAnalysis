

#include "AccEff.h"

using std::ofstream;

/**
 * @brief Acceptancy test
 * 
 * @param file File where to get the tree to acceptancy test.
 * @param wroteTreeName Name of the acceptancy tree to write
 * @return Output data.
 */
void AccTest(const char* filename,const char* outputfilename, const char* configname)
{
    std::cout << "\nACCEPTANCY TEST\n";

    //input file
    std::cout << "Reading input file: " << filename <<'\n';
    TFile* file = OpenFile(filename,"READ");
    
    //output file
    std::string outfilename=outputfilename;
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    TTree *myTree = (TTree *)file->Get("hionia/myTree");
    if (myTree == nullptr)
    {
        std::cout << "hionia/myTree tree not found\n";
        throw std::runtime_error("Error: Tree not found.\n");
    }

    AccCutter* cutterAcc =new AccCutter();
    OniaWriter* writer =new OniaWriterBase("DetectableOnia",QQtype::Gen);
    OniaReader* reader=new OniaReader(myTree);
    std::unique_ptr<AccAnalyzer> accAnalyzer(new AccAnalyzer(reader,cutterAcc,writer));

    //Run acceptancy test
    accAnalyzer->Test();

    //write results to same folder as outputfilename
    std::string outputfilesBasename=ReplaceExtension(outfilename.data(),"");
    accAnalyzer->Write(outputfilesBasename);

    outputfile->Close();
    file->Close();
    delete file;
    delete outputfile;
    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
}

/**
 * @brief Efficiency test
 * 
 * @param file File where to get the tree to acceptancy test.
 * @param wroteTreeName Name of the acceptancy tree to write
 * @return Output data.
 */
void EffTest(const char* filename,const char* outputfilename, const char* configname)
{
    std::cout << "\nEFFICIENCY TEST\n";

    //input file
    std::cout << "Reading input file: " << filename <<'\n';
    TFile* file = OpenFile(filename,"READ");

    //input file of acceptancy
    std::string accfilename=outputfilename;
    std::cout << "Reading efficiency input file: " << filename <<'\n';
    TFile* accfile = OpenFile(accfilename.data(),"READ");
    
    //output file
    std::string outfilename= ReplaceExtension(outputfilename,"Eff.root");
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    TTree *myTree = (TTree *)file->Get("hionia/myTree");
    if (myTree == nullptr)
    {
        std::cout << "hionia/myTree tree not found\n";
        return;
    }

    cutParams cut;
    cut.deserialize(configname);
    if(!cut.isValid())
    {
        std::cerr << "Error: Invalid cut parameters.\n";
        return;
    }

    EffCutter* cutterAcc =new EffCutter(&cut);
    OniaWriter* writer =new OniaWriterBase("RecoCutOnia",QQtype::Reco);
    OniaReader* reader=new OniaReader(myTree);

    TH1F* ptQQdetected=(TH1F*)accfile->Get("pt QQ Detected");
    if(ptQQdetected==nullptr)
    {
        std::cerr << "Error: 'pt QQ Detected' histogram not found.\n";
        return;
    }

    EffAnalyzer effAnalyzer(reader,cutterAcc,writer,ptQQdetected);

    //Run efficiency test
    effAnalyzer.Test();

    //write results to same folder as outputfilename
    std::string outputfilesBasename=outfilename;
    effAnalyzer.Write(outputfilesBasename);

    accfile->Close();
    outputfile->Close();
    file->Close();
    delete file;
    delete outputfile;
    delete accfile;
    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
    return;
}

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    std::string flags(argv[1]);
    if (argc == 5)
    {
        if (flags=="-acc")
            AccTest(argv[2],argv[3],argv[4]);
        else if (flags=="-eff")
            EffTest(argv[2],argv[3],argv[4]);
    }
    else if (argc == 6)
    {
        if (flags=="-final")
            AccEffResults(argv[2],argv[3],argv[4],argv[5]);
    }
    else
    {
        std::cerr << "Incorrect number of parameters\n";
    }
    return 0;
}

#endif