#ifndef ONIADATASIMPLE
#define ONIADATASIMPLE

#include"TLorentzVector.h"
#include "../../TreeProcessor/TreeWriter.h"

struct OniaSimpleInfo
{
    Int_t Evt;
    Int_t pdgId;

    void addOutputs(TreeWriter* writer);
    void Write(int entry, int id)
    {
        Evt=entry;
        pdgId=id;
    }
};

struct OniaSimpleQQ
{
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;

    void addOutputs(TreeWriter* writer, const char* prefix=nullptr);
    void Write(TLorentzVector* dimuon);
};

struct OniaSimpleMu
{
    Float_t pT_mi;
    Float_t eta_mi;
    Float_t phi_mi;
    Float_t pT_pl;
    Float_t eta_pl;
    Float_t phi_pl;

    void addOutputs(TreeWriter* writer, const char* prefix=nullptr);
    void Write(TLorentzVector* muonPl, TLorentzVector* muonMi);
};

struct OniaSimpleJet
{
    Float_t jt_pt;
    Float_t jt_rap;
    Float_t jt_eta;
    Float_t jt_phi;
    Float_t z;
    Float_t jt_pt_JEU_Down;
    Float_t jt_pt_JEU_Up;
    Float_t jt_pt_jettyCorr;
    Float_t z_jettyCorr;

    Float_t jt_ref_pt;
    Float_t jt_ref_rap;
    Float_t jt_ref_eta;
    Float_t jt_ref_phi;
    Float_t gen_z;
    Float_t jt_pt_genZJEC;
    void addOutputs(TreeWriter* writer);
};

#endif