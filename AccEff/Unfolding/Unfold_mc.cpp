#include <iostream>

#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TDecompSVD.h"

#include "../../Utils/Helpers/Helpers.h"
#include "../../OniaBase/Params/Params.h"
#include "../Common/AccEffAux.h"
#include "../../Drawing/Drawing.h"

#include "Unfolder.h"
#include "Helpers.h"

const bool flatPrior=true;

void Unfold_Test_DAngostini(TH2D* measured_test, TH2D* truth_test, const std::vector<TH1D*>& measured_test_z,const std::vector<TH1D*>& truth_test_z,RooUnfoldResponse* response, const std::string& outputdir, const std::vector<int>& iterations);
void Unfold_Test_MatrixInversion(TH2D* measured_test, TH2D* truth_test, const std::vector<TH1D*>& measured_test_z,const std::vector<TH1D*>& truth_test_z,RooUnfoldResponse* response, const std::string& outputdir);

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

    Unfolder2D unfolder(myTree,flatPrior);

    std::cout << "Starting training\n";

    unfolder.Train();

    TH2D* measured_train = unfolder.getMeasuredTraining();
    TH2D* truth_train = unfolder.getTruthTraining();
    TH2D* measured_test = unfolder.getMeasuredTest();
    TH2D* truth_test = unfolder.getTruthTest();

    auto response= unfolder.getResponse();
    TMatrixD* R = (TMatrixD*) (response->Mresponse()).Clone();
    
    TH2* Rth = (TH2*) (response->Hresponse())->Clone();

    R->Print();

    DrawHistM(R, ReplaceExtension(outfilename.data(),"_response_matrix.png"));
    DrawHistM(Rth, ReplaceExtension(outfilename.data(),"_response_hist.png"));
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

void Unfold_Test(const char* filename,const char* outputfilename)
{
    std::cout << "\nUNFOLDING TESTING\n";

    //input file
    std::cout << "Reading input file: " << filename <<'\n';
    TFile* file = OpenFile(filename,"READ");
    
    //output file
    std::string outfilename= outputfilename;
    const std::string outputDir= RemoveFilename(outfilename);
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    std::cout << "Starting unfolding\n";

    RooUnfoldResponse* response= Get<RooUnfoldResponse>(file,"response");
    TH2D* measured_test= Get<TH2D>(file,"measured_test");
    TH2D* truth_test= Get<TH2D>(file,"truth_test");

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

    TDecompSVD *svd= new TDecompSVD (response->Mresponse()); //this is the singular value decomposition (SVD) matrix
    auto singular_values = svd->GetSig(); //this is a vector with the singular values, i.e., the diagonal elements of S. They are ordered from largest to smallest.
    svd->Print();
    singular_values.Print();

    std::cout << "\nD'agostini method\n";
    Unfold_Test_DAngostini(measured_test,truth_test,measured_test_z,truth_test_z,response,outputDir+"/dagostini",iterations);
    std::cout << "\nMatrix inversion method\n";
    Unfold_Test_MatrixInversion(measured_test,truth_test,measured_test_z,truth_test_z,response,outputDir+"/inversion");

    //clean up
    outputfile->Close();
    file->Close();
    delete file;
    delete outputfile;
    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
    return;
}

