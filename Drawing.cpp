
#include "Main.h"

#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"
#include "TStyle.h"

#define FITFUNCNAME "fitFunc"
#define DATASETNAME "dataset"

using namespace std;
using namespace RooFit;

TCanvas* drawData(RooRealVar* var, RooDataSet* dataset, RooAbsReal* fittedFunc,RooFitResult* fitResults, const kineCutParam* kineCut);
void drawTexts(RooRealVar* var,RooAbsReal* fittedFunc, const kineCutParam* kineCut);
void drawPullText(RooHist* hist, RooFitResult* fitResults);

void setGraphStyle(RooPlot* plot,const kineCutParam* kineCut);
void setPullStyle(RooPlot* pullPlot);
TLegend* drawLegend(RooPlot* plot, const kineCutParam* kineCut);
void setTDRStyle();

void Drawing(const char* filename, const kineCutParam* kineCut)
{
    TFile file(filename,"UPDATE");

    if (file.IsZombie())
    {
        cout << "File " << filename << " not found.";
        return;
    }

    RooRealVar* massVar = (RooRealVar*) (file.Get("mass"));
    RooAbsReal* fittedFunc = (RooAbsReal*) (file.Get("dcb_fit"));
    RooDataSet* dataset = (RooDataSet*) (file.Get("dataset"));
    RooFitResult* fitResults = (RooFitResult*) (file.Get("fitresults"));

    if ((massVar==nullptr) || (fittedFunc==nullptr) || (dataset==nullptr))
    {
        cout << "Needed data not found in file" ;
        return;
    }

    TCanvas* canvas = drawData(massVar,dataset,fittedFunc,fitResults,kineCut);

    if (canvas==nullptr)
    {
        cout << "Canvas not generated";
        return;
    }

    canvas->Update();
    canvas->Write(0,TObject::kOverwrite);
    canvas->SaveAs("files/massfit.pdf");

    return;
}

/**
 * @brief Draws the data in variable var of dataset and fittedFunc in same plot.
 * 
 * @param var variable to use in x axis.
 * @param dataset experimental/sim data points.
 * @param fittedFunc fitted curve to experimental/sim
 * @return Drawn canvas.
 */
TCanvas* drawData(RooRealVar* var, RooDataSet* dataset, RooAbsReal* fittedFunc,RooFitResult* fitResults, const kineCutParam* kineCut)
{
    TCanvas* canvas = new TCanvas("Fit_plot","mass fit",4,45,550,520);
    canvas->SetLeftMargin(2.6);
    canvas->cd();

    //setTDRStyle();

    TPad* graph = new TPad("graph_pad","mass fit", 0, 0.30, 0.98, 1.0);
    graph->SetTicks(1,1);
    graph->SetLeftMargin(0.15);
    graph->SetLogy();
    graph->Draw();

    TPad* pull = new TPad("pull_pad","mass fit pull", 0, 0.00, 0.98, 0.35);
    pull->SetTopMargin(0); // Upper and lower plot are joined
    pull->SetBottomMargin(0.63); 
    pull->SetLeftMargin(0.15);
    pull->SetTicks(1,1);
    pull->Draw();

    //draw fitted and dataset function
    graph->cd();
    RooPlot* plot = var->frame(kineCut->nBins);
    fittedFunc->plotOn(plot,Name(FITFUNCNAME),LineColor(kOrange+7),Normalization(1.0,RooAbsReal::RelativeExpected));
    fittedFunc->plotOn(plot,Name("dcb"),Components("dcb"),LineStyle(9),LineColor(13),Normalization(1.0,RooAbsReal::RelativeExpected));
    fittedFunc->plotOn(plot,Name("cb1"),Components("cball_1"),LineStyle(7),LineColor(kGreen-2),Normalization(1.0,RooAbsReal::RelativeExpected));
    fittedFunc->plotOn(plot,Name("cb2"),Components("cball_2"),LineStyle(7),LineColor(kMagenta+1),Normalization(1.0,RooAbsReal::RelativeExpected));
    if (kineCut->bkgOn)
      fittedFunc->plotOn(plot,Name("bkg"),Components("bkg"),LineStyle(kDashed),LineColor(kBlue),Normalization(1.0,RooAbsReal::RelativeExpected));
    dataset->plotOn(plot,Name(DATASETNAME), MarkerSize(0.5), XErrorSize(0));
    TLegend* legend= drawLegend(plot,kineCut);

    setGraphStyle(plot,kineCut);
    plot->Draw("same");
    legend->Draw("same");

    drawTexts(var,fittedFunc,kineCut);
    
    //draw pull
    pull->cd();
    RooPlot* pullPlot = var->frame(Title("Pull Distribution"));
    RooHist* pullHist = plot->pullHist(DATASETNAME,FITFUNCNAME);
    pullHist->SetMarkerSize(0.5f);
    
    pullPlot->addPlotable(pullHist,"PSAME");
    
    setPullStyle(pullPlot);  
    pullPlot->Draw("same");
    TLine *l1 = new TLine(8.5,0,10.0,0.0);
    l1->SetLineColor(4);
    l1->Draw("same");

    drawPullText(pullHist,fitResults);

    return canvas;
}

void setGraphStyle(RooPlot* plot,const kineCutParam* kineCut)
{
    float div= (kineCut->massHigh - kineCut->massLow)/(kineCut->nBins);

    plot->SetTitle("PbPb Nonprompt #varUpsilon(1S) MC ( 5.02 TeV)");
    plot->SetFillStyle(4000);
    plot->SetMarkerStyle(2);
    plot->SetMarkerSize(0.02);
    plot->SetAxisRange(kineCut->massLow,kineCut->massHigh);

    plot->GetXaxis()->SetLabelSize(0);
    plot->GetXaxis()->SetTitleSize(0);
    plot->GetXaxis()->SetRangeUser(kineCut->massLow,kineCut->massHigh);

    plot->GetYaxis()->SetTitleOffset(1.0);
    plot->GetYaxis()->CenterTitle();
    plot->GetYaxis()->SetTitleSize(0.048);
    plot->GetYaxis()->SetTitle( Form("Events / ( %.3f GeV/c^{2} )",div));
    plot->GetYaxis()->SetRangeUser(10,10000000);

    return;
}

void setPullStyle(RooPlot* pullPlot)
{
    pullPlot->SetTitleSize(0);
    pullPlot->SetMarkerStyle(2);
    pullPlot->SetMarkerSize(0.01);
    
    pullPlot->SetDrawOption("HIST");
    pullPlot->GetYaxis()->SetTitleOffset(0.31) ;
    pullPlot->GetYaxis()->SetTitle("Pull") ;
    pullPlot->GetYaxis()->SetTitleSize(0.10) ;
    pullPlot->GetYaxis()->SetLabelSize(0.09) ;
    pullPlot->GetYaxis()->SetRangeUser(-7,7) ;
    pullPlot->GetYaxis()->CenterTitle();
    pullPlot->GetYaxis()->SetTickSize(0.02);
    pullPlot->GetYaxis()->SetNdivisions(505);

    pullPlot->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} (GeV/c^{2})");
    pullPlot->GetXaxis()->SetTitleOffset(1.20) ;
    pullPlot->GetXaxis()->SetLabelOffset(0.04) ;
    pullPlot->GetXaxis()->SetLabelSize(0.10) ;
    pullPlot->GetXaxis()->SetTitleSize(0.10) ;
    pullPlot->GetXaxis()->CenterTitle();
    pullPlot->GetXaxis()->SetRangeUser(8.5,10.0);
    pullPlot->GetXaxis()->SetTickSize(0.03);
    return;
}

TLegend* drawLegend(RooPlot* plot,const kineCutParam* kineCut)
{
    TLegend* fitleg = new TLegend(0.70,0.65,0.85,0.85);
    fitleg->SetTextSize(12);
    fitleg->SetTextFont(43);
    fitleg->SetBorderSize(0);
    fitleg->AddEntry(plot->findObject(DATASETNAME),"Data","pe");
    fitleg->AddEntry(plot->findObject(FITFUNCNAME),"Total fit","l");
    fitleg->AddEntry(plot->findObject("dcb"),"Signal","l");
    if (kineCut->bkgOn)
      fitleg->AddEntry(plot->findObject("bkg"),"Background","l");
    fitleg->AddEntry(plot->findObject("cb1"),"CBall 1","l");
    fitleg->AddEntry(plot->findObject("cb2"),"CBall 2","l");
    return fitleg;
}

void drawTexts(RooRealVar* var,RooAbsReal* fittedFunc,const kineCutParam* kineCut)
{
    TextDrawer tdrawer(0.22,0.8);
    RooArgSet* params= fittedFunc->getParameters(*var);

    float alpha_1 = params->getRealValue("alpha_1");
    float fs = params->getRealValue("fs");
    float mean_1 =params->getRealValue("mean_1");
    float n_1 = params->getRealValue("n_1");
    float sigma_1 = params->getRealValue("sigma_1");
    float sigma_2 = params->getRealValue("sigma_2");

    tdrawer.drawText("#varUpsilon(1S) #rightarrow #mu#mu");
    tdrawer.drawText( Form("#alpha=%.3f",alpha_1));
    tdrawer.drawText( Form("m=%.3f",mean_1));
    tdrawer.drawText( Form("N=%.3f",n_1));
    tdrawer.drawText( Form("#sigma_{1}=%.4f, #frac{#sigma_{2}}{#sigma_{1}}=%.4f",sigma_1,sigma_2/sigma_1));
    tdrawer.drawText( Form("f_{s}=%.4f",fs));

    TextDrawer tdrawer2(0.45,0.8);
    if (kineCut->ptLow == 0.0f)
        tdrawer2.drawText(Form("p_{T}^{#mu#mu} < %.1f GeV/c", kineCut->ptHigh));
    else 
        tdrawer2.drawText(Form("%.f < p_{T}^{#mu#mu} < %.1f GeV/c",kineCut->ptLow, kineCut->ptHigh));
    
    if (kineCut->yLow == 0.0f)
        tdrawer2.drawText(Form("|y^{#mu#mu}| < %.2f", kineCut->yHigh));
    else 
        tdrawer2.drawText(Form("%.2f < |y^{#mu#mu}| < %.2f",kineCut->yLow, kineCut->yHigh));
    tdrawer2.drawText(Form("p_{T}^{#mu} > %.1f GeV/c", kineCut->singleMuPtLow));
    tdrawer2.drawText(Form("|#eta^{#mu}| < %.2f",kineCut->singleMuEtaHigh));
}

void drawPullText(RooHist* hist,RooFitResult* fitResults)
{
    double* pullVal = hist->GetY();
    int nbins = hist->GetN();
    double chisq =0.0;
    int nFullBins =0;

    for(int i=0;i<nbins;++i)
    {
        double val=pullVal[i];
        if (val==0.0) continue;
        chisq+= val*val;
        ++nFullBins;
    }

    int nFitPar = fitResults->floatParsFinal().getSize();
    int ndf = nFullBins - nFitPar;

    TLatex *tex = new TLatex(0.72,0.93,Form("#chi^{2}/ndf : %.1f / %d",chisq,ndf));
    tex->SetTextFont(43);
    tex->SetTextSize(12.0);
    tex->SetNDC();
    tex->Draw("same");
}

void setTDRStyle() 
{
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);
  
// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(0);
  // tdrStyle->SetErrorMarker(20);
  //tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);
  
//For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.08); // was 0.05
  tdrStyle->SetPadBottomMargin(0.12); // was 0.13
  tdrStyle->SetPadLeftMargin(0.16); // was 0.16
  tdrStyle->SetPadRightMargin(0.04); // was 0.02

// For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9); // was 0.9
  tdrStyle->SetTitleYOffset(1.25); // was 1.25
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

// Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->SetHatchesLineWidth(5);
  tdrStyle->SetHatchesSpacing(0.05);

  tdrStyle->cd();
}