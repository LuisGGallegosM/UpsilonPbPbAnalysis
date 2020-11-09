
#include "Drawing.h"

using namespace RooFit;

TCanvas* drawData(RooRealVar* var, RooDataSet* dataset, RooAbsReal* fittedFunc,RooFitResult* fitResults, const drawConfig* config);
void drawTexts(RooRealVar* var,RooAbsReal* fittedFunc, const drawConfig* cut);
void drawPullText(RooHist* hist, RooFitResult* fitResults);

void setGraphStyle(RooPlot* plot,const drawConfig* config);
void setPullStyle(RooPlot* pullPlot);
TLegend* drawLegend(RooPlot* plot, bool bkgOn);
void setTDRStyle();

void Drawing(const char* filename,const char* drawfilename, const char* cutfilename, const char* fitfilename)
{
    TFile file(filename,"UPDATE");

    if (file.IsZombie())
    {
        std::cerr << "Error: File " << filename << " not found.\n";
        return;
    }
    drawConfig config;
    config.deserialize(cutfilename,fitfilename);

    if (!config.isValid())
    {
        std::cerr << "Error: Invalid arguments\n";
        return;
    }

    RooRealVar* massVar = (RooRealVar*) (file.Get("mass"));
    RooAbsReal* fittedFunc = (RooAbsReal*) (file.Get("dcb_fit"));
    RooDataSet* dataset = (RooDataSet*) (file.Get("dataset"));
    RooFitResult* fitResults = (RooFitResult*) (file.Get("fitresults"));

    if ((massVar==nullptr) || (fittedFunc==nullptr) || (dataset==nullptr))
    {
        std::cerr << "Error: Needed data not found in file\n" ;
        return;
    }

    TCanvas* canvas = drawData(massVar,dataset,fittedFunc,fitResults,&config);

    if (canvas==nullptr)
    {
        std::cerr << "Error: Canvas not generated\n";
        return;
    }

    canvas->Update();
    canvas->Write(0,TObject::kOverwrite);
    canvas->SaveAs(drawfilename);

    return;
}

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    if (argc ==5)
        Drawing(argv[1],argv[2],argv[3],argv[4]);
    else
    {
        std::cerr << "Error: Incorrect number of parameters\n";  
    }
    return 0;
}

#endif



/**
 * @brief Draws the data in variable var of dataset and fittedFunc in same plot.
 * 
 * @param var variable to use in x axis.
 * @param dataset experimental/sim data points.
 * @param fittedFunc fitted curve to experimental/sim
 * @return Drawn canvas.
 */
TCanvas* drawData(RooRealVar* var, RooDataSet* dataset, RooAbsReal* fittedFunc,RooFitResult* fitResults, const drawConfig* config)
{
    if (!config->isValid())
    {
        std::cerr << "Error: invalid draw configuration\n";
        return nullptr;
    }

    TCanvas* canvas = new TCanvas("Fit_plot","mass fit",4,45,550,520);
    canvas->SetLeftMargin(2.6);
    canvas->cd();

    //setTDRStyle();

    TPad* graph = new TPad("graph_pad","mass fit", 0, 0.45, 0.98, 1.0);
    graph->SetTicks(1,1);
    graph->SetLeftMargin(0.15);
    graph->SetLogy();
    graph->Draw();

    TPad* pull = new TPad("pull_pad","mass fit pull", 0.00, 0.00, 0.98, 0.5);
    pull->SetTopMargin(0); // Upper and lower plot are joined
    pull->SetBottomMargin(0.63); 
    pull->SetLeftMargin(0.15);
    pull->SetTicks(1,1);
    pull->Draw();

    //draw fitted and dataset function
    graph->cd();
    RooPlot* plot = var->frame(config->nBins);
    fittedFunc->plotOn(plot,Name(FITFUNCNAME),LineColor(kOrange+7),Normalization(1.0,RooAbsReal::RelativeExpected));
    fittedFunc->plotOn(plot,Name("dcb"),Components("dcb"),LineStyle(9),LineColor(13),Normalization(1.0,RooAbsReal::RelativeExpected));
    fittedFunc->plotOn(plot,Name("cb1"),Components("cball_1"),LineStyle(7),LineColor(kGreen-2),Normalization(1.0,RooAbsReal::RelativeExpected));
    fittedFunc->plotOn(plot,Name("cb2"),Components("cball_2"),LineStyle(7),LineColor(kMagenta+1),Normalization(1.0,RooAbsReal::RelativeExpected));
    if (config->fitConf.bkgOn)
      fittedFunc->plotOn(plot,Name("bkg"),Components("bkg"),LineStyle(kDashed),LineColor(kBlue),Normalization(1.0,RooAbsReal::RelativeExpected));
    dataset->plotOn(plot,Name(DATASETNAME), MarkerSize(0.5), XErrorSize(0));
    TLegend* legend= drawLegend(plot,config->fitConf.bkgOn);

    setGraphStyle(plot,config);
    plot->Draw("same");
    legend->Draw("same");

    drawTexts(var,fittedFunc,config);
    
    //draw pull
    pull->cd();
    RooPlot* pullPlot = var->frame(Title("Pull Distribution"));
    RooHist* pullHist = plot->pullHist(DATASETNAME,FITFUNCNAME);
    pullHist->SetMarkerSize(0.7f);
    
    pullPlot->addPlotable(pullHist,"PSAME");
    
    setPullStyle(pullPlot);  
    pullPlot->Draw("same");
    TLine *l1 = new TLine(8.5,0,10.0,0.0);
    l1->SetLineColor(4);
    l1->Draw("same");

    drawPullText(pullHist,fitResults);

    return canvas;
}

