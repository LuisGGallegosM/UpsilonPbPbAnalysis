
#include "AccEffAux.h"

#include <array>

#include "TCanvas.h"
#include "TGraphAsymmErrors.h"

std::unique_ptr<TEfficiency> createTEff(TH1* num, TH1* den, const std::string& name, const std::string& title)
{
    TEfficiency* teff=new TEfficiency(*num,*den);
    teff->SetStatisticOption(TEfficiency::EStatOption::kFNormal);
    teff->SetName(name.data());
    teff->SetTitle(title.data());
    return std::unique_ptr<TEfficiency>(teff);
}


TH1D* toTH1D(const TEfficiency* asym)
{
    int nBins= asym->GetPassedHistogram()->GetNbinsX()+2;
    TH1D* result = new TH1D(*dynamic_cast<TH1D*>(asym->GetPassedHistogram()->Clone()));
    for(int i=0;i< nBins;i++)
    {
        result->SetBinContent(i,asym->GetEfficiency(i));
        double upErr= asym->GetEfficiencyErrorUp(i);
        double downErr = asym->GetEfficiencyErrorLow(i);
        result->SetBinError(i, (upErr > downErr) ? upErr : downErr );
    }
    return result;
}

TH1D* calcCorrectedYields(TH1D* nSig,TEfficiency* AccXEff,const std::string& subfix)
{
    TH1D* AccXEffth1=toTH1D(AccXEff);
    TH1D* nSigCorrected = new TH1D((*nSig)/(*AccXEffth1));
    std::string newname=nSigCorrected->GetName();
    newname+=subfix;
    nSigCorrected->SetName(newname.data());
    nSigCorrected->SetStats(false);
    return nSigCorrected;
}


TH1D* Normalize(TH1D* nSigCorrected)
{
    TH1D* dN_dPt = new TH1D(*nSigCorrected);
    dN_dPt->Scale(1.0/dN_dPt->Integral());
    std::string newname=nSigCorrected->GetName();
    newname+="_norm";
    dN_dPt->SetName(newname.data());
    return dN_dPt;
}

TH1D* calcDN_DpT(TH1D* nSigCorrected)
{
    TH1D* dN_dPt = new TH1D(*nSigCorrected);
    int nbins = dN_dPt->GetNbinsX() +2;
    for(int i=0;i<nbins;i++)
    {
        double dPt= dN_dPt->GetBinWidth(i);
        double factor=1.0/dPt;
        dN_dPt->SetBinContent(i, dN_dPt->GetBinContent(i)*factor);
        dN_dPt->SetBinError(i, nSigCorrected->GetBinError(i)*factor );
    }
    std::string newname=nSigCorrected->GetName();
    newname+="_dN_dpT";
    dN_dPt->SetName(newname.data());
    dN_dPt->SetTitle("#frac{dN}{p_{T}}");
    return dN_dPt;
}

float WeightFuncRooAbs::getWeight(float x) const
{
    f->getVariables()->setRealValue("pt",x);
    return f->getVal();
}

double WeightFuncRooAbs::getWeight(double x) const
{
    f->getVariables()->setRealValue("pt",x);
    return f->getVal();
}

WeightFuncRooAbs::WeightFuncRooAbs(RooAbsReal* func): f(func)
{
}

float WeightFuncTEff::getWeight(float x) const
{
    return 1.0/hist->GetEfficiency(hist->FindFixBin(x));
}

double WeightFuncTEff::getWeight(double x) const
{
    return 1.0/hist->GetEfficiency(hist->FindFixBin(x));
}