void Unfold_Test_MatrixInversion(TH2D* measured_test, TH2D* truth_test, const std::vector<TH1D*>& measured_test_z,const std::vector<TH1D*>& truth_test_z,RooUnfoldResponse* response, const std::string& outputdir)
{
    RooUnfoldInvert unfold(response,measured_test,"unfold");
    unfold.SetName("unfold_inv");

    TMatrixD matrix=  response->Mresponse();

    TMatrixD inv_matrix = matrix.Invert() ;
    inv_matrix.Print();
    DrawHistM(& inv_matrix,outputdir+"/invertedMatrix.png");

    TH2* unfolded = dynamic_cast<TH2*>(unfold.Hreco(RooUnfold::kCovariance));
    outputTH(unfolded,"unfolded_inv",outputdir);

    const std::string refolded_name="refolded_inv";
    TH2D* refolded = dynamic_cast<TH2D*>(response->ApplyToTruth(unfolded,refolded_name.data()));
    outputTH(refolded,refolded_name,outputdir);

    for(int j=0;j<jtptbins_n;j++)
    {
        const std::string postfix_jt=generatePostfix(j);
        const std::string jtptstr=generateJtptString(j);

        const std::string obj_name="unfolded_z"+postfix_jt;
        TH1D* unfolded_z= unfolded->ProjectionX(obj_name.data(),1+j,1+j,"eo");
        outputTH(unfolded_z,obj_name,"unfolded",outputdir);

        const std::string refolded_name_z="refolded_z"+postfix_jt;
        TH1D* refolded_z = refolded->ProjectionX(refolded_name_z.data(),1+j,1+j,"eo");
        outputTH(refolded_z,refolded_name_z,"refolded",outputdir);

        TH1D* unfolded_ratio_z= new TH1D( (*unfolded_z)/(*truth_test_z[j])  );
        unfolded_ratio_z->GetXaxis()->SetTitle("z^2");
        unfolded_ratio_z->GetYaxis()->SetTitle("ratio");
        outputTH(unfolded_ratio_z,"unfolded_ratio_z"+postfix_jt,  "pseudodata unfolded/truth ratio " + jtptstr,outputdir);

        TH1D* ratio_refold = new TH1D((*refolded_z)/(*measured_test_z[j]));
        ratio_refold->GetXaxis()->SetTitle("z^2");
        ratio_refold->GetYaxis()->SetTitle("ratio");
        outputTH(ratio_refold,"refolded_ratio_z"+postfix_jt,"pseudodata refolded/measured ratio " + jtptstr,outputdir);

        std::vector<TH1*> measured_unfolded_comp{unfolded_z,measured_test_z[j],truth_test_z[j]};
        writeToCanvas(measured_unfolded_comp,"yields comparison","z^2","N",outputdir+"/measured_unfolded_z"+postfix_jt+".png");

    }
}

void Unfold_Test_DAngostini(TH2D* measured_test, TH2D* truth_test, const std::vector<TH1D*>& measured_test_z,const std::vector<TH1D*>& truth_test_z,RooUnfoldResponse* response, const std::string& outputdir, const std::vector<int>& iterations)
{
    std::vector<std::vector<TH1*>> unfolded_ratios_z;
    unfolded_ratios_z.resize(jtptbins_n);

    std::vector<std::vector<TH1*>> refolded_ratios_z;
    refolded_ratios_z.resize(jtptbins_n);

    std::vector<float> Xi2;
    std::vector<float> Xi2_log;
    std::vector<float> pvalue;
    std::vector<float> itF;
    std::vector<float> itF_log;
    int dof=0;

    for(int i=0;i < iterations.size();i++)
    {
        const int its=iterations[i];
        const std::string postfix=std::to_string(its);

        RooUnfold* unfold= new RooUnfoldBayes(response,measured_test,its,false,"unfold");
        unfold->SetName(("unfold_"+postfix).data());

        TH2* unfolded = dynamic_cast<TH2*>(unfold->Hreco(RooUnfold::kCovariance));
        outputTH(unfolded,"unfolded_"+postfix,outputdir+"/unfolded");
        unfold->PrintTable(std::cout,truth_test);

        TH2D* refolded = dynamic_cast<TH2D*>(response->ApplyToTruth(unfolded,("refolded_"+postfix).data()));
        outputTH(refolded,"refolded_"+postfix,outputdir+"/refolded");

        Double_t chi2=0.0f;
        Int_t ndf=0;
        Int_t igood=0;
        float p= refolded->Chi2TestX(measured_test,chi2,ndf,igood,"UUP");

        Xi2.push_back(chi2);
        Xi2_log.push_back(log10(chi2));
        pvalue.push_back(p);
        itF.push_back( iterations[i] );
        itF_log.push_back( log10(iterations[i]) );
        dof=ndf;

        for(int j=0;j< jtptbins_n;j++)
        {
            const std::string postfix_jt=generatePostfix(j)+"_"+postfix;

            const std::string unfolded_z_name="unfolded_z"+postfix_jt;
            TH1D* unfolded_z = unfolded->ProjectionX(unfolded_z_name.data(),1+j,1+j,"eo");
            outputTH(unfolded_z,unfolded_z_name,postfix+" iterations",outputdir+"/unfolded");
            
            TH1D* unfolded_ratio_z= new TH1D( (*unfolded_z)/(*truth_test_z[j])  );
            outputTH(unfolded_ratio_z,"unfolded_ratio_z"+postfix_jt,postfix+" iterations",outputdir+"/unfolded");
            if(!(i % it_draw_skip)) unfolded_ratios_z[j].push_back(unfolded_ratio_z);

            const std::string refolded_z_name="refolded_z"+postfix_jt;
            TH1D* refolded_z = refolded->ProjectionX(refolded_z_name.data(),1+j,1+j,"eo");
            outputTH(refolded_z,refolded_z_name,postfix+" iterations",outputdir+"/refolded");

            TH1D* ratio_refold = new TH1D((*refolded_z)/(*measured_test_z[j]));
            ratio_refold->GetXaxis()->SetTitle("z^2");
            ratio_refold->GetYaxis()->SetTitle("ratio");
            outputTH(ratio_refold,"refolded_ratio_z"+postfix_jt,postfix+" iterations",outputdir+"/refolded");
            if(!(i % it_draw_skip)) refolded_ratios_z[j].push_back(ratio_refold);

            const std::string unfolded_jtpt_name="unfolded_jtpt";
            TH1D* unfolded_jtpt = unfolded->ProjectionY(unfolded_jtpt_name.data(),0,-1,"eo");
            outputTH(unfolded_jtpt,unfolded_jtpt_name,outputdir);

            std::vector<TH1*> measured_unfolded_comp{unfolded_z,measured_test_z[j],truth_test_z[j]};
            writeToCanvas(measured_unfolded_comp,"measured vs unfolded","z^2","N",outputdir+"/unfolded/measured_unfolded_z"+postfix_jt+".png");
        }

    }

    for(int i=0;i< jtptbins_n;i++)
    {
        const std::string postfix=generatePostfix(i);
        const std::string outname_r=outputdir+"/unfolded_ratios_z"+postfix+".png";
        writeToCanvas(unfolded_ratios_z[i],"pseudodata unfolded/truth ratio","z^2","ratio",outname_r,false,0.70f,1.3f);

        const std::string outname_m=outputdir+"/refolded_ratios_z"+postfix+".png";
        writeToCanvas(refolded_ratios_z[i],"pseudodata refolded/measured ratio","z^2","ratio",outname_m,false,0.90f,1.1f);
    }

    outTGraph(itF,Xi2,"ChivsItGraph","#chi^{2} as a function of iterations","iterations","#chi^{2}",outputdir);
    outTGraph(itF,Xi2_log,"ChivsItGraph_log","log_{10}(#chi^{2}) as a function of iterations","iterations","log_{10}(#chi^{2})",outputdir);
    outTGraph(itF,pvalue,"ChivsItGraph_pvalue","pvalue as a function of iterations","iterations","pvalue",outputdir);

    std::cout <<"X^2 comparison tests" << "\n";
    std::cout <<"degrees of freedom:" << dof << "\n";
    for(int i=0;i<Xi2.size();i++)
    {
        std::cout << " it: " << iterations[i] << "  X^2: " << Xi2[i] << " pvalue: " << pvalue[i] << '\n';
    }
}