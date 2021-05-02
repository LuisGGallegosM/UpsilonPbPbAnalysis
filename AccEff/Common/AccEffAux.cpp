
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


TH1F* toTH1F(const TEfficiency* asym)
{
    int nBins= asym->GetPassedHistogram()->GetNbinsX()+2;
    TH1F* result = new TH1F(*dynamic_cast<TH1F*>(asym->GetPassedHistogram()->Clone()));
    for(int i=0;i< nBins;i++)
    {
        result->SetBinContent(i,asym->GetEfficiency(i));
        float upErr= asym->GetEfficiencyErrorUp(i);
        float downErr = asym->GetEfficiencyErrorLow(i);
        result->SetBinError(i, (upErr > downErr) ? upErr : downErr );
    }
    return result;
}

TH1F* calcCorrectedYields(TH1F* nSig,TEfficiency* AccXEff)
{
    TH1F* AccXEffth1f=toTH1F(AccXEff);
    TH1F* nSigCorrected = new TH1F((*nSig)/(*AccXEffth1f));
    std::string newname=nSigCorrected->GetName();
    newname+="_corr";
    nSigCorrected->SetName(newname.data());
    nSigCorrected->SetStats(false);
    return nSigCorrected;
}


TH1F* Normalize(TH1F* nSigCorrected)
{
    TH1F* dN_dPt = new TH1F(*nSigCorrected);
    dN_dPt->Scale(1.0f/dN_dPt->Integral());
    std::string newname=nSigCorrected->GetName();
    newname+="_norm";
    dN_dPt->SetName(newname.data());
    return dN_dPt;
}

TH1F* calcDN_DpT(TH1F* nSigCorrected)
{
    TH1F* dN_dPt = new TH1F(*nSigCorrected);
    int nbins = dN_dPt->GetNbinsX() +2;
    for(int i=0;i<nbins;i++)
    {
        float dPt= dN_dPt->GetBinWidth(i);
        float factor=1.0f/dPt;
        dN_dPt->SetBinContent(i, dN_dPt->GetBinContent(i)*factor);
        dN_dPt->SetBinError(i, nSigCorrected->GetBinError(i)*factor );
    }
    std::string newname=nSigCorrected->GetName();
    newname+="_dN_dpT";
    dN_dPt->SetName(newname.data());
    dN_dPt->SetTitle("#frac{dN}{p_{T}}");
    return dN_dPt;
}