#ifndef ONIAWRITER
#define ONIAWRITER

#include"TLorentzVector.h"
#include"../TreeProcessor/TreeWriter.h"
#include"OniaReader.h"
#include"OniaData/OniaDataSimple.h"
#include"JEC/JetCorrector.h"
#include"JEC/JetUncertainty.h"

double jeuCorr (double jtPt, double z, double jeu);
double jecCorr(double jtPt, double rawPt, double jpsiPt);
double zjecCorr(double jtPt, double rawPt, double z);

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
    OniaSimpleJet oniaJetOut;

    void writeJet(const Reader* input, int index, int iJet, JetCorrector* JEC, JetUncertainty* JEU)
    {
        const OniaJetInfo* inJet =  &(input->jetInfo);
        const TLorentzVector* recoQQmom4 =(TLorentzVector*) input->recoQQ.mom4->At(index);
        const TLorentzVector* genQQmom4 = (TLorentzVector*) input->genQQ.mom4->At(input->which.RecoQQWhichGen[index]);
        float pt=recoQQmom4->Pt();

        oniaJetOut.jt_eta = inJet->eta[iJet];
        oniaJetOut.jt_phi = inJet->phi[iJet];
        oniaJetOut.jt_rap = inJet->y[iJet];

        JEC->SetJetPT(input->jetInfo.rawpt[iJet]);
        JEC->SetJetEta(oniaJetOut.jt_eta);
        JEC->SetJetPhi(oniaJetOut.jt_phi);
        JEC->SetRho(1);
        JEC->SetJetArea(input->jetInfo.area[iJet]);
        float jt_pt_noZJEC = JEC->GetCorrectedPT();

        oniaJetOut.jt_pt = jecCorr(jt_pt_noZJEC, inJet->rawpt[iJet],pt);
	    oniaJetOut.z = recoQQmom4->Pt()/oniaJetOut.jt_pt;
	    if (oniaJetOut.z >= 1.0f && oniaJetOut.z <= 1.0001f) oniaJetOut.z = 0.9999f;

        JEU->SetJetPT(jt_pt_noZJEC);
        JEU->SetJetEta(oniaJetOut.jt_eta);
        JEU->SetJetPhi(oniaJetOut.jt_phi);

	    oniaJetOut.jt_pt_JEU_Down = jeuCorr(oniaJetOut.jt_pt, oniaJetOut.z, -1*(JEU->GetUncertainty().first));
	    oniaJetOut.jt_pt_JEU_Up = jeuCorr(oniaJetOut.jt_pt, oniaJetOut.z, JEU->GetUncertainty().second);

        oniaJetOut.jt_ref_eta = inJet->refeta[iJet];
        oniaJetOut.jt_ref_phi = inJet->refphi[iJet];
        oniaJetOut.jt_ref_rap = inJet->refy[iJet];
        oniaJetOut.jt_ref_pt = inJet->refpt[iJet];

        oniaJetOut.jt_pt_jettyCorr = pt;
        if ( inJet->rawpt[iJet] > pt ) 
        {
            JEC->SetJetPT(inJet->rawpt[iJet]-pt); 
            oniaJetOut.jt_pt_jettyCorr = pt+JEC->GetCorrectedPT();
        }
        oniaJetOut.z_jettyCorr = pt/oniaJetOut.jt_pt_jettyCorr;

        oniaJetOut.gen_z = genQQmom4->Pt()/oniaJetOut.jt_ref_pt;
	    if (oniaJetOut.gen_z >= 1 && oniaJetOut.gen_z <= 1.0001) oniaJetOut.gen_z = 0.9999;
	    oniaJetOut.jt_pt_genZJEC = zjecCorr(jt_pt_noZJEC, inJet->rawpt[iJet], oniaJetOut.gen_z);
    }

    public:

    OniaWriterJet(const char* treeName):
        writer(treeName)
    {
        oniaInfoOut.addOutputs(&writer);
        recoQQOut.addOutputs(&writer,"reco_");
        genQQOut.addOutputs(&writer,"gen_");
        oniaMuOut.addOutputs(&writer);
        oniaJetOut.addOutputs(&writer);
    }

    void writeQQ(const Reader* dataIn, int iQQ, int iJet, int entry, JetCorrector* JEC, JetUncertainty* JEU)
    {
        TLorentzVector *RecoQQ4mom = (TLorentzVector*) dataIn->recoQQ.mom4->At(iQQ);
        TLorentzVector *GenQQ4mom = (TLorentzVector*) dataIn->genQQ.mom4->At(dataIn->which.RecoQQWhichGen[iQQ]);
        oniaInfoOut.Write(entry,dataIn->genQQ.id[iQQ]);
        recoQQOut.Write(RecoQQ4mom);
        genQQOut.Write(GenQQ4mom);
        int mumi_idx= dataIn->recoQQ.mumi_idx[iQQ];
        int mupl_idx= dataIn->recoQQ.mupl_idx[iQQ];
        TLorentzVector* mumi = (TLorentzVector*) dataIn->recoMu.mom4->At(mumi_idx);
        TLorentzVector* mupl = (TLorentzVector*) dataIn->recoMu.mom4->At(mupl_idx);
        oniaMuOut.Write(mupl,mumi);

        writeJet(dataIn,iQQ,iJet,JEC,JEU);
        
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