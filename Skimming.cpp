#include "Main.h"

TTree* oniaSkim(TFile *file,const char* wroteTreeName, std::unique_ptr<Onia_Aux>* auxData, const cutParams* kineCut);
TTree* jetSkim(TFile *file,const char* wroteTreeName, Onia_Aux* auxData);
void saveCutParams(const char* filename,const cutParams*);

using std::ofstream;

/**
 * @brief Execute Onia Upsilon 1S skimming and Jets.
 * 
 * @param filename Name of file where to find the tree to skim.
 * @param outputfilename Name of the root output file to save skimmed data.
 */
void Skim(const char* filename,const char* outputfilename, const cutParams* cut)
{
    //input file
    TFile file(filename, "READ");

    //output file
    TFile outputfile(outputfilename, "RECREATE");

    if (file.IsZombie()) //input file is found?
    {
        std::cout << "file cannot readed\n";
        return;
    }

    //tree to write skimmed data
    std::unique_ptr<Onia_Aux> auxData;

    TTree* onia_skimmed =  oniaSkim(&file,ONIATTREENAME,&auxData,cut);
    if (onia_skimmed==nullptr) return;
    onia_skimmed->Write(0,TObject::kOverwrite);

    TTree* jet_skimmed = jetSkim(&file,JETTTREENAME,auxData.get());
    if (jet_skimmed==nullptr) return;
    jet_skimmed->Write(0,TObject::kOverwrite);

    saveCutParams(filename,cut);

    outputfile.Close();
    file.Close();
    std::cout << "Success.\n TTrees wrote to '" << outputfilename<< "' root file\n";
    return;
}

/**
 * @brief Skim 
 * 
 * @param file File where to get the tree to skim.
 * @param wroteTreeName Name of the skimmed tree to write
 * @param auxData A place where to save auxiliary data used for jet skimming.
 * @return Tree with skimmed data.
 */
TTree* oniaSkim(TFile *file,const char* wroteTreeName, std::unique_ptr<Onia_Aux>* auxData, const cutParams* cut)
{
    TTree *myTree = (TTree *)file->Get("hionia/myTree");
    if (myTree == nullptr)
    {
        std::cout << "hionia/myTree tree not found\n";
        return nullptr;
    }

    //execute skim
    OniaCutter cutter(cut);

    OniaSkimmer skimmer = OniaSkimmer(myTree,wroteTreeName);
    TTree* wroteTree = skimmer.Skim(cutter);

    (*auxData) = std::move(skimmer.auxData);

    return wroteTree;
}

/**
 * @brief Same functionality as oniaSkim, but for jets.
 * 
 * @param file File where to get the tree to skim.
 * @param wroteTreeName Name of the skimmed tree to write
 * @param auxData A place where to get auxiliary data used for jet skimming.
 * @return Tree with skimmed data.
 */
TTree* jetSkim(TFile *file,const char* wroteTreeName, Onia_Aux* auxData)
{
    TTree *myTree = (TTree *)file->Get("ak3PFJetAnalyzer/t");
    if (myTree == nullptr)
    {
        std::cout << "ak3PFJetAnalyzer/t tree not found\n";
        return nullptr;
    }

    JetCutter cutter(auxData);
    
    JetSkimmer skimmer(myTree,wroteTreeName,auxData);
    TTree* wroteTree =skimmer.Skim(cutter);

    return wroteTree;
}

void saveCutParams(const char* filename,const cutParams* cut)
{
    std::string filen= std::string(filename) + ".txt";
    ofstream file(filen.data());

    serialize(file,"isMC",cut->isMC);
    serialize(file,"checkSign",cut->checkSign);
    serialize(file,"trigSelect",cut->trigSelect);
    serialize(file,"sign",cut->sign);
    serialize(file,"selectionBits",cut->selectionBits);
    serialize(file,"minTracks",cut->minTracks);
    serialize(file,"minPixels",cut->minPixels);
    serialize(file,"maxDxy",cut->maxDxy);
    serialize(file,"maxDz",cut->maxDz);
    serialize(file,"minVtxProb",cut->minVtxProb);

    serialize(file,"ptLow",cut->ptLow);
    serialize(file,"ptHigh",cut->ptHigh);
    serialize(file,"yLow",cut->yLow);
    serialize(file,"yHigh",cut->yHigh);
    serialize(file,"singleMuPtLow",cut->singleMuPtLow);
    serialize(file,"singleMuEtaHigh",cut->singleMuEtaHigh);
    serialize(file,"massLow",cut->massLow);
    serialize(file,"massHigh",cut->massHigh);

    file.close();
}