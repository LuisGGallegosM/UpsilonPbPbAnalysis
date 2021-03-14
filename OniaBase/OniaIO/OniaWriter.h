#ifndef ONIAWRITER
#define ONIAWRITER

#include"../../TreeProcessor/TreeProcessor.h"
#include"../JEC/JetCorrector.h"
#include"../JEC/JetUncertainty.h"
#include"OniaReader.h"
#include"OniaDataSimple.h"
#include"TLorentzVector.h"
#include"TH2F.h"

struct OniaQQ;
struct OniaJetQQ;

double jeuCorr (double jtPt, double z, double jeu);
double jecCorr(double jtPt, double rawPt, double jpsiPt);
double zjecCorr(double jtPt, double rawPt, double z);
float getCorrectedPt(JetCorrector* JEC,const OniaJetInfo* inputJet, int iJet);
float zTolerance(float z);
float getJettyCorr(JetCorrector* JEC,const OniaJetInfo* inputJet, int iJet,float recoQQpt);
TH2F* createTH2Z(const std::string& name,const std::string& title);
void writeToCanvasZ(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname, const std::string& option);

void addOutputs(OniaQQ* data,TreeWriter* writer);
void addOutputs(OniaJetQQ* data,TreeWriter* writer);

struct OniaQQ
{
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ oniaQQOut;
    OniaSimpleMu oniaMuOut;
};

struct OniaJetQQ
{
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ genQQOut;
    OniaSimpleMu oniaMuOut;
    OniaSimpleJet jetOut;
    OniaSimpleRefJet refJetOut;
    OniaSimpleExtraQQ recoQQOut;
};

class OniaWriterRecoQQ : public DataWriter
{
    OniaQQ output;
    public:

    void registerWriter(TreeWriter* writer) override
    {
        addOutputs(&output,writer);
    }

    template<typename Reader>
    void writeData(const Reader* input, int index, int entry)
    {
        output.oniaInfoOut.Write(entry,0.0f);
        output.oniaQQOut.Write((TLorentzVector*) input->recoQQ.mom4->At(index));
        int mumi_idx= input->recoQQ.mumi_idx[index];
        int mupl_idx= input->recoQQ.mupl_idx[index];
        TLorentzVector* mumi = (TLorentzVector*) input->recoMu.mom4->At(mumi_idx);
        TLorentzVector* mupl = (TLorentzVector*) input->recoMu.mom4->At(mupl_idx);
        output.oniaMuOut.Write(mupl,mumi);
    }
};

class OniaWriterGenQQ : public DataWriter
{
    OniaQQ output;
    public:

    void registerWriter(TreeWriter* writer) override
    {
        addOutputs(&output,writer);
    }

    template<typename Reader>
    void writeData(const Reader* input, int index, int entry)
    {
        output.oniaInfoOut.Write(entry,input->genQQ.id[index]);
        output.oniaQQOut.Write((TLorentzVector*) input->genQQ.mom4->At(index));
        int mumi_idx= input->genQQ.mumi_idx[index];
        int mupl_idx= input->genQQ.mupl_idx[index];
        TLorentzVector* mumi = (TLorentzVector*) input->genMu.mom4->At(mumi_idx);
        TLorentzVector* mupl = (TLorentzVector*) input->genMu.mom4->At(mupl_idx);
        output.oniaMuOut.Write(mupl,mumi);
    }

    void writeData(const OniaGenOnlyData* input, int index, int entry);
};

class OniaWriterJetQQ : public DataWriter
{
    OniaJetQQ output;
    public:

    void registerWriter(TreeWriter* writer) override
    {
        addOutputs(&output,writer);
    }

    template<typename Reader>
    void writeData(const Reader* input, int iQQ, int iJet, int entry, JetCorrector* JEC, JetUncertainty* JEU)
    {
        writeQQ(input,&output,iQQ,entry);
        writeMu(input,&output,iQQ);
        writeJet(input,&output,iQQ,iJet,JEC,JEU);
        writeRefJet(input,&output,iQQ,iJet,output.jetOut.jt_pt_noZJEC);
    }
};

template<typename Reader>
void writeQQ(const Reader* input, OniaJetQQ* output, int iQQ,int entry)
{
    TLorentzVector *RecoQQ4mom = (TLorentzVector*) input->recoQQ.mom4->At(iQQ);
    TLorentzVector *GenQQ4mom = (TLorentzVector*) input->genQQ.mom4->At(input->which.RecoQQWhichGen[iQQ]);
    output->oniaInfoOut.Write(entry,input->genQQ.id[iQQ]);
    output->recoQQOut.Write(RecoQQ4mom,input->recoQQ.ctau[iQQ]);
    output->genQQOut.Write(GenQQ4mom);
}

template<typename Reader>
void writeMu(const Reader* dataIn, OniaJetQQ* output, int iQQ)
{
    int mumi_idx= dataIn->recoQQ.mumi_idx[iQQ];
    int mupl_idx= dataIn->recoQQ.mupl_idx[iQQ];
    TLorentzVector* mumi = (TLorentzVector*) dataIn->recoMu.mom4->At(mumi_idx);
    TLorentzVector* mupl = (TLorentzVector*) dataIn->recoMu.mom4->At(mupl_idx);
    output->oniaMuOut.Write(mupl,mumi);
}

template<typename Reader>
void writeJet(const Reader* input,OniaJetQQ* output, int index, int iJet, JetCorrector* JEC, JetUncertainty* JEU)
{
    const OniaJetInfo* inputJet =  &(input->jetInfo);
    const TLorentzVector* recoQQmom4 =(TLorentzVector*) input->recoQQ.mom4->At(index);
    float recoQQpt=recoQQmom4->Pt();

    float jt_pt_noZJEC = getCorrectedPt(JEC,inputJet,iJet);
    float jt_pt=jecCorr(jt_pt_noZJEC, inputJet->rawpt[iJet],recoQQpt);
    float z = zTolerance(recoQQpt/jt_pt);
    float jt_jettyCorr = getJettyCorr(JEC,inputJet,iJet,recoQQpt);

    output->jetOut.jt_eta = inputJet->eta[iJet];
    output->jetOut.jt_phi = inputJet->phi[iJet];
    output->jetOut.jt_rap = inputJet->y[iJet];
    output->jetOut.jt_pt_noZJEC = jt_pt_noZJEC;
    output->jetOut.jt_pt = jt_pt;
    output->jetOut.z = z;

    output->jetOut.jt_pt_jettyCorr = jt_jettyCorr;
    output->jetOut.z_jettyCorr = recoQQpt/jt_jettyCorr;

    JEU->SetJetPT(jt_pt_noZJEC);
    JEU->SetJetEta(inputJet->eta[iJet]);
    JEU->SetJetPhi(inputJet->phi[iJet]);
    output->jetOut.jt_pt_JEU_Down = jeuCorr(jt_pt, z, -1*(JEU->GetUncertainty().first));
    output->jetOut.jt_pt_JEU_Up = jeuCorr(jt_pt, z, JEU->GetUncertainty().second);
}

template<typename Reader>
void writeRefJet(const Reader* input,OniaJetQQ* output, int index, int iJet, float jt_pt_noZJEC)
{
    const OniaJetRef* inputJet =  &(input->jetRef);
    const TLorentzVector* genQQmom4 = (TLorentzVector*) input->genQQ.mom4->At(input->which.RecoQQWhichGen[index]);
    float gen_z=zTolerance(genQQmom4->Pt()/inputJet->refpt[iJet]);
    output->refJetOut.jt_ref_eta = inputJet->refeta[iJet];
    output->refJetOut.jt_ref_phi = inputJet->refphi[iJet];
    output->refJetOut.jt_ref_rap = inputJet->refy[iJet];
    output->refJetOut.jt_ref_pt = inputJet->refpt[iJet];
    output->refJetOut.gen_z = gen_z;
    output->refJetOut.jt_pt_genZJEC = zjecCorr(jt_pt_noZJEC, input->jetInfo.rawpt[iJet], gen_z);
}

#endif