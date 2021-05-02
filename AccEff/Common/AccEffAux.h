#ifndef ACCEFFAUX
#define ACCEFFAUX

#include"TH1.h"
#include"TEfficiency.h"

TH1F* toTH1F(const TEfficiency* asym);
TH1F* calcCorrectedYields(TH1F* nSig,TEfficiency* AccXEff);
TH1F* calcDN_DpT(TH1F* nSigCorrected);
TH1F* Normalize(TH1F* nSigCorrected);

constexpr const char* accDenName ="PtQQ_Gen";
constexpr const char* accNumName ="PtQQ_Det_acc";
constexpr const char* effDenName= "PtQQ_Det_eff";
constexpr const char* effNumName= "PtQQ_RecoCut";
constexpr const char* accXEffName= "ptQQ_AccXEff";
constexpr const char* nSigY1SName= "signal_nSigY1S";
constexpr const char* yieldFitFuncName= "ratio_fit";
constexpr const char yieldFitName[] = "ratio_dN_dpT_norm";

#endif