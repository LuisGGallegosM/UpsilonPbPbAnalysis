#ifndef ONIAWRITER
#define ONIAWRITER

#include"../TreeProcessor/TreeProcessor.h"
#include"../JetCorrections/JetCorrections.h"
#include"OniaReader.h"
#include"OniaDataSimple.h"
#include"TLorentzVector.h"
#include"TH2F.h"

struct OniaQQ;
struct OniaQQW;
struct OniaJetQQMC;
struct OniaJetQQMCW;
struct OniaJetQQRealData;
struct OniaJetQQRealDataW;
struct OniaOnlyW;

double jeuCorr (double jtPt, double z, double jeu);
double jecCorr(double jtPt, double rawPt, double jpsiPt);
double zjecCorr(double jtPt, double rawPt, double z);
float getCorrectedPt(JetCorrector* JEC,const OniaJetInfo* inputJet, int iJet);
float zTolerance(float z);
float getJettyCorr(JetCorrector* JEC,const OniaJetInfo* inputJet, int iJet,float recoQQpt);
TH2F* createTH2Z(const std::string& name,const std::string& title);
void writeToCanvasZ(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname, const std::string& option);

void addOutputs(OniaQQ* data,TreeWriter* writer, const char* prefix);
void addOutputs(OniaQQW* data,TreeWriter* writer, const char* prefix);
void addOutputs(OniaJetQQMC* data,TreeWriter* writer, const char* prefix);
void addOutputs(OniaJetQQMCW* data,TreeWriter* writer, const char* prefix);
void addOutputs(OniaJetQQRealData* data,TreeWriter* writer, const char* prefix);
void addOutputs(OniaOnlyW* data,TreeWriter* writer, const char* prefix);
void addOutputs(OniaJetQQRealDataW* data,TreeWriter* writer, const char* prefix);

struct OniaQQ
{
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ oniaQQOut;
    OniaSimpleMu oniaMuOut;
};

struct OniaQQW
{
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ oniaQQOut;
    OniaSimpleMu oniaMuOut;
    OniaWeight weight;
};

struct OniaJetQQRealData
{
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleMu oniaMuOut;
    OniaSimpleJet jetOut;
    OniaSimpleExtraQQ recoQQOut;
};
struct OniaJetQQRealDataW
{
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleMu oniaMuOut;
    OniaSimpleJet jetOut;
    OniaSimpleExtraQQ recoQQOut;
    OniaWeight weight;
};

struct OniaOnlyW
{
    OniaWeight weight;
};

struct OniaJetQQMC
{
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ genQQOut;
    OniaSimpleMu oniaMuOut;
    OniaSimpleJet jetOut;
    OniaSimpleRefJet refJetOut;
    OniaSimpleExtraQQ recoQQOut;
};

struct OniaJetQQMCW
{
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ genQQOut;
    OniaSimpleMu oniaMuOut;
    OniaSimpleJet jetOut;
    OniaSimpleRefJet refJetOut;
    OniaSimpleExtraQQ recoQQOut;
    OniaWeight weight;
};

void addInputs(OniaQQ* data, TreeReader* reader);
void addInputs(OniaQQW* data, TreeReader* reader);
void addInputs(OniaJetQQMC* data, TreeReader* reader);
void addInputs(OniaJetQQRealData* data, TreeReader* reader);

struct SimpleSelector
{
    long long int entry;
    int index;
};

struct JetSelector
{
    long long int entry;
    int iQQ;
    int iJet;
    JetCorrector* JEC;
    JetUncertainty* JEU;
};

template<typename Data>
class OniaWriter
{
    Data output;
    TreeWriter writer;
    
    public:

    OniaWriter(const char* treeOutName, const char* prefix =nullptr ) : writer(treeOutName)
    {
        addOutputs(&output,&writer,prefix);
    }

    Data* getDataBuffer() {return &output;}

    void writeData() { writer.FillEntries(); }

    TTree* getTree() { return writer.getTree();}
};

template<typename Data>
void writeRecoQQ(const Data* input, OniaQQ* output, SimpleSelector sel)
{
    output->oniaInfoOut.Write(sel.entry,0.0f);
    output->oniaQQOut.Write((TLorentzVector*) input->recoQQ.mom4->At(sel.index));
    int mumi_idx= input->recoQQ.mumi_idx[sel.index];
    int mupl_idx= input->recoQQ.mupl_idx[sel.index];
    TLorentzVector* mumi = (TLorentzVector*) input->recoMu.mom4->At(mumi_idx);
    TLorentzVector* mupl = (TLorentzVector*) input->recoMu.mom4->At(mupl_idx);
    output->oniaMuOut.Write(mupl,mumi);
}

template<typename Data>
void writeGenQQ(const Data* input, OniaQQ* output, SimpleSelector sel)
{
    output->oniaInfoOut.Write(sel.entry,input->genQQ.id[sel.index]);
    output->oniaQQOut.Write((TLorentzVector*) input->genQQ.mom4->At(sel.index));
    int mumi_idx= input->genQQ.mumi_idx[sel.index];
    int mupl_idx= input->genQQ.mupl_idx[sel.index];
    TLorentzVector* mumi = (TLorentzVector*) input->genMu.mom4->At(mumi_idx);
    TLorentzVector* mupl = (TLorentzVector*) input->genMu.mom4->At(mupl_idx);
    output->oniaMuOut.Write(mupl,mumi);
}

template<typename Data>
void writeJet(const Data* input, OniaJetQQMC* output, JetSelector sel)
{
    writeQQ(input,output,sel.iQQ,sel.entry);
    writeMu(input,output,sel.iQQ);
    writeJet(input,output,sel.iQQ,sel.iJet,sel.JEC,sel.JEU);
    writeRefJet(input,output,sel.iQQ,sel.iJet,output->jetOut.jt_pt_noZJEC);
}

template<typename Data>
void writeJet(const Data* input, OniaJetQQRealData* output, JetSelector sel)
{
    writeQQ(input,output,sel.iQQ,sel.entry);
    writeMu(input,output,sel.iQQ);
    writeJet(input,output,sel.iQQ,sel.iJet,sel.JEC,sel.JEU);
}

template<typename Data>
void writeQQ(const Data* input, OniaJetQQMC* output, int iQQ,int entry)
{
    TLorentzVector *RecoQQ4mom = (TLorentzVector*) input->recoQQ.mom4->At(iQQ);
    TLorentzVector *GenQQ4mom = (TLorentzVector*) input->genQQ.mom4->At(input->which.RecoQQWhichGen[iQQ]);
    output->oniaInfoOut.Write(entry,input->genQQ.id[iQQ]);
    output->recoQQOut.Write(RecoQQ4mom,input->recoQQ.ctau[iQQ]);
    output->genQQOut.Write(GenQQ4mom);
}

template<typename Data>
void writeQQ(const Data* input, OniaJetQQRealData* output, int iQQ,int entry)
{
    TLorentzVector *RecoQQ4mom = (TLorentzVector*) input->recoQQ.mom4->At(iQQ);
    output->oniaInfoOut.Write(entry,0);
    output->recoQQOut.Write(RecoQQ4mom,input->recoQQ.ctau[iQQ]);
}

template<typename Data, typename Writer>
void writeMu(const Data* dataIn, Writer* output, int iQQ)
{
    int mumi_idx= dataIn->recoQQ.mumi_idx[iQQ];
    int mupl_idx= dataIn->recoQQ.mupl_idx[iQQ];
    TLorentzVector* mumi = (TLorentzVector*) dataIn->recoMu.mom4->At(mumi_idx);
    TLorentzVector* mupl = (TLorentzVector*) dataIn->recoMu.mom4->At(mupl_idx);
    output->oniaMuOut.Write(mupl,mumi);
}

template<typename Data, typename Writer>
void writeJet(const Data* input,Writer* output, int index, int iJet, JetCorrector* JEC, JetUncertainty* JEU)
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

    output->jetOut.jt_pt_JEU_Down = jeuCorr(1.0, z, -1*(JEU->GetUncertainty().first));
    output->jetOut.jt_pt_JEU_Up = jeuCorr(1.0, z, JEU->GetUncertainty().second);
    //output->jetOut.jec_corr = JEU->GetUncertainty().second;
    output->jetOut.jec_corr = (1.0f-z)*(1.0f+JEU->GetUncertainty().second)+z;

    // const int jeccor=0;
    // if(jeccor==1) output->jetOut.jt_pt= output->jetOut.jt_pt_JEU_Up;
    // else if(jeccor==-1) output->jetOut.jt_pt= output->jetOut.jt_pt_JEU_Down;
}

template<typename Data>
void writeRefJet(const Data* input,OniaJetQQMC* output, int index, int iJet, float jt_pt_noZJEC)
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
    output->refJetOut.pthat = inputJet->pthat;
}

#endif