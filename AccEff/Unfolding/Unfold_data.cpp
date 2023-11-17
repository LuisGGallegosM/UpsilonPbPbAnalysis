#include <iostream>

#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include <map>

#include "../../Utils/Helpers/Helpers.h"
#include "../../OniaBase/Params/Params.h"
#include "../Common/AccEffAux.h"
#include "../../Drawing/Drawing.h"

#include "Unfolder.h"
#include "Helpers.h"
#include "TDecompSVD.h"

using THgroup = std::map<std::string,TH1*>;

THgroup Unfolding_Dagostini(TH2D* measured,const std::vector<TH1D*>& measured_z,const std::string& outputdir,RooUnfoldResponse* response, TH1D* truth_z, const std::vector<int>& iterations);
THgroup Unfolding_MatrixInversion(TH2D* measured,const std::vector<TH1D*>& measured_z,const std::string& outputdir,RooUnfoldResponse* response, TH1D* truth_z);

void yieldComparison(TH1D* data_z, TH1D* mc_z, const std::string& outputdir);

void Unfold(const char* filename, const char* responsefile,const char* outputfilename)
{
    std::cout << "\nUNFOLDING\n";

    //input file
    std::cout << "Reading input file: " << filename <<'\n';
    TFile* file = OpenFile(filename,"READ");

    //response file
    std::cout << "Reading input response file: " << filename <<'\n';
    TFile* response_file = OpenFile(responsefile,"READ");

    const std::string outfilename= outputfilename;

    TFile* yagg_file = OpenFile( (RemoveFilename(outfilename)+"/yagg.root").data(),"READ");
    TH2D* yagg = Get<TH2D>(yagg_file,"yields1S");
    yagg->SetTitle("Measured");
    
    //output file
    const std::string outputDir= RemoveFilename(outfilename);
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    RooUnfoldResponse* response= Get<RooUnfoldResponse>(response_file,"response");

    TH1D* truth_2030 = Get<TH1D>(response_file,"truth_test_z_jtpt2030");
    TH1D* recomc_measured_2030 = Get<TH1D>(response_file,"measured_test_z_jtpt2030");
    recomc_measured_2030->SetTitle("PYTHIA8");

    //TTree *myTree = GetTree(file,"onia_skimmed");

    std::cout << "Starting unfolding\n";

    TH2D* measured=yagg;

    //THFiller_DATA filler(myTree);
    //TH2D* measured= filler.fill();

    DrawHist(measured,ReplaceExtension(outfilename.data(),"_measured.png"));
    measured->Write();

    std::vector<TH1D*> measured_z=generateProjectionsZ(measured,outfilename,"measured_z");

    // TH1D* measured_jtpt=measured->ProjectionY("measured_jtpt",1,zbins_n,"eo");
    // DrawHist(measured_jtpt,ReplaceExtension(outfilename.data(),"_measured_jtpt2030.png"));

    TDecompSVD *svd= new TDecompSVD (response->Mresponse()); //this is the singular value decomposition (SVD) matrix
    auto singular_values = svd->GetSig(); //this is a vector with the singular values, i.e., the diagonal elements of S. They are ordered from largest to smallest.
    svd->Print();
    singular_values.Print();

     std::cout << "\nMatrix inversion method\n";
     auto inversion = Unfolding_MatrixInversion(measured,measured_z,outputDir+"/inversion",response,truth_2030);
    std::cout << "\nD'agostini method\n";
    auto agostini = Unfolding_Dagostini(measured,measured_z,outputDir+"/dagostini",response,truth_2030,iterations);

    std::vector<TH1*> preunfold_comparison{measured_z[1],recomc_measured_2030};

    yieldComparison(measured_z[1],recomc_measured_2030,outputDir+"/yield_comparison_preunfold.png");

    writeToCanvas(preunfold_comparison,"DATA vs MC reco","z^2","1/N dN/dz^2",outputDir+"/comparison_preunfold.png");

    std::vector<TH1*> comparison{ inversion.at("unfolded_z"),agostini.at("unfolded_z"),measured_z[1] };

    writeToCanvas(comparison,"unfolding comparison","z^2","N",outputDir+"/comparison.png");

    //clean up
    outputfile->Close();
    file->Close();
    response_file->Close();
    delete file;
    delete outputfile;
    delete response_file;
    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
}

