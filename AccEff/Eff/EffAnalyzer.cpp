
#include "EffAnalyzer.h"
#include "tnp_weight_lowptpp.h"

std::unique_ptr<EffAnalyzer> createEffAnalyzer(TTree* input,CutParams* effCut, const char* outTreeName, WeightFunc* weights, bool onlyNumCorr )
{
    if (effCut->getIsMC())
        return std::unique_ptr<EffAnalyzer>(new EffAnalyzerMC(input,effCut,outTreeName,weights,onlyNumCorr));
    else
        return nullptr;
}

double calculateTnp(double muplPt, double mumiPt, double muplY, double mumiY)
{
    auto muplGlb = tnp_weight_GlobalMuon_TightAcceptance_pp(muplPt,muplY);
	auto mumiGlb = tnp_weight_GlobalMuon_TightAcceptance_pp(mumiPt,mumiY);
	auto muplMuIdTrg = tnp_weight_HybridSoftIDTrigger_TightAcceptance_pp(muplPt,muplY);
	auto mumiMuIdTrg = tnp_weight_HybridSoftIDTrigger_TightAcceptance_pp(mumiPt,mumiY);
    return std::get<0>(muplGlb) * std::get<0>(muplMuIdTrg)
	      *std::get<0>(mumiGlb) * std::get<0>(mumiMuIdTrg);
}