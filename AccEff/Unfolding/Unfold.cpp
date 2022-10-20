#include <iostream>

#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"

#include "../../Utils/Helpers/Helpers.h"
#include "../../OniaBase/Params/Params.h"
#include "../Common/AccEffAux.h"
#include "../../Drawing/Drawing.h"

#include "Unfolder.h"
#include "Helpers.h"

TMatrixD normalizeTH2D(const TH2* hist);
TVectorD h2v  (const TH2* h);

void Unfold_Train(const char* filename,const char* outputfilename)
{
    std::cout << "\nUNFOLDING TRAINING\n";

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

    Unfolder2D unfolder(myTree);

    std::cout << "Starting training\n";

    unfolder.Train();

    TH2D* measured_train = unfolder.getMeasuredTraining();
    TH2D* truth_train = unfolder.getTruthTraining();
    TH2D* measured_test = unfolder.getMeasuredTest();
    TH2D* truth_test = unfolder.getTruthTest();

    auto response= unfolder.getResponse();
    TMatrixD* Ruf = (TMatrixD*) (response->Mresponse()).Clone();
    
    TH2* Rth = (TH2*) (response->Hresponse())->Clone();
    TMatrixD* R = new TMatrixD(normalizeTH2D(Rth));

    R->Print();

    DrawHist(Ruf, ReplaceExtension(outfilename.data(),"_response_matrix_lib.png"));

    DrawHist(R, ReplaceExtension(outfilename.data(),"_response_matrix.png"));
    DrawHist(Rth, ReplaceExtension(outfilename.data(),"_response_hist.png"));
    DrawHist(measured_train,ReplaceExtension(outfilename.data(),"_measured_train.png"));
    DrawHist(truth_train,ReplaceExtension(outfilename.data(),"_truth_train.png"));
    DrawHist(measured_test,ReplaceExtension(outfilename.data(),"_measured_test.png"));
    DrawHist(truth_test,ReplaceExtension(outfilename.data(),"_truth_test.png"));

    //draw projection z
    for(int i=0;i< jtptbins_n;i++)
    {
        std::string postfix=generatePostfix(i);
        std::string projx_t_name="truth_test_z"+postfix;
        std::string projx_m_name="measured_test_z"+postfix;
        std::string hist_m_name ="_measured_z"+postfix+".png";
        std::string hist_t_name ="_truth_z"+postfix+".png";

        auto truth_test_z= truth_test->ProjectionX(projx_t_name.data(),1+i,1+i,"eo");
        auto measured_test_z= measured_test->ProjectionX(projx_m_name.data(),1+i,1+i,"eo");
        
        DrawHist(measured_test_z,ReplaceExtension(outfilename.data(),hist_m_name.data()));
        DrawHist(truth_test_z,ReplaceExtension(outfilename.data(),hist_t_name.data()));

        std::vector<TH1*> comparison {truth_test_z,measured_test_z};
        std::string hist_comp_name="_comparison_z_"+postfix+".png";
        writeToCanvas(comparison,"truth vs measured comparison","z^2","count",ReplaceExtension(outfilename.data(),hist_comp_name.data()));
        
        measured_test_z->Write();
        truth_test_z->Write();
    }

    //draw projection jtpt
    auto truth_test_jtpt= truth_test->ProjectionY("truth_test_jtpt",0,-1,"eo");
    auto measured_test_jtpt= measured_test->ProjectionY("measured_test_jtpt",0,-1,"eo");
    
    DrawHist(measured_test_jtpt,ReplaceExtension(outfilename.data(),"_measured_jtpt.png"));
    DrawHist(truth_test_jtpt,ReplaceExtension(outfilename.data(),"_truth_jtpt.png"));
    
    measured_test_jtpt->Write();
    truth_test_jtpt->Write();

    response->Write("response");
    R->Write("responseMatrix");
    Ruf->Write("responseMatrixTest");
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

void Unfold(const char* filename,const char* outputfilename)
{
    std::cout << "\nUNFOLDING\n";

    //input file
    std::cout << "Reading input file: " << filename <<'\n';
    TFile* file = OpenFile(filename,"READ");
    
    //output file
    std::string outfilename= outputfilename;
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    std::cout << "Starting unfolding\n";

    RooUnfoldResponse* response= Get<RooUnfoldResponse>(file,"response");
    TH2D* measured_test= Get<TH2D>(file,"measured_test");
    TH2D* truth_test= Get<TH2D>(file,"truth_test");
    TH2D* measured_train = Get<TH2D>(file,"measured_train");
    TH2D* truth_train= Get<TH2D>(file,"truth_train");
    TMatrixD* responseMatrix = Get<TMatrixD>(file,"responseMatrix");

    std::vector<TH1D*> truth_test_z;
    std::vector<TH1D*> measured_test_z;
    for(int i=0;i<jtptbins_n;i++)
    {
        std::string postfix=generatePostfix(i);
        std::string obj_name= "truth_test_z"+ postfix;
        TH1D* hist= Get<TH1D>(file,obj_name.data());
        truth_test_z.push_back(hist);
        obj_name= "measured_test_z"+ postfix;
        hist= Get<TH1D>(file,obj_name.data());
        measured_test_z.push_back(hist);
    }

    std::vector<RooUnfold*>  unfolders;

    for(int its : iterations)
    {
        RooUnfold* unfold= new RooUnfoldBayes(response,measured_test,its,false,false,"unfold");
        unfold->SetName(("unfold_"+std::to_string(its)).data());
        unfolders.push_back(unfold);
    } 

    std::vector<std::vector<TH1*>> ratios_z;
    ratios_z.resize(jtptbins_n);

    std::vector<std::vector<TH1*>> ratios_refolded_z;
    ratios_refolded_z.resize(jtptbins_n);

    for(int i=0;i < unfolders.size();i++)
    {
        TH2* unfolded = dynamic_cast<TH2*>(unfolders[i]->Hunfold(RooUnfolding::kCovariance));
        unfolders[i]->PrintTable(std::cout,truth_test);
        std::string name="unfolded_hist_"+std::to_string(iterations[i]);
        unfolded->SetName(name.data());
        unfolded->SetTitle(name.data());
        std::string nam_out="unfolded_"+std::to_string(iterations[i]);
        std::string nam=nam_out +".png";
        
        DrawHist( unfolded,ReplaceExtension(outfilename.data(),nam.data()));

        //auto v= h2v(unfolded);
        //TH2* refolded = v2h( (*responseMatrix)*v ,unfolded);

        TH2D* refolded = dynamic_cast<TH2D*>(response->ApplyToTruth(unfolded,("refolded_"+std::to_string(iterations[i])).data()));

        DrawHist(refolded,ReplaceExtension(outfilename.data(),("_refolded_"+std::to_string(iterations[i])+".png").data()));

        for(int j=0;j< jtptbins_n;j++)
        {
            std::string postfix=generatePostfix(j)+"_"+std::to_string(i);
            TH1D* unfolded_z = unfolded->ProjectionX((nam_out+"_z"+postfix).data(),1+j,1+j,"eo");
            unfolded_z->SetName( ( "unfolded_z"+postfix ).data());
            DrawHist( unfolded_z,ReplaceExtension(outfilename.data(),(nam_out+"unfolded_z"+postfix+".png").data()));

            TH1D* ratio_z= new TH1D( (*unfolded_z)/(*truth_test_z[j])  );
            ratio_z->SetName( ("ratio_z"+postfix).data() );
            ratios_z[j].push_back(ratio_z);
            unfolded_z->Write();
            ratio_z->Write();

            TH1D* refolded_z = refolded->ProjectionX((nam_out+"_z"+postfix).data(),1+j,1+j,"eo");
            refolded_z->SetName( ( "refolded_z"+postfix ).data());
            DrawHist( refolded_z,ReplaceExtension(outfilename.data(),(nam_out+"refolded_z"+postfix+".png").data()));

            TH1D* ratio_refold = new TH1D((*refolded_z)/(*measured_test_z[j]));
            ratio_refold->SetName(("ratio_refold_z"+postfix).data());
            ratios_refolded_z[j].push_back(ratio_refold);
            refolded_z->Write();
            ratio_refold->Write();

            DrawHist(ratio_refold,ReplaceExtension(outfilename.data(),("_ratio_refolded"+postfix+".png").data()));
        }

        TH1D* unfolded_jtpt = unfolded->ProjectionY((nam_out+"_jtpt").data(),0,-1,"eo");
        DrawHist( unfolded_jtpt,ReplaceExtension(outfilename.data(),(nam_out+"unfolded_jtpt.png").data()));

        unfolded_jtpt->Write();

        unfolded->Write(nam_out.data());
        refolded->Write();
        unfolders[i]->Write();
    }

    for(int i=0;i< jtptbins_n;i++)
    {
        std::string postfix=generatePostfix(i);
        std::string on="_ratios_z"+postfix+".png";
        writeToCanvas(ratios_z[i],"pseudodata unfolded/truth ratio","z^2","ratio",ReplaceExtension(outfilename.data(),on.data()),false,0.70f,1.3f);
    }

        for(int i=0;i< jtptbins_n;i++)
    {
        std::string postfix=generatePostfix(i);
        std::string on="_ratios_refolded_z"+postfix+".png";
        writeToCanvas(ratios_refolded_z[i],"pseudodata refolded/measured ratio","z^2","ratio",ReplaceExtension(outfilename.data(),on.data()),false,0.70f,1.3f);
    }
   
    //clean up
    outputfile->Close();
    file->Close();
    delete file;
    delete outputfile;
    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
    return;
}

TMatrixD normalizeTH2D(const TH2* hist)
{
    const int binx=hist->GetNbinsX();
    const int biny= hist->GetNbinsY();

    TMatrixD output(binx,biny);

    for(int y=0;y< biny ;y++)
    {
        float norm_factor=0.0f;
        for(int x=0;x < binx;x++ )
        {
            norm_factor+= hist->GetBinContent(x+1,y+1);
        }
        if(norm_factor!=0.0f) norm_factor=1.0f/norm_factor;
        for(int x=0;x< binx;x++)
        {
            output(x,y)= hist->GetBinContent(x+1,y+1)*norm_factor;
        }
    }
    return output;

}

TVectorD h2v  (const TH2* h){
    TVectorD v;
    int nbinstotal = h->GetNbinsX()*h->GetNbinsY();
    v.ResizeTo(nbinstotal);
    int n = 0;
    for (int j= 1; j < h->GetNbinsY()+1; j++){
        for(int i=1; i < h->GetNbinsX()+1;i++)
        {
            v[n] = h->GetBinContent(i,j);
        }
      ++n;
    }
    return v;
  }

// TH2* v2h  (const TVectorD& v){
//     TH2* h= new TH2D()
//     int nbinstotal = h->GetNbinsX()*h->GetNbinsY();
//     v.ResizeTo(nbinstotal);
//     int n = 0;
//     for (int j= 1; j < h->GetNbinsY()+1; j++){
//         for(int i=1; i < h->GetNbinsX()+1;i++)
//         {
//             v[n] = h->GetBinContent(i,j);
//         }
//       ++n;
//     }
//     return v;
//   }

//helpers
