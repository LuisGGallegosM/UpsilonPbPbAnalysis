
#include "Main.h"

#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"

#define FITFUNCNAME "fitFunc"
#define DATASETNAME "dataset"

using namespace std;
using namespace RooFit;

TCanvas* drawData(RooRealVar* var, RooDataSet* dataset, RooAbsReal* fittedFunc,RooFitResult* fitResults, const kineCutParam* kineCut);
void drawTexts(RooRealVar* var,RooAbsReal* fittedFunc, const kineCutParam* kineCut);
void drawPullText(RooHist* hist, RooFitResult* fitResults);

void setGraphStyle(RooPlot* plot,const kineCutParam* kineCut);
void setPullStyle(RooPlot* pullPlot);
TLegend* drawLegend(RooPlot* plot);

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
    fittedFunc->plotOn(plot,Range(kineCut->massLow,kineCut->massHigh),Name(FITFUNCNAME),Normalization(1.0,RooAbsReal::RelativeExpected));
    fittedFunc->plotOn(plot,Range(kineCut->massLow,kineCut->massHigh),Name("cb1"),Components("cball_1"),LineStyle(2),LineColor(kGreen),Normalization(1.0,RooAbsReal::RelativeExpected));
    fittedFunc->plotOn(plot,Range(kineCut->massLow,kineCut->massHigh),Name("cb2"),Components("cball_2"),LineStyle(2),LineColor(kRed),Normalization(1.0,RooAbsReal::RelativeExpected));
    dataset->plotOn(plot,Range(kineCut->massLow,kineCut->massHigh),Name(DATASETNAME), MarkerSize(0.5), XErrorSize(0));
    TLegend* legend= drawLegend(plot);

    setGraphStyle(plot,kineCut);
    plot->Draw();
    legend->Draw("same");

    drawTexts(var,fittedFunc,kineCut);
    
    //draw pull
    pull->cd();
    RooPlot* pullPlot = var->frame(Title("Pull Distribution"));
    RooHist* pullHist = plot->pullHist(DATASETNAME,FITFUNCNAME);
    pullHist->SetMarkerSize(0.5f);
    
    pullPlot->addPlotable(pullHist,"P");
    
    setPullStyle(pullPlot);  
    pullPlot->Draw();
    TLine *l1 = new TLine(8.5,0,10.0,0.0);
    l1->SetLineColor(4);
    l1->Draw("same");

    drawPullText(pullHist,fitResults);

    return canvas;
}

void setGraphStyle(RooPlot* plot,const kineCutParam* kineCut)
{
    float div= (kineCut->massHigh - kineCut->massLow)/(kineCut->nBins);

    plot->SetTitle("PbPb #varUpsilon(1S) MC");
    plot->SetFillStyle(4000);
    plot->SetMarkerStyle(2);
    plot->SetMarkerSize(0.02);
    //plot->SetAxisRange(kineCut->massLow,kineCut->massHigh);

    plot->GetXaxis()->SetLabelSize(0);
    plot->GetXaxis()->SetTitleSize(0);
    plot->GetXaxis()->SetRangeUser(kineCut->massLow,kineCut->massHigh);

    plot->GetYaxis()->SetTitleOffset(1.4);
    plot->GetYaxis()->CenterTitle();
    plot->GetYaxis()->SetTitleSize(0.048);
    plot->GetYaxis()->SetTitle( Form("Events / ( %.3f GeV/c^{2} )",div));
    plot->GetYaxis()->SetRangeUser(10,1000000);

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
    pullPlot->GetYaxis()->SetTitleSize(0.13) ;
    pullPlot->GetYaxis()->SetLabelSize(0.09) ;
    pullPlot->GetYaxis()->SetRangeUser(-6,6) ;
    pullPlot->GetYaxis()->CenterTitle();
    pullPlot->GetYaxis()->SetTickSize(0.02);
    pullPlot->GetYaxis()->SetNdivisions(505);

    pullPlot->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} (GeV/c^{2})");
    pullPlot->GetXaxis()->SetTitleOffset(1.20) ;
    pullPlot->GetXaxis()->SetLabelOffset(0.04) ;
    pullPlot->GetXaxis()->SetLabelSize(0.10) ;
    pullPlot->GetXaxis()->SetTitleSize(0.15) ;
    pullPlot->GetXaxis()->CenterTitle();
    pullPlot->GetXaxis()->SetRangeUser(8.5,10.0);
    pullPlot->GetXaxis()->SetTickSize(0.03);
    return;
}

TLegend* drawLegend(RooPlot* plot)
{
    TLegend* fitleg = new TLegend(0.75,0.65,0.85,0.85); 
    fitleg->SetTextSize(15);
    fitleg->SetTextFont(43);
    fitleg->SetBorderSize(0);
    fitleg->AddEntry(plot->findObject(DATASETNAME),"Data","pe");
    fitleg->AddEntry(plot->findObject(FITFUNCNAME),"Total fit","l");
    fitleg->AddEntry(plot->findObject("cb1"),"CBall 1","l");
    fitleg->AddEntry(plot->findObject("cb2"),"CBall 2","l");
    return fitleg;
}

void drawTexts(RooRealVar* var,RooAbsReal* fittedFunc,const kineCutParam* kineCut)
{
    TextDrawer tdrawer(0.18,0.8);
    RooArgSet* params= fittedFunc->getParameters(*var);

    float alpha_1 = params->getRealValue("alpha_1");
    float alpha_2 = params->getRealValue("alpha_2");
    float c1 = params->getRealValue("c1");
    float c2 = params->getRealValue("c2");
    float mean_1 =params->getRealValue("mean_1");
    float mean_2 =params->getRealValue("mean_2");
    float n_1 = params->getRealValue("n_1");
    float n_2 = params->getRealValue("n_2");
    float sigma_1 = params->getRealValue("sigma_1");
    float sigma_2 = params->getRealValue("sigma_2");

    tdrawer.drawText("#varUpsilon(1S) #rightarrow #mu#mu");
    tdrawer.drawText( Form("#alpha_{1}=%.3f, #alpha_{2}=%.3f",alpha_1,alpha_2));
    tdrawer.drawText( Form("m_{1}=%.3f, m_{2}=%.3f",mean_1,mean_2));
    tdrawer.drawText( Form("N_{1}=%.3f, N_{2}=%.3f",n_1,n_2));
    tdrawer.drawText( Form("#sigma_{1}=%.4f, #sigma_{2}=%.4f",sigma_1,sigma_2));
    tdrawer.drawText( Form("c_{1}=%.0f, c_{2}=%.0f",c1,c2));

    TextDrawer tdrawer2(0.4,0.8);
    if (kineCut->ptLow == 0.0f)
        tdrawer2.drawText(Form("p_{T}^{#mu#mu} < %.f GeV/c", kineCut->ptHigh));
    else 
        tdrawer2.drawText(Form("%.f < p_{T}^{#mu#mu} < %.f GeV/c",kineCut->ptLow, kineCut->ptHigh));
    
    if (kineCut->yLow == 0.0f)
        tdrawer2.drawText(Form("|y^{#mu#mu}| < %.1f", kineCut->yHigh));
    else 
        tdrawer2.drawText(Form("%.1f < |y^{#mu#mu}| < %.1f",kineCut->yLow, kineCut->yHigh));
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

    TLatex *tex = new TLatex(0.65,0.85,Form("#chi^{2}/ndf : %.1f / %d",chisq,ndf));
    tex->SetTextFont(43);
    tex->SetTextSize(12.0);
    tex->SetNDC();
    tex->Draw();
}