#ifndef ACCEFFAUX
#define ACCEFFAUX

#include"TH1.h"
#include"TH2.h"
#include"TEfficiency.h"

#include "../../OniaBase/Params/Params.h"

TH1D* toTH1D(const TEfficiency* asym);
TH1D* calcCorrectedYields(TH1D* nSig,TEfficiency* AccXEff,const std::string& subfix="_corr");
TH1D* calcDN_DpT(TH1D* nSigCorrected);
TH1D* Normalize(TH1D* nSigCorrected);

Double_t weightF(Double_t x);

const char accDenName[] ="PtQQ_Gen";
const char accNumName[] ="PtQQ_Det_acc";
const char effDenName[] = "PtQQ_Det_eff";
const char effNumName[] = "PtQQ_RecoCut";
const char accXEffName[]= "ptQQ_AccXEff";
const char accName[]    = "PtQQ_Acceptancy";
const char effName[]    = "PtQQ_Efficiency";
const char nSigY1SName[]= "signal_nSigY1S";
const char weightFuncRooAbsName[]= "ratio_fit";
const char weightFuncTH2Name[]= "hGenUpsilonPtvsEtaWeight";
const char yieldFitName[] = "ratio_dN_dpT_norm";


const double integratedLum = 302000.0;// in nb^-1
const double yRange = 4.8;

const double yMax = 2.4;
const double ptMax = 30.0;

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
    float getWeight(float x) const override;
    double getWeight(double x) const override;

    WeightFuncRooAbs(RooAbsReal* func);
};

class WeightFuncTEff : public WeightFunc
{
    TEfficiency* hist;

    public:
    float getWeight(float x) const override;
    double getWeight(double x) const override;

    WeightFuncTEff(TEfficiency* h) : hist(h) {};
};

class WeightFuncTH1 : public WeightFunc
{
    TH1* hist;

    public:
    float getWeight(float x) const override;
    double getWeight(double x) const override;

    WeightFuncTH1(TH1* h) : hist(h) {};
};

class WeightFuncTH1_2D : public WeightFunc2D
{
    TH1* hist;

    public:
    float getWeight(float x, float y) const override;
    double getWeight(double x, double y) const override;

    WeightFuncTH1_2D(TH1* h) : hist(h) {};
};

class WeightFuncTH2 : public WeightFunc2D
{
    TH2* hist;

    public:
    float getWeight(float x, float y) const override;
    double getWeight(double x, double y) const override;

    WeightFuncTH2(TH2* h) : hist(h) {};
};


#endif