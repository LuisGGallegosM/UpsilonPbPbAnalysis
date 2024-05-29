#include"../../Utils/Helpers/Helpers.h"
#include "../../Drawing/Drawing.h"

#include<iostream>

#include"TEfficiency.h"
#include"TH1.h"
#include<array>
#include<string>

#include "../../OniaBase/TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../../OniaBase/Params/Params.h"
#include "../Common/AccEffAux.h"

class JECAnalyzer
{
    private:
    OniaReader<OniaJetQQMC> oniaReader;

    public:
    TH1D* jtpt_comp[2];
    float* pt_ratio[2];
    int* ptn[2];
    TH1D* jt_ratio_dist[2];

    JECAnalyzer(TTree* input): oniaReader(input)
    {
        jtpt_comp[0]= new TH1D("jtpt_resp_nozjec","jet response no z-corr", 11,0.0,1.1);
        jtpt_comp[1]= new TH1D("jtpt_resp","jet response z-corr", 11,0.0,1.1);
        jtpt_comp[0]->SetStats(false);
        jtpt_comp[1]->SetStats(false);
        jt_ratio_dist[0]= new TH1D("jtpt_r_dist0","jtpt_reco/ jtpt_gen distribution  z^2 < 0.5", 20,0.5,1.5);
        jt_ratio_dist[1]= new TH1D("jtpt_r_dist1","jtpt_reco/ jtpt_gen distribution  z^2 > 0.5", 20,0.5,1.5);
        jt_ratio_dist[0]->SetStats(false);
        jt_ratio_dist[1]->SetStats(false);
        for(int i=0;i<2;i++)
        {
            pt_ratio[i]= new float[20];
            ptn[i]= new int[20];
        }
        
        for(int i=0;i<20;i++)
        {
            for(int j=0;j<2;j++)
            {
                (pt_ratio[j])[i]=0.0f;
                (ptn[j])[i]=0;
            }
        }
    }

    void Test() 
    { 
        TreeProcess(this,oniaReader.getName(),oniaReader.getEntries());
        int bnum=jtpt_comp[0]->GetNbinsX();
        for(int i=0;i<bnum;i++)
        {
            if(ptn[i]!=0)
            {
                double recopt_mean_nozjec=(pt_ratio[0])[i]/(ptn[0])[i];
                double recopt_mean=(pt_ratio[1])[i]/(ptn[1])[i];
                jtpt_comp[0]->SetBinContent(i, recopt_mean_nozjec);
                jtpt_comp[1]->SetBinContent(i, recopt_mean);
            } else
            {
                jtpt_comp[0]->SetBinContent(i, 0.0);
                jtpt_comp[1]->SetBinContent(i, 0.0);
            }
        }
        jt_ratio_dist[0]->Scale(1.0f/jt_ratio_dist[0]->Integral());
        jt_ratio_dist[1]->Scale(1.0f/jt_ratio_dist[1]->Integral());
    }
    void ProcessEvent(Long64_t entry);
};

class JECAnalyzer2
{
    private:
    OniaReader<OniaJetQQRealData> oniaReader;

    public:
    TH1D* jtpt_jecs[4];

    JECAnalyzer2(TTree* input): oniaReader(input)
    {
        jtpt_jecs[0] = new TH1D("jtpt_jec0","jec correction factor 0.00<z^2<0.25", 8,0.0,0.4);
        jtpt_jecs[1] = new TH1D("jtpt_jec1","jec correction factor 0.25<z^2<0.50", 8,0.0,0.4);
        jtpt_jecs[2] = new TH1D("jtpt_jec2","jec correction factor 0.50<z^2<0.75", 8,0.0,0.4);
        jtpt_jecs[3] = new TH1D("jtpt_jec3","jec correction factor 0.75<z^2<1.00", 8,0.0,0.4);

        jtpt_jecs[0]->SetStats(false);
        jtpt_jecs[1]->SetStats(false);
        jtpt_jecs[2]->SetStats(false);
        jtpt_jecs[3]->SetStats(false);
    }

