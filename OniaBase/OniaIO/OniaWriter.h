#ifndef ONIAWRITER
#define ONIAWRITER

#include"../../TreeProcessor/TreeWriter.h"
#include"../JEC/JetCorrector.h"
#include"../JEC/JetUncertainty.h"
#include"OniaReader.h"
#include"OniaDataSimple.h"
#include"TLorentzVector.h"

double jeuCorr (double jtPt, double z, double jeu);
double jecCorr(double jtPt, double rawPt, double jpsiPt);
double zjecCorr(double jtPt, double rawPt, double z);
float getCorrectedPt(JetCorrector* JEC,const OniaJetInfo* inputJet, int iJet);
float zTolerance(float z);

template<typename Reader>
class OniaWriter
{
    public:
    virtual void writeQQ(const Reader* dataIn, int index, int entry) = 0;
    virtual void Write() = 0;
    ~OniaWriter() = default;
};

template<typename Reader>
class OniaWriterReco :public OniaWriter<Reader>
{
    TreeWriter writer;
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ oniaQQOut;
    OniaSimpleMu oniaMuOut;

    public:
    OniaWriterReco(const char* treeName):
        writer(treeName)
    {
        oniaInfoOut.addOutputs(&writer);
        oniaQQOut.addOutputs(&writer);
        oniaMuOut.addOutputs(&writer);
    }
    void writeQQ(const Reader* dataIn, int index, int entry) override
    {
        int pdgId=0;
        oniaInfoOut.Write(entry,pdgId);
        oniaQQOut.Write((TLorentzVector*) dataIn->recoQQ.mom4->At(index));
        int mumi_idx= dataIn->recoQQ.mumi_idx[index];
        int mupl_idx= dataIn->recoQQ.mupl_idx[index];
        TLorentzVector* mumi = (TLorentzVector*) dataIn->recoMu.mom4->At(mumi_idx);
        TLorentzVector* mupl = (TLorentzVector*) dataIn->recoMu.mom4->At(mupl_idx);
        oniaMuOut.Write(mupl,mumi);
        writer.FillEntries(); 
    }
    void Write() override 
    {
        writer.Write();
    }
};

template<typename Reader>
class OniaWriterGen :public OniaWriter<Reader>
{
    TreeWriter writer;
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ oniaQQOut;
    OniaSimpleMu oniaMuOut;

    public:
    OniaWriterGen(const char* treeName):
        writer(treeName)
    {
        oniaInfoOut.addOutputs(&writer);
        oniaQQOut.addOutputs(&writer);
        oniaMuOut.addOutputs(&writer);
    }
    void writeQQ(const Reader* dataIn, int index, int entry) override
    {
        oniaInfoOut.Write(entry,dataIn->genQQ.id[index]);
        oniaQQOut.Write((TLorentzVector*) dataIn->genQQ.mom4->At(index));
        int mumi_idx= dataIn->genQQ.mumi_idx[index];
        int mupl_idx= dataIn->genQQ.mupl_idx[index];
        TLorentzVector* mumi = (TLorentzVector*) dataIn->genMu.mom4->At(mumi_idx);
        TLorentzVector* mupl = (TLorentzVector*) dataIn->genMu.mom4->At(mupl_idx);
        oniaMuOut.Write(mupl,mumi);
        writer.FillEntries(); 
    }
    void Write() override 
    {
        writer.Write();
    }
};

//template<typename Reader>
class OniaWriterJet
{
    using Reader=OniaJetReaderMC;
    TreeWriter writer;
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ recoQQOut;
    OniaSimpleQQ genQQOut;
    OniaSimpleMu oniaMuOut;
    OniaSimpleJet jetOut;
    OniaSimpleRefJet refJetOut;

    void writeJet(const Reader* input, int index, int iJet, JetCorrector* JEC, JetUncertainty* JEU)
    {
        const OniaJetInfo* inputJet =  &(input->jetInfo);
        const TLorentzVector* recoQQmom4 =(TLorentzVector*) input->recoQQ.mom4->At(index);
        float recoQQpt=recoQQmom4->Pt();

        float jt_pt_noZJEC = getCorrectedPt(JEC,inputJet,iJet);
        float jt_pt=jecCorr(jt_pt_noZJEC, inputJet->rawpt[iJet],recoQQpt);
        float z = zTolerance(recoQQpt/jt_pt);

        jetOut.jt_eta = inputJet->eta[iJet];
        jetOut.jt_phi = inputJet->phi[iJet];
        jetOut.jt_rap = inputJet->y[iJet];
        jetOut.jt_pt_noZJEC = jt_pt_noZJEC;
        jetOut.jt_pt = jt_pt;
        jetOut.z = z;

        jetOut.jt_pt_jettyCorr = recoQQpt;
        if ( inputJet->rawpt[iJet] > recoQQpt ) 
        {
            JEC->SetJetPT(inputJet->rawpt[iJet]-recoQQpt); 
            jetOut.jt_pt_jettyCorr = recoQQpt+JEC->GetCorrectedPT();
        }
        jetOut.z_jettyCorr = recoQQpt/jetOut.jt_pt_jettyCorr;

        JEU->SetJetPT(jt_pt_noZJEC);
        JEU->SetJetEta(inputJet->eta[iJet]);
        JEU->SetJetPhi(inputJet->phi[iJet]);
	    jetOut.jt_pt_JEU_Down = jeuCorr(jt_pt, z, -1*(JEU->GetUncertainty().first));
	    jetOut.jt_pt_JEU_Up = jeuCorr(jt_pt, z, JEU->GetUncertainty().second);

    }

    void writeRefJet(const Reader* input, int index, int iJet, float jt_pt_noZJEC)
    {
        const OniaJetInfo* inputJet =  &(input->jetInfo);
        const TLorentzVector* genQQmom4 = (TLorentzVector*) input->genQQ.mom4->At(input->which.RecoQQWhichGen[index]);
        refJetOut.jt_ref_eta = inputJet->refeta[iJet];
        refJetOut.jt_ref_phi = inputJet->refphi[iJet];
        refJetOut.jt_ref_rap = inputJet->refy[iJet];
        refJetOut.jt_ref_pt = inputJet->refpt[iJet];
        refJetOut.gen_z = genQQmom4->Pt()/refJetOut.jt_ref_pt;
	    if (refJetOut.gen_z >= 1 && refJetOut.gen_z <= 1.0001) refJetOut.gen_z = 0.9999;
	    refJetOut.jt_pt_genZJEC = zjecCorr(jt_pt_noZJEC, inputJet->rawpt[iJet], refJetOut.gen_z);
    }

    void writeQQ(const Reader* dataIn, int iQQ,int entry)
    {
        TLorentzVector *RecoQQ4mom = (TLorentzVector*) dataIn->recoQQ.mom4->At(iQQ);
        TLorentzVector *GenQQ4mom = (TLorentzVector*) dataIn->genQQ.mom4->At(dataIn->which.RecoQQWhichGen[iQQ]);
        oniaInfoOut.Write(entry,dataIn->genQQ.id[iQQ]);
        recoQQOut.Write(RecoQQ4mom);
        genQQOut.Write(GenQQ4mom);
    }

    void writeMu(const Reader* dataIn, int iQQ)
    {
        int mumi_idx= dataIn->recoQQ.mumi_idx[iQQ];
        int mupl_idx= dataIn->recoQQ.mupl_idx[iQQ];
        TLorentzVector* mumi = (TLorentzVector*) dataIn->recoMu.mom4->At(mumi_idx);
        TLorentzVector* mupl = (TLorentzVector*) dataIn->recoMu.mom4->At(mupl_idx);
        oniaMuOut.Write(mupl,mumi);
    }

    public:

    OniaWriterJet(const char* treeName):
        writer(treeName)
    {
        oniaInfoOut.addOutputs(&writer);
        recoQQOut.addOutputs(&writer,"reco_");
        genQQOut.addOutputs(&writer,"gen_");
        oniaMuOut.addOutputs(&writer, "reco_");
        jetOut.addOutputs(&writer);
        refJetOut.addOutputs(&writer);
    }

    void write(const Reader* dataIn, int iQQ, int iJet, int entry, JetCorrector* JEC, JetUncertainty* JEU)
    {
        writeQQ(dataIn,iQQ,entry);
        writeMu(dataIn,iQQ);
        writeJet(dataIn,iQQ,iJet,JEC,JEU);
        writeRefJet(dataIn,iQQ,iJet,jetOut.jt_pt_noZJEC);
        
        writer.FillEntries(); 
    }
    void Write()
    {
        writer.Write();
    }
};

template<>
void OniaWriterGen<OniaReaderGenOnly>::writeQQ(const OniaReaderGenOnly* dataIn, int index, int entry);

using OniaWriterGenOnly = OniaWriterGen<OniaReaderGenOnly>;

#endif