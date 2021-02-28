
#include "OniaReader.h"

OniaReader::OniaReader(TTree* treeIn, bool isMC) : TreeReader(treeIn), genQQ(), recoQQ(), recoMu(), genMu()
{
    SelectionType = new Int_t[maxBranchSize];
    nTrkWMea = new Int_t[maxBranchSize];
    nPixWMea = new Int_t[maxBranchSize];
    dxy = new Float_t[maxBranchSize];
    dz = new Float_t[maxBranchSize]; 
    VtxProb = new Float_t[maxBranchSize];
    trig = new ULong64_t[maxBranchSize];
    RecoMuWhichGen = new Int_t[maxBranchSize];
    GenMuWhichReco = new Int_t[maxBranchSize];
    GenQQWhichReco = new Int_t[maxBranchSize];
    genQQid = new Int_t[maxBranchSize];

    //input branches
    addInput("Reco_QQ_4mom",&recoQQ.mom4);
    addInput("Reco_QQ_size",&recoQQ.size);
    addInput("Reco_QQ_mupl_idx",recoQQ.mupl_idx);
    addInput("Reco_QQ_mumi_idx",recoQQ.mumi_idx);
    addInput("Reco_QQ_VtxProb",VtxProb);
    addInput("Reco_QQ_trig",trig);
    addInput("Reco_QQ_sign",recoQQ.sign);

    addInput("Reco_mu_4mom",&recoMu.mom4);
    addInput("Reco_mu_size",&recoMu.size);
    addInput("Reco_mu_SelectionType",SelectionType);
    addInput("Reco_mu_nTrkWMea",nTrkWMea);
    addInput("Reco_mu_nPixWMea",nPixWMea);
    addInput("Reco_mu_dxy",dxy);
    addInput("Reco_mu_dz",dz);

    if(isMC)
    {
        addInput("Reco_mu_whichGen",RecoMuWhichGen);

        addInput("Gen_QQ_4mom",&genQQ.mom4);
        addInput("Gen_QQ_size",&genQQ.size);
        addInput("Gen_QQ_mupl_idx",genQQ.mupl_idx);
        addInput("Gen_QQ_mumi_idx",genQQ.mumi_idx);
        addInput("Gen_QQ_momId",genQQid);
        addInput("Gen_QQ_whichRec",GenQQWhichReco);

        addInput("Gen_mu_4mom",&genMu.mom4);
        addInput("Gen_mu_size",&genMu.size);
    }
}

OniaReader::~OniaReader()
{
    delete[] SelectionType;
    delete[] nTrkWMea;
    delete[] nPixWMea;
    delete[] dxy;
    delete[] dz;
    delete[] VtxProb;
    delete[] trig;
    delete[] RecoMuWhichGen;
    delete[] GenMuWhichReco;
    delete[] genQQid;
}

baseQQ::baseQQ()
{
    mom4 = new TClonesArray("TLorentzVector");
    mupl_idx = new Int_t[maxBranchSize];
    mumi_idx = new Int_t[maxBranchSize];
    sign = new Int_t[maxBranchSize];
}
baseQQ::~baseQQ()
{
    delete mom4;
    delete[] mupl_idx;
    delete[] mumi_idx;
    delete[] sign;
}

baseMu::baseMu()
{
    mom4 = new TClonesArray("TLorentzVector");
}

baseMu::~baseMu()
{
    delete mom4;
}