    void Test() 
    { 
        TreeProcess(this,oniaReader.getName(),oniaReader.getEntries());
        jtpt_jecs[0]->Scale(1.0/ jtpt_jecs[0]->Integral()  );
        jtpt_jecs[1]->Scale(1.0/ jtpt_jecs[1]->Integral()  );
        jtpt_jecs[2]->Scale(1.0/ jtpt_jecs[2]->Integral()  );
        jtpt_jecs[3]->Scale(1.0/ jtpt_jecs[3]->Integral()  );
    }
    void ProcessEvent(Long64_t entry);
};

void jecan(const char* filename,const char* filename2)
{
    TFile* file = OpenFile(filename,"READ");

    TTree *myTree = GetTree(file,"onia_skimmed");

    TFile* file2 = OpenFile(filename2,"READ");

    TTree *myTree2 = GetTree(file2,"onia_skimmed");

    JECAnalyzer jecan(myTree);

    jecan.Test();

    JECAnalyzer2 jecan2(myTree2);
    jecan2.Test();

    const char* xlabel="factor";

    writeToCanvas(jecan.jtpt_comp[0],"gen z", "R", "out.png");
    writeToCanvas(jecan.jtpt_comp[1],"gen z", "R", "out2.png");
    writeToCanvas(jecan2.jtpt_jecs[0],xlabel, "N",  "outj0" );
    writeToCanvas(jecan2.jtpt_jecs[1],xlabel, "N",  "outj1" );
    writeToCanvas(jecan2.jtpt_jecs[2],xlabel, "N",  "outj2" );
    writeToCanvas(jecan2.jtpt_jecs[3],xlabel, "N",  "outj3" );

    writeToCanvas(jecan.jt_ratio_dist[0],"R", "N", "out_dist.png");
    writeToCanvas(jecan.jt_ratio_dist[1],"R", "N", "out2_dist.png");

    std::vector<TH1*> ths = { jecan.jtpt_comp[0], jecan.jtpt_comp[1]};

    writeToCanvas(ths,"jet response comparison", "gen z","R","out_c.pdf");

    std::vector<TH1*> ths2 = { jecan2.jtpt_jecs[0],jecan2.jtpt_jecs[1],jecan2.jtpt_jecs[2],jecan2.jtpt_jecs[3]};

    writeToCanvas(ths2,"jet correction factors", "<unc-up>","N","out_c2.pdf");

}

void JECAnalyzer::ProcessEvent(Long64_t entry)
{
    auto input= oniaReader.getData(entry);

    float gen_z=input->refJetOut.gen_z;
    float reco_jtpt_nzjec= input->jetOut.jt_pt_noZJEC;
    float reco_jtpt= input->jetOut.jt_pt;
    float gen_jtpt = input->refJetOut.jt_ref_pt;

    int bin =jtpt_comp[0]->FindBin(gen_z);

    float ratio_0=reco_jtpt_nzjec/gen_jtpt;
    float ratio_1=reco_jtpt/gen_jtpt;

    if(gen_z*gen_z < 0.5)
    {
        jt_ratio_dist[0]->Fill(ratio_1);
    } else
    {
        jt_ratio_dist[1]->Fill(ratio_1);
    }

    (pt_ratio[0])[bin]+=ratio_0;
    (ptn[0])[bin]++;
    (pt_ratio[1])[bin]+=ratio_1;
    (ptn[1])[bin]++;
}

void JECAnalyzer2::ProcessEvent(Long64_t entry)
{
    auto input= oniaReader.getData(entry);
    float val =input->jetOut.jec_corr-1.0f;

    float z2= input->jetOut.z*input->jetOut.z;
    if(z2 <0.25f) jtpt_jecs[0]->Fill(val);
    else if(z2<0.5f) jtpt_jecs[1]->Fill(val);
    else if(z2<0.75f) jtpt_jecs[2]->Fill(val);
    else jtpt_jecs[3]->Fill(val);
}