THgroup Unfolding_MatrixInversion(TH2D* measured,const std::vector<TH1D*>& measured_z,const std::string& outputdir,RooUnfoldResponse* response, TH1D* truth_z)
{
    RooUnfoldInvert unfold(response,measured,"unfold");
    unfold.SetName("unfold_inv");

    TH2* unfolded = dynamic_cast<TH2*>(unfold.Hreco(RooUnfold::kCovariance));
    outputTH(unfolded,"unfolded_inv",outputdir);

    const std::string refolded_name="refolded_inv";
    TH2D* refolded = dynamic_cast<TH2D*>(response->ApplyToTruth(unfolded,refolded_name.data()));
    outputTH(refolded,refolded_name,outputdir);

    TH1D* out_th=nullptr;

    for(int j=0;j<jtptbins_n;j++)
    {
        const std::string postfix_jt=generatePostfix(j);

        const std::string obj_name="unfolded_z"+postfix_jt;
        TH1D* unfolded_z= unfolded->ProjectionX(obj_name.data(),1+j,1+j,"eo");
        outputTH(unfolded_z,obj_name,"unfolded by inversion",outputdir);

        const std::string refolded_name_z="refolded_z"+postfix_jt;
        TH1D* refolded_z = refolded->ProjectionX(refolded_name_z.data(),1+j,1+j,"eo");
        outputTH(refolded_z,refolded_name_z,"refolded by inversion",outputdir);

        TH1D* ratio_refold = new TH1D((*refolded_z)/(*measured_z[j]));
        ratio_refold->GetXaxis()->SetTitle("z^2");
        ratio_refold->GetYaxis()->SetTitle("ratio");
        outputTH(ratio_refold,"refolded_ratio_z"+postfix_jt,outputdir);

        std::vector<TH1*> measured_unfolded_comp{unfolded_z,measured_z[j]};
        writeToCanvas(measured_unfolded_comp,"measured vs unfolded","z^2","N",outputdir+"/measured_unfolded_z"+postfix_jt+".png");

        if(j==1) out_th=unfolded_z;
    }

    const std::string unfolded_jtpt_name="unfolded_jtpt";
    TH1D* unfolded_jtpt= unfolded->ProjectionY(unfolded_jtpt_name.data(),1,zbins_n,"eo");
    outputTH(unfolded_jtpt,unfolded_jtpt_name,"unfolded",outputdir);

    if(out_th!=nullptr);
    yieldComparison( out_th,truth_z,outputdir+"/yield_comparison.png");

    THgroup out;

    out["unfolded_z"]=out_th;
    
    return out;
}

THgroup Unfolding_Dagostini(TH2D* measured,const std::vector<TH1D*>& measured_z,const std::string& outputdir,RooUnfoldResponse* response, TH1D* truth_z, const std::vector<int>& iterations)
{
    std::vector<float> Xi2;
    std::vector<float> Xi2_log;
    std::vector<float> pvalue;
    std::vector<float> itF;
    int dof=0;

    std::vector<std::vector<TH1*>> measured_unfolded_z;
    measured_unfolded_z.resize(jtptbins_n);

    std::vector<std::vector<TH1*>> ratios_refolded_z;
    ratios_refolded_z.resize(jtptbins_n);

    TH1D* out_th=nullptr;

    for(int i=0;i < iterations.size();i++)
    {
        const int its=iterations[i];
        const std::string postfix=std::to_string(its);

        RooUnfold* unfold= new RooUnfoldBayes(response,measured,its,false,"unfold");
        unfold->SetName(("unfold"+postfix).data());

        TH2* unfolded = dynamic_cast<TH2*>(unfold->Hreco(RooUnfold::kCovariance));
        outputTH(unfolded,"unfolded_"+postfix,outputdir+"/unfolded");

        const std::string refolded_name="refolded_"+postfix;
        TH2D* refolded = dynamic_cast<TH2D*>(response->ApplyToTruth(unfolded,refolded_name.data()));
        outputTH(refolded,refolded_name,outputdir+"/refolded");

        Double_t chi2;
        Int_t ndf;
        Int_t igood;
        float p= refolded->Chi2TestX(measured,chi2,ndf,igood,"UUP");
        Xi2.push_back(chi2);
        Xi2_log.push_back(log10(chi2));
        pvalue.push_back(p);
        itF.push_back( iterations[i] );
        dof=ndf;

        for(int j=0;j<jtptbins_n;j++)
        {
            const std::string postfix_jt=generatePostfix(j)+'_'+postfix;

            const std::string obj_name="unfolded_z"+postfix_jt;
            TH1D* unfolded_z= unfolded->ProjectionX(obj_name.data(),1+j,1+j,"eo");
            outputTH(unfolded_z,obj_name,postfix+" iterations",outputdir+"/unfolded");

             if(!(i % it_draw_skip))  measured_unfolded_z[j].push_back(unfolded_z);

            const std::string refolded_name="refolded_z"+postfix_jt;
            TH1D* refolded_z = refolded->ProjectionX(refolded_name.data(),1+j,1+j,"eo");
            outputTH(refolded_z,refolded_name,postfix+" iterations",outputdir+"/refolded");

            TH1D* ratio_refold = new TH1D((*refolded_z)/(*measured_z[j]));
            ratio_refold->GetXaxis()->SetTitle("z^2");
            ratio_refold->GetYaxis()->SetTitle("ratio");
            outputTH(ratio_refold,"refolded_ratio_z"+postfix_jt,postfix+" iterations",outputdir+"/refolded");
             if(!(i % it_draw_skip))  ratios_refolded_z[j].push_back(ratio_refold);

            std::vector<TH1*> measured_unfolded_comp{unfolded_z,measured_z[j]};
            writeToCanvas(measured_unfolded_comp,"measured vs unfolded","z^2","N",outputdir+"/unfolded/measured_unfolded_z"+postfix_jt+".png");

            if((j==1) && ( p > 0.5f ) && (!out_th) )
            {
                out_th=unfolded_z;
            }
        }
    }

    for(int i=0;i< jtptbins_n;i++)
    {
        const std::string postfix=generatePostfix(i);
        const std::string outname_r=outputdir+"/ratios_z"+postfix+".png";
        writeToCanvas(ratios_refolded_z[i],"data refolded/measured ratio "+generateJtptString(i),"z^2","ratio",outname_r,false,0.65f,1.30f);

        const std::string outname_m=outputdir+"/unfolded_z"+postfix+".png";
        measured_unfolded_z[i].push_back(measured_z[i]);
        writeToCanvas(measured_unfolded_z[i],"data unfolded "+generateJtptString(i),"z^2","Events",outname_m);
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

    if(out_th!=nullptr);
    yieldComparison(out_th,truth_z,outputdir+"/yield_comparison.png");

    THgroup out;
    out["unfolded_z"]=out_th;

    return out;
}

void yieldComparison(TH1D* data_z, TH1D* mc_z, const std::string& outputdir)
{
    TH1D unfolded_z_norm= *data_z;
    unfolded_z_norm.Scale( 1.0f/ unfolded_z_norm.Integral() );
    unfolded_z_norm.SetTitle("DATA");

    TH1D truth_2030_norm= *mc_z;
    truth_2030_norm.Scale(1.0f/ truth_2030_norm.Integral());
    truth_2030_norm.SetTitle("PYTHIA8");

    std::vector<TH1*> yields_comp{ &unfolded_z_norm, &truth_2030_norm};

    writeToCanvas(yields_comp,"yields comparison","z^2","1/N dN/dz^2",outputdir);

}