
#include"BaseCanvas.h"

#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "THStack.h"

#include"CMS_lumi.h"

constexpr std::array<double,7> pTBins  { 0.0f,2.0f,4.0f,6.0f,9.0f,12.0f,30.0f};
constexpr std::array<double,6> EtaBins { 0.0f,0.5f,1.0f,1.5f,2.0f,2.4f};

TH2D* createTH2QQ(const std::string& name,const std::string& title)
{
    TH2D* result =new TH2D(name.data(),title.data(),EtaBins.size()-1,EtaBins.data(),pTBins.size()-1,pTBins.data());
    result->SetStats(false);
    result->Sumw2();
    return result;
}

TH2D* createTH2Mu(const std::string& name,const std::string& title)
{
    const int binPtN = 40;
    const float ptMax = 20.0f;
    const int binEtaN = 40;
    const float etaMax = 4.0f;
    TH2D* result =new TH2D(name.data(),title.data(),binEtaN,0.0,etaMax,binPtN,0.0,ptMax);
    result->SetStats(false);
    result->Sumw2();
    return result;
}

TH1D* createTH1(const std::string& name,const std::string& title)
{
    TH1D* result=new TH1D(name.data(),title.data(),pTBins.size()-1,pTBins.data());
    result->SetStats(false);
    result->Sumw2();
    return result;
}

void writeToCanvas(TGraph* hist,const std::string& xname,const std::string& yname, const std::string& outname, const std::string& option)
{
    const std::string canvasName=std::string(hist->GetName())+" plot";
    const std::string outputFilename=outname+"_"+hist->GetName()+".pdf";
    TCanvas canvas(canvasName.data(),canvasName.data(),canvasWidth,canvasHeight);
    canvas.cd();
    TPad pad("pad","fit", padSizes[0], padSizes[1], padSizes[2], padSizes[3]);
    pad.Draw();
    pad.cd();
    hist->GetYaxis()->SetTitle(yname.data());
    hist->GetXaxis()->SetTitle(xname.data());
    hist->Draw(option.data());
    canvas.Write();
    canvas.SaveAs(outputFilename.data());
}

void writeToCanvasBase(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname, const std::string& option, bool yLog)
{
    const std::string canvasName=std::string(hist->GetName())+" plot";
    const std::string outputFilename=outname+"_"+hist->GetName()+".pdf";
    TCanvas canvas(canvasName.data(),canvasName.data(),canvasWidth,canvasHeight);
    canvas.cd();
    TPad pad("pad","fit", padSizes[0], padSizes[1], padSizes[2], padSizes[3]);
    CMS_lumi( &pad);
    pad.Draw();
    pad.cd();
    if (yLog) pad.SetLogy();
    hist->GetYaxis()->SetTitle(yname.data());
    hist->GetXaxis()->SetTitle(xname.data());
    hist->Draw(option.data());
    canvas.Write();
    canvas.SaveAs(outputFilename.data());
}

void writeToCanvas(std::vector<TH1*>& hists,const std::string& title,const std::string& xname,const std::string& yname, const std::string& outname, bool yLog, float lowlimit, float highlimit)
{
    TCanvas canvas((title+"_plot").data(),"",canvasWidth,canvasHeight);
    canvas.cd();

    TPad pad("pad","fit", padSizes[0], padSizes[1], padSizes[2], padSizes[3]);
    pad.Draw();
    pad.cd();
    if (yLog) pad.SetLogy();
    THStack stack((title+"_stack").data(),(title+" plot").data());
    if( ! isnanf(lowlimit) )
    {
        stack.SetMaximum(highlimit);
        stack.SetMinimum(lowlimit);
    } 
    int i=2;
    for(auto hist : hists)
    {
        const int color= (i!=3) ? i : 30;
        hist->SetLineColor(color);
        hist->SetLineWidth(3);
        if (hists.size()<5) 
            hist->SetMarkerStyle(24+i);
        else
            hist->SetMarkerStyle(1);
        stack.Add(hist);
        hist->Write();
        i++;
    }
   
    stack.Draw("nostack");
    stack.GetYaxis()->SetTitle(yname.data());
    stack.GetXaxis()->SetTitle(xname.data());
    
    pad.BuildLegend(0.85,0.75,1.0,1.0);
    canvas.Write(0, TObject::kOverwrite);
    canvas.SaveAs(outname.data());
}

void writeToCanvas(TH2* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
    writeToCanvasBase(hist,xname,yname,outname,"COLZ",false);
}

void writeToCanvas(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname, bool yLog)
{
     writeToCanvasBase(hist,xname,yname,outname,"COL",yLog);
}

void writeToCanvasEff(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
    const std::string canvasName=std::string(hist->GetName())+" plot";
    const std::string outputFilename=outname+"_"+hist->GetName()+".pdf";
    TCanvas canvas(canvasName.data(),canvasName.data(),canvasWidth,canvasHeight);
    canvas.cd();
    TPad pad("pad","fit", padSizes[0], padSizes[1], padSizes[2], padSizes[3]);
    pad.Draw();
    pad.cd();
    hist->Draw();
    canvas.Write();
    canvas.SaveAs(outputFilename.data());
}

void writeToCanvasEff2D(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
    const std::string canvasName=std::string(hist->GetName())+" plot";
    const std::string outputFilename=outname+"_"+hist->GetName()+".pdf";
    TCanvas canvas(canvasName.data(),canvasName.data(),canvasWidth,canvasHeight);
    canvas.cd();
    TPad pad("pad","fit", padSizes[0], padSizes[1], padSizes[2], padSizes[3]);
    pad.Draw();
    pad.cd();
    hist->Draw("COLZ");
    canvas.Write();
    canvas.SaveAs(outputFilename.data());
}