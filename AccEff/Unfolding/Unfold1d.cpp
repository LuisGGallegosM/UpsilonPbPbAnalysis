#include <iostream>

#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"

#include "../../Utils/Helpers/Helpers.h"
#include "../../OniaBase/Params/Params.h"
#include "../Common/AccEffAux.h"
#include "../../Drawing/Drawing.h"

#include "Unfolder.h"

void Unfold_Train1D(const char* filename,const char* outputfilename)
{
    std::cout << "\nUNFOLDING TRAINING 1D\n";

    //input file
    std::cout << "Reading input file: " << filename <<'\n';
    TFile* file = OpenFile(filename,"READ");
    
    //output file
    std::string outfilename= outputfilename;
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    TTree *myTree = GetTree(file,"hionia/myTree");
    TTree *jetTree = GetTree(file,"ak4PFJetAnalyzer/t");
    TTree *skimTree = GetTree(file,"skimanalysis/HltTree");

    myTree->AddFriend(jetTree);
    myTree->AddFriend(skimTree);

    Unfolder1D unfolder(myTree);

    std::cout << "Starting training\n";

    unfolder.Train();

    TH1D* measured_train = unfolder.getMeasuredTraining();
    TH1D* truth_train = unfolder.getTruthTraining();
    TH1D* measured_test = unfolder.getMeasuredTest();
    TH1D* truth_test = unfolder.getTruthTest();

    auto response= unfolder.getResponse();
    TMatrixD* R = (TMatrixD*) (response->Mresponse()).Clone();
    TH2* Rth = (TH2*) (response->Hresponse())->Clone();

    R->Print();

    DrawHist(R, ReplaceExtension(outfilename.data(),"_response_matrix.png"));
    DrawHist(Rth, ReplaceExtension(outfilename.data(),"_response_hist.png"));
    DrawHist(measured_train,ReplaceExtension(outfilename.data(),"_measured_train.png"));
    DrawHist(truth_train,ReplaceExtension(outfilename.data(),"_truth_train.png"));
    DrawHist(measured_test,ReplaceExtension(outfilename.data(),"_measured_test.png"));
    DrawHist(truth_test,ReplaceExtension(outfilename.data(),"_truth_test.png"));

    std::vector<TH1*> trains_samples{measured_train, truth_train};
    writeToCanvas(trains_samples,"training samples: truth and measured","jt_{pt}","evts",ReplaceExtension(outfilename.data(),"_comparison_train.png"));

    std::vector<TH1*> tests_samples{measured_test, truth_test};
    writeToCanvas(tests_samples,"testing samples: truth and measured","jt_{pt}","evts",ReplaceExtension(outfilename.data(),"_comparison_test.png"));

    response->Write("response");
    R->Write("responseMatrix");
    Rth->Write("responseTH");
    
    measured_train->Write();
    truth_train->Write();
    measured_test->Write();
    truth_test->Write();

    //clean up
    outputfile->Close();
    file->Close();
    delete file;
    delete outputfile;
    std::cout << "Success.\n Data wrote to '" << outfilename<< "' root file\n";
}

void Unfold1D(const char* filename,const char* outputfilename)
{
    std::cout << "\nUNFOLDING 1D\n";

    //input file
    std::cout << "Reading input file: " << filename <<'\n';
    TFile* file = OpenFile(filename,"READ");
    
    //output file
    std::string outfilename= outputfilename;
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    std::cout << "Starting unfolding\n";

    RooUnfoldResponse* response= Get<RooUnfoldResponse>(file,"response");
    TH1D* measured_test= Get<TH1D>(file,"measured_test");
    TH1D* truth_test= Get<TH1D>(file,"truth_test");
    TH1D* measured_train = Get<TH1D>(file,"measured_train");
    TH1D* truth_train= Get<TH1D>(file,"truth_train");

    std::vector<RooUnfold*>  unfolders;

    for(int its : iterations)
    {
        RooUnfold* unfold= new RooUnfoldBayes(response,measured_test,its,false,false,"unfold");
        unfold->SetName(("unfold_"+std::to_string(its)).data());
        unfolders.push_back(unfold);
    } 

    std::vector<TH1*> ratios;
    std::vector<TH1*> ratios_train;
    std::vector<TH1*> ratios_refolded;

    for(int i=0;i < unfolders.size();i++)
    {
        TH1D* unfolded = dynamic_cast<TH1D*>(unfolders[i]->Hunfold(RooUnfolding::kCovariance));
        unfolders[i]->PrintTable(std::cout,truth_test);
        std::string name="unfolded_hist_"+std::to_string(iterations[i]);
        unfolded->SetName(name.data());
        unfolded->SetTitle(name.data());
        std::string nam_out="unfolded_"+std::to_string(iterations[i]);
        std::string nam=nam_out +".png";
        
        DrawHist( unfolded,ReplaceExtension(outfilename.data(),nam.data()));

        std::string postfix="_"+std::to_string( iterations[i]);

        TH1D* ratio= new TH1D( (*unfolded)/(*truth_test)  );
        ratio->SetName( ("ratio"+postfix).data() );
        ratios.push_back(ratio);
        ratio->Write();
        DrawHist(ratio, ReplaceExtension(outfilename.data(),("_ratio"+ postfix +".png").data()));

        TH1D* ratio_train= new TH1D( (*unfolded)/(*truth_train)  );
        ratio_train->SetName( ("ratio_train"+postfix).data() );
        ratios_train.push_back(ratio_train);
        ratio_train->Write();
        DrawHist(ratio_train, ReplaceExtension(outfilename.data(),("_ratio_train"+ postfix +".png").data()));

        TH1D* refolded = dynamic_cast<TH1D*>(response->ApplyToTruth(unfolded,("refolded"+postfix).data()));

        DrawHist(refolded,ReplaceExtension(outfilename.data(),("_refolded"+postfix+".png").data()));

        TH1D* ratio_refold = new TH1D((*refolded)/(*measured_test));
        ratio_refold->SetName(("ratio_refold"+postfix).data());
        ratios_refolded.push_back(ratio_refold);

        DrawHist(ratio_refold,ReplaceExtension(outfilename.data(),("_ratio_refolded"+postfix+".png").data()));

        refolded->Write(("refolded"+postfix).data());
        ratio_refold->Write(("ratio_refolded"+postfix).data());

        unfolded->Write(nam_out.data());
        unfolders[i]->Write();
    }

    writeToCanvas(ratios,"pseudodata unfolded/truth ratio", v1d_jtpt ? "jt_{pT}" : "z^2","ratio",ReplaceExtension(outfilename.data(),"_ratios.png"));
    writeToCanvas(ratios_train,"pseudodata unfolded/truth ratio",v1d_jtpt ? "jt_{pT}" : "z^2","ratio",ReplaceExtension(outfilename.data(),"_ratios_train.png"));
    writeToCanvas(ratios_refolded,"pseudodata refolded/measured ratio",v1d_jtpt ? "jt_{pT}" : "z^2","ratio",ReplaceExtension(outfilename.data(),"_ratios_refolded.png"));
    //clean up
    outputfile->Close();
    file->Close();
    delete file;
    delete outputfile;
    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
    return;
}