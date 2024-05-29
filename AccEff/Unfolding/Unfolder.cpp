
#include"Unfolder.h"

const int v1d_n= v1d_jtpt ? jtptbins_n : zbins_n;
const double v1d_low= v1d_jtpt ? jtptbins_low : zbins_low;
const double v1d_high= v1d_jtpt ? jtptbins_high : zbins_high;

int findRecoQQ(const OniaJetMCData* input,int iQQ)
    {
        for(int i=0;i<input->recoQQ.size;i++)
        {
            if(input->which.RecoQQWhichGen[i]==iQQ)
                return i;
        }
        return -1;
    }

template<>
Unfolder<TH2D>::Unfolder(TTree* tree, bool fPrior): 
        oniaReader(tree),
        response(&measured_train,&truth_train),
        measured_train("measured_train","measured_train",zbins_n,zbins_low,zbins_high,jtptbins_n,jtptbins_low,jtptbins_high),
        truth_train("truth_train","truth_train",zbins_n,zbins_low,zbins_high,jtptbins_n,jtptbins_low,jtptbins_high),
        measured_test("measured_test","measured_test",zbins_n,zbins_low,zbins_high,jtptbins_n,jtptbins_low,jtptbins_high),
        truth_test("truth_test","truth_test",zbins_n,zbins_low,zbins_high,jtptbins_n,jtptbins_low,jtptbins_high),
        JEC(LoadJECFiles(true)), JEU(LoadJEUFiles(true)),
        weighter(nullptr),
        flatPrior(fPrior),
        fillingWeights(false)
    {
    }

template<>
Unfolder<TH1D>::Unfolder(TTree* tree, bool fPrior): 
        oniaReader(tree),
        response(v1d_n,v1d_low,v1d_high),
        measured_train("measured_train","measured_train",v1d_n,v1d_low,v1d_high),
        truth_train("truth_train","truth_train",v1d_n,v1d_low,v1d_high),
        measured_test("measured_test","measured_test",v1d_n,v1d_low,v1d_high),
        truth_test("truth_test","truth_test",v1d_n,v1d_low,v1d_high),
        JEC(LoadJECFiles(true)), JEU(LoadJEUFiles(true)),
        weighter(nullptr),
        flatPrior(fPrior),
        fillingWeights(false)
    {
    }

template<typename T>
void Unfolder<T>::Train()
{
    if(flatPrior)
    {
        fillingWeights=true;
        TreeProcess(this,oniaReader.getName(),oniaReader.getEntries());
        fillingWeights=false;
        weighter = (T*) truth_test.Clone("weighter");
        weighter->Scale(1.0f/ weighter->Integral());
        TreeProcess(this,oniaReader.getName(),oniaReader.getEntries());
    } else
    {
        TreeProcess(this,oniaReader.getName(),oniaReader.getEntries());
    }
}

template<typename T>
void Unfolder<T>::ProcessEvent(Long64_t entry)
{
    auto input = oniaReader.getData(entry);
    int size=input->genQQ.size;

    bool isForTrain= ((entry % 2)==0);

    for(int index=0;index<size;index++)
    {
        TLorentzVector* mom4vec=(TLorentzVector*) input->genQQ.mom4->At(index);
        
        if ((abs(mom4vec->Rapidity()) <= yMax) && ( mom4vec->Pt() <= ptMax ))
        {
            if (accCutter.cut(input,index,entry))
            {
                int genJet= FindJetGen(input,index);

                if(genJet>=0)
                {
                    int recoIndex= findRecoQQ(input,index);
                    const TLorentzVector* GenQQ4mom= (TLorentzVector*) input->genQQ.mom4->At(index);
                    float jtpt_truth= input->jetRef.refpt[genJet];
                    float z_truth= zTolerance(GenQQ4mom->Pt()/jtpt_truth);

                    bool inRange= (z_truth>=zbins_low) && (z_truth <=zbins_high) && (jtpt_truth>=jtptbins_low) && (jtpt_truth <= jtptbins_high);

                    if((recoIndex>=0) && (inRange))
                    {
                        int iJet=FindJet(input,&JEC,recoIndex);
                        if(iJet>=0)
                        {
                            const TLorentzVector* RecoQQ4mom= (TLorentzVector*) input->recoQQ.mom4->At(recoIndex);
                            const float recoQQpt=RecoQQ4mom->Pt();
                            float jt_pt_noZJEC = getCorrectedPt(&JEC,&(input->jetInfo),iJet);
                            float jtpt_measured=jecCorr(jt_pt_noZJEC, input->jetInfo.rawpt[iJet],recoQQpt);
                            float z_measured=zTolerance(recoQQpt/jtpt_measured);

                            bool inRangeMeas= (z_measured>=zbins_low) && (z_measured <=zbins_high) && (jtpt_measured>=jtptbins_low) && (jtpt_measured <= jtptbins_high);
                            
                            if(inRangeMeas) Fill(z_measured,z_truth,jtpt_measured,jtpt_truth,isForTrain);
                        }
                    }

                }
                
            }
        }
    }
        
    }

template<>
void Unfolder<TH2D>::Fill(float z_measured, float z_truth, float jtpt_measured, float jtpt_truth, bool isForTrain)
{
    if(flatPrior)
    {
        if(isForTrain)
        {
            if( fillingWeights )
            {
                measured_train.Fill(z_measured*z_measured,jtpt_measured);
                truth_train.Fill(z_truth*z_truth,jtpt_truth);
            } else
            {
                float z2_truth=z_truth*z_truth;
                Int_t binn = weighter->FindBin(z2_truth,jtpt_truth);
                float weight = 1.0f/ weighter->GetBinContent(binn);
                response.Fill(z_measured*z_measured,jtpt_measured,z2_truth,jtpt_truth, weight);
            }
        }
        else
        {
            measured_test.Fill(z_measured*z_measured,jtpt_measured);
            truth_test.Fill(z_truth*z_truth,jtpt_truth);
        }
    } else
    {
        if(isForTrain)
        {
            response.Fill(z_measured*z_measured,jtpt_measured,z_truth*z_truth,jtpt_truth);
            measured_train.Fill(z_measured*z_measured,jtpt_measured);
            truth_train.Fill(z_truth*z_truth,jtpt_truth);
        } else
        {
            measured_test.Fill(z_measured*z_measured,jtpt_measured);
            truth_test.Fill(z_truth*z_truth,jtpt_truth);
        }
    }
}

template<>
void Unfolder<TH1D>::Fill(float z_measured, float z_truth, float jtpt_measured, float jtpt_truth, bool isForTrain)
{
    if(v1d_jtpt)
    {
        if(isForTrain)
        {
            response.Fill(jtpt_measured,jtpt_truth);
            measured_train.Fill(jtpt_measured);
            truth_train.Fill(jtpt_truth);
        } else
        {
            measured_test.Fill(jtpt_measured);
            truth_test.Fill(jtpt_truth);
        }
    } else
    {
        if(isForTrain)
        {
            response.Fill(z_measured*z_measured,z_truth*z_truth);
            measured_train.Fill(z_measured*z_measured);
            truth_train.Fill(z_truth*z_truth);
        } else
        {
            measured_test.Fill(z_measured*z_measured);
            truth_test.Fill(z_truth*z_truth);
        }
    }
    

}

template class Unfolder<TH2D>;
template class Unfolder<TH1D>;

THFiller_DATA::THFiller_DATA(TTree* tree): 
        oniaReader(tree),
        measured("measured","measured",zbins_n,zbins_low,zbins_high,jtptbins_n,jtptbins_low,jtptbins_high)
    {
    }

TH2D* THFiller_DATA::fill()
    {
        const int size=oniaReader.getEntries();

        for(int entry=0;entry<size;entry++)
        {
            auto input = oniaReader.getData(entry);
            const float z=input->jetOut.z;
            const float jt_pt= input->jetOut.jt_pt;

            measured.Fill(z*z,jt_pt);
        }

        return &measured;
    }

