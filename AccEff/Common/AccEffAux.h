#ifndef ACCEFFAUX
#define ACCEFFAUX

#include"TH1.h"
#include"TEfficiency.h"

#include "../../OniaBase/Params/Params.h"

TH1F* toTH1F(const TEfficiency* asym);
TH1F* calcCorrectedYields(TH1F* nSig,TEfficiency* AccXEff,const std::string& subfix="_corr");
TH1F* calcDN_DpT(TH1F* nSigCorrected);
TH1F* Normalize(TH1F* nSigCorrected);

const char accDenName[] ="PtQQ_Gen";
const char accNumName[] ="PtQQ_Det_acc";
const char effDenName[] = "PtQQ_Det_eff";
const char effNumName[] = "PtQQ_RecoCut";
const char accXEffName[]= "ptQQ_AccXEff";
const char accName[]    = "PtQQ_Acceptancy";
const char effName[]    = "PtQQ_Efficiency";
const char nSigY1SName[]= "signal_nSigY1S";
const char yieldFitFuncName[]= "ratio_fit";
const char yieldFitName[] = "ratio_dN_dpT_norm";


const float integratedLum = 302000.0f;// in nb^-1
const float yRange = 4.8f;

constexpr std::array<float,6> ref_cross_section_value=
{ 0.102f, 0.193f , 0.176f, 0.093f, 0.039f, 0.0046f};
constexpr std::array<float,6> ref_cross_section_error_stat=
{ 0.002f, 0.006f, 0.004f, 0.002f, 0.001f, 0.0001f};
constexpr std::array<float,6> ref_cross_section_error_sys=
{ 0.016f, 0.012f, 0.011f, 0.005f, 0.001f, 0.0002f};

constexpr std::array<double,7> pt_bins  
{ 0.0f,2.0f,4.0f,6.0f,9.0f,12.0f,30.0f};

class WeightFuncRooAbs : public WeightFunc
{
    RooAbsReal* f;
    public:
    float getWeight(float x) override;

    WeightFuncRooAbs(RooAbsReal* func);
};

#endif