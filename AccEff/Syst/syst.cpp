

#include"../../Utils/Helpers/Helpers.h"
#include "../../Drawing/Drawing.h"

#include<iostream>

#include"TEfficiency.h"
#include"TH1.h"
#include<array>
#include<string>
#include "RooUnfold.h"
#include "../Unfolding/Helpers.h"
#include"TRandom3.h"
#include"TSystem.h"

using THgroup = std::map<std::string,TH1*>;

constexpr std::array<double,6> EtaBins { 0.0f,0.25f,0.5f,0.75f,1.0f};

THgroup Unfolding_Dagostini_forsystematics(TH2D* measured,const std::vector<TH1D*>& measured_z,const std::string& outputdir,RooUnfoldResponse* response, TH1D* truth_z, const std::vector<int>& iterations);

void systgraph(const char* outFilename)
{
    std::cout << "\nSys graph\n";

    std::array<float,4> range_var={2.7496f, 0.57079, 0.42685f, 0.66895f };
    std::array<float,4> param_var={3.6876f,3.2417f,2.8103f,2.3679f};
    std::array<float,4> bkg_var={0.09f,2.78f,0.002f,0.001f};
    std::array<float,4> eff_var={0.75240f,0.79259f,4.4621f,4.3036f};
    std::array<float,4> accmc_var={1.81f,2.74f,2.01f,1.25f};
    std::array<float,4> effmc_var={1.4f,2.71f,1.71f,0.96f};
    std::array<float,4> jet_var={16.31f,6.85f,3.76f,0.96f};
    std::array<float,4> sigshape_var={0.88f,1.25f,0.59f,1.07f};

    const char* names[8]={"mass range","signal shape","params","bkg shape","eff syst","acc mc stat","eff mc stat","jet var"};

    std::vector<TH1*> hists;
    int i=0;
    for(auto& arr : {range_var,sigshape_var, param_var,bkg_var,eff_var,accmc_var,effmc_var,jet_var})
    {
        TH1D* th1=new TH1D(names[i],names[i],4,0.0,1.0);
        th1->SetStats(false);
        th1->Sumw2();
        for(int i=0;i<4;i++)
        {
            th1->SetBinContent(i+1, arr[i]);
        }
        hists.push_back(th1);
        i++;
    }

    writeToCanvas(hists,"systematic uncertainties overview","z^2","% var",outFilename );
}

void unfolding_create_systematic_vars(const char* filename,const char* outputfilename)
{
    //response file
    std::cout << "Reading input response file: " << filename <<'\n';
    TFile* response_file = OpenFile(filename,"READ");

    const std::string outfilename= std::string(outputfilename)+"/systematics/syst.root";

    RooUnfoldResponse* response= Get<RooUnfoldResponse>(response_file,"response");

    TH2D* measured_train = Get<TH2D>(response_file,"measured_train");
    TH2D* truth_train = Get<TH2D>(response_file,"truth_train");

    std::string yaggfilename= std::string(outputfilename)+"/yagg.root";
    TFile* yagg_file = OpenFile( yaggfilename.data(),"READ");

    //output file
    const std::string outputDir= RemoveFilename(outfilename);
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    TH2D* yagg = Get<TH2D>(yagg_file,"yields1S");
    yagg->SetTitle("Measured");
    TH2D* measured=yagg;

    std::vector<TH1D*> measured_z=generateProjectionsZ(measured,outfilename+"/systematics","measured_z");

    TH1D* truth_2030 = Get<TH1D>(response_file,"truth_test_z_jtpt2030");
    TH1D* recomc_measured_2030 = Get<TH1D>(response_file,"measured_test_z_jtpt2030");

    TH2* response_th = response->Hresponse();

    const int toynum=500;

    TRandom3 * rand = new TRandom3();
    rand->SetSeed(0);

    double systematics_vals[toynum*4];

    int binxnum= response_th->GetNbinsX();
    int binynum = response_th->GetNbinsY();

    int correctsysts=0;

    for(int i=0;i<toynum;i++)
    {
        std::cout << "Starting unfolding systematic variation "<< i << "\n";
        std::string rname= "response_"+std::to_string(i);
        //TH2* response_th_toy=response_th;
        TH2D* response_th_toy= new TH2D(rname.data(),rname.data(),binxnum , 0, 12, binynum,0, 12);
        for(int j=0;j<binxnum+2;j++)
        {
            for(int k=0;k<binynum+2;k++)
            {
                float val =response_th->GetBinContent(j,k);
                float err = response_th->GetBinError(j,k);

                float smearedVal = rand->Gaus(val,err);

                response_th_toy->SetBinContent(j,k,smearedVal);
                response_th_toy->SetBinError(j,k, err);
            }
        }

        const std::string respname="response_toy_"+std::to_string(i);

        response_th_toy->Write();

        RooUnfoldResponse* response_toy= new RooUnfoldResponse(measured_train, truth_train, response_th_toy,respname.data());

        const std::string dirname_out=outputDir+"/unfold_"+std::to_string(i);
        const std::string dirname_out2=outputDir+"/unfold_"+std::to_string(i)+"/refolded";
        const std::string dirname_out3=outputDir+"/unfold_"+std::to_string(i)+"/unfolded";

        gSystem->mkdir(dirname_out.data());
        gSystem->mkdir(dirname_out2.data());
        gSystem->mkdir(dirname_out3.data());

        auto agostini = Unfolding_Dagostini_forsystematics(measured,measured_z,dirname_out,response_toy,truth_2030,iterations);
        TH1* unfolded_res=agostini["unfolded_z"];

        if(unfolded_res!=NULL)
        {
            double vals[4];
            for(int j=0;j<4;j++)
            {
                vals[j] = unfolded_res->GetBinContent(j+1);
            }
            if(vals[0]>1.0 && vals[1]>1.0 &&vals[2]>1.0 &&vals[3]>1.0)
            {
                for(int j=0;j<4;j++)
                {
                    systematics_vals[correctsysts*4+j] = vals[j];
                }
                correctsysts++;
            }
            
        } 
    }

    double averages[4];
    for(int i=0;i<4;i++)
    {
        averages[i]=0.0f;
        for(int j=0;j<correctsysts;j++)
        {
            averages[i]+= systematics_vals[j*4+i];
        }
        averages[i]/=correctsysts;
    }

    double std_dev[4];
    for(int i=0;i<4;i++)
    {
        std_dev[i]=0.0;
        for(int j=0;j<correctsysts;j++)
        {
            float x=systematics_vals[j*4+i];
            std_dev[i]+= (x-averages[i])*(x-averages[i]);
        }
        std_dev[i]/=correctsysts;
        std_dev[i]=sqrt(std_dev[i]);
        std::cout << "average bin" << i << " : " << averages[i] << "\n";
        std::cout << "std dev bin" << i << " : " << std_dev[i] << "\n";
        std::cout << "error rel" << i << " : " << std_dev[i]/averages[i] << "\n";
    }

    std::cout << "values "<< correctsysts << "\n";
    
    for(int i=0;i< correctsysts;i++)
    {
        std::cout << i << ": ";
        for(int j=0;j<4;j++)
        {
            std::cout <<systematics_vals[i*4+j] << " ";
        }
        std::cout << "\n";
    }

    TH1D* yield_dist[4];
    yield_dist[0]= new TH1D("yield_dist0","Upsilon 1S yield variation distribution 0 < z^2 < 0.25",40, -50.0f,50.0f);
    yield_dist[1]= new TH1D("yield_dist1","Upsilon 1S yield variation distribution 0.25 < z^2 < 0.50",40, -50.0f,50.0f);
    yield_dist[2]= new TH1D("yield_dist2","Upsilon 1S yield variation distribution 0.50 < z^2 < 0.75",40, -50.0f,50.0f);
    yield_dist[3]= new TH1D("yield_dist3","Upsilon 1S yield variation distribution 0.75 < z^2 < 1.00",40, -50.0f,50.0f);
    yield_dist[0]->SetStats(false);
    yield_dist[1]->SetStats(false);
    yield_dist[2]->SetStats(false);
    yield_dist[3]->SetStats(false);
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<correctsysts;j++)
        {
            float x=systematics_vals[j*4+i];
            float val=100.0f*(x-averages[i])/averages[i];
            yield_dist[i]->Fill(val);
        }
        yield_dist[i]->Scale( 1.0f/yield_dist[i]->Integral() );
    }
    writeToCanvas(yield_dist[0], "% var","N",std::string(outputfilename)+"/systematics/yield_dist0.png");
    writeToCanvas(yield_dist[1], "% var","N",std::string(outputfilename)+"/systematics/yield_dist1.png");
    writeToCanvas(yield_dist[2], "% var","N",std::string(outputfilename)+"/systematics/yield_dist2.png");
    writeToCanvas(yield_dist[3], "% var","N",std::string(outputfilename)+"/systematics/yield_dist3.png");


    outputfile->Close();
    yagg_file->Close();
    response_file->Close();
}