void setGraphStyle(RooPlot* plot, const drawConfig* config)
{
  float massHigh = config->fitConf.massHigh;
  float massLow = config->fitConf.massLow;
  float div= (massHigh - massLow)/(config->nBins);

  plot->SetTitle("#varUpsilon(1S) MC ( 5.02 TeV)");
  plot->SetFillStyle(4000);
  plot->SetMarkerStyle(2);
  plot->SetMarkerSize(0.02);
  plot->SetAxisRange(massLow,massHigh);

  plot->GetXaxis()->SetLabelSize(0);
  plot->GetXaxis()->SetTitleSize(0);
  plot->GetXaxis()->SetRangeUser(massLow,massHigh);

  plot->GetYaxis()->SetTitleOffset(1.0);
  plot->GetYaxis()->CenterTitle();
  plot->GetYaxis()->SetTitleSize(0.048);
  plot->GetYaxis()->SetTitle( Form("Events / ( %.3f GeV/c^{2} )",div));
  plot->GetYaxis()->SetRangeUser(100,10000000);

  return;
}

void setPullStyle(RooPlot* pullPlot)
{
    pullPlot->SetTitleSize(0);
    pullPlot->SetMarkerStyle(2);
    pullPlot->SetMarkerSize(0.1);
    
    pullPlot->SetDrawOption("HIST");
    pullPlot->GetYaxis()->SetTitleOffset(0.31) ;
    pullPlot->GetYaxis()->SetTitle("Pull") ;
    pullPlot->GetYaxis()->SetTitleSize(0.10) ;
    pullPlot->GetYaxis()->SetLabelSize(0.05) ;
    pullPlot->GetYaxis()->SetRangeUser(-12,12) ;
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

TLegend* drawLegend(RooPlot* plot,bool bkgOn)
{
    TLegend* fitleg = new TLegend(0.70,0.65,0.85,0.85);
    fitleg->SetTextSize(12);
    fitleg->SetTextFont(43);
    fitleg->SetBorderSize(0);
    fitleg->AddEntry(plot->findObject(DATASETNAME),"Data","pe");
    fitleg->AddEntry(plot->findObject(FITFUNCNAME),"Total fit","l");
    fitleg->AddEntry(plot->findObject("dcb"),"Signal","l");
    if (bkgOn)
      fitleg->AddEntry(plot->findObject("bkg"),"Background","l");
    fitleg->AddEntry(plot->findObject("cb1"),"CBall 1","l");
    fitleg->AddEntry(plot->findObject("cb2"),"CBall 2","l");
    return fitleg;
}

void drawTexts(RooRealVar* var,RooAbsReal* fittedFunc,const drawConfig* config)
{
    TextDrawer tdrawer(0.22,0.8);
    RooArgSet* params= fittedFunc->getParameters(*var);

    float alpha =   params->getRealValue("alpha_1");
    float f =       params->getRealValue("f");
    float mean =    params->getRealValue("mean_1");
    float n =       params->getRealValue("n_1");
    float sigma =   params->getRealValue("sigma_1");
    float x =       params->getRealValue("x");

    tdrawer.drawText("#varUpsilon(1S) #rightarrow #mu#mu");
    tdrawer.drawText( Form("#alpha=%.3f",alpha));
    tdrawer.drawText( Form("m=%.3f",mean));
    tdrawer.drawText( Form("N=%.3f",n));
    tdrawer.drawText( Form("#sigma_{1}=%.4f, #frac{#sigma_{2}}{#sigma_{1}}=%.4f",sigma,x));
    tdrawer.drawText( Form("f=%.4f",f));

    float ptLow=config->fitConf.cut.ptLow;
    float ptHigh=config->fitConf.cut.ptHigh;
    float yLow=config->fitConf.cut.yLow;
    float yHigh=config->fitConf.cut.yHigh;
    float singleMuPtLow=config->cut.singleMuPtLow;
    float singleMuEtaHigh=config->cut.singleMuEtaHigh;

    TextDrawer tdrawer2(0.45,0.8);
    if (ptLow == 0.0f)
        tdrawer2.drawText(Form("p_{T}^{#mu#mu} < %.1f GeV/c", ptHigh));
    else 
        tdrawer2.drawText(Form("%.f < p_{T}^{#mu#mu} < %.1f GeV/c",ptLow, ptHigh));
    
    if (yLow == 0.0f)
        tdrawer2.drawText(Form("|y^{#mu#mu}| < %.2f", yHigh));
    else 
        tdrawer2.drawText(Form("%.2f < |y^{#mu#mu}| < %.2f",yLow, yHigh));
    tdrawer2.drawText(Form("p_{T}^{#mu} > %.1f GeV/c", singleMuPtLow));
    tdrawer2.drawText(Form("|#eta^{#mu}| < %.2f",singleMuEtaHigh));
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

    TLatex *tex = new TLatex(0.68,0.93,Form("#chi^{2}/ndf : %.1f / %d",chisq,ndf));
    tex->SetTextFont(43);
    tex->SetTextSize(12.0);
    tex->SetNDC();
    tex->Draw("same");
}