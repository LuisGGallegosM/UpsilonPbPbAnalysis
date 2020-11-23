
#include "Drawing.h"

using namespace RooFit;

void drawGraphText(const fitParams* fParams,const drawConfig* config);
void drawPullText(RooHist* hist, RooFitResult* fitResults);

TLegend* drawLegend(RooPlot* plot, bool bkgOn,bool moreUpsilon);

RooPlot* drawGraphs(RooRealVar* var, RooDataSet* dataset, RooAbsReal* fittedFunc,const drawConfig* config);
RooHist*  drawPull(RooPlot* plot, RooRealVar* var, RooFitResult* fitResults,const drawConfig* config);

void Drawing(const char* filename,const char* drawfilename, const char* configfilename, const char* cutfilename, const char* fitfilename)
{
    std::cout << "\nDRAWING\n";
    std::cout << "Reading input file: " << filename <<'\n';
    std::cout << "Drawing output file: " << drawfilename <<'\n';
    std::cout << "Reading draw configuration file: " << configfilename <<'\n';
    std::cout << "Reading cut configuration file: " << cutfilename <<'\n';
    std::cout << "Reading fit configuration file: " << fitfilename <<'\n';

    TFile file(filename,"UPDATE");

    if (file.IsZombie())
    {
        std::cerr << "Error: File " << filename << " not found.\n";
        return;
    }
    drawConfig config;
    config.deserialize(configfilename,cutfilename,fitfilename);

    if (!config.isValid())
    {
        std::cerr << "Error: Invalid arguments\n";
        return;
    }

    CopyFile(configfilename, ReplaceExtension(drawfilename,".drawconf").data());

    RooRealVar* massVar = (RooRealVar*) (file.Get("mass"));
    RooAbsReal* fittedFunc = (RooAbsReal*) (file.Get("dcb_fit"));
    RooDataSet* dataset = (RooDataSet*) (file.Get("dataset"));
    RooFitResult* fitResults = (RooFitResult*) (file.Get("fitresults"));

    if ((massVar==nullptr) || (fittedFunc==nullptr) || (dataset==nullptr) || (fitResults==nullptr))
    {
        std::cerr << "Error: Needed data not found in file\n" ;
        return;
    }

    fitParams fParams;
    fParams.deserialize(ReplaceExtension(fitfilename,".fit"));

    //DRAWING START

    TCanvas* canvas = getStyledCanvas();
    canvas->cd();
    TPad* graph = getStyledGraphPad(config.isLog);
    TPad* pull = getStyledPullPad();

    graph->cd();
    RooPlot* graphPlot =drawGraphs(massVar,dataset,fittedFunc,&config);
    drawGraphText(&fParams,&config);

    pull->cd();
    RooHist* pullHist = drawPull(graphPlot,massVar,fitResults,&config);
    drawPullText(pullHist,fitResults);

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
    std::string option(argv[1]);

    if (option=="-m")
    {
        //enter in multicomparation draw mode
        int numFiles=(argc -3) ;
        const char* args[32];

        if (numFiles<=0) return 0;
        
        for (int i=0;i<numFiles;i++) args[i]=argv[i+3];

        DrawingCmp(argv[2],numFiles,args);
    }
    else
    {
        //normal mode draw
        if (argc ==6)
            Drawing(argv[1],argv[2],argv[3],argv[4],argv[5]);
        else
        {
            std::cerr << "Error: Incorrect number of parameters\n";  
        }
    }

    return 0;
}

#endif


//DRAW GRAPH

RooPlot* drawGraphs(RooRealVar* var, RooDataSet* dataset, RooAbsReal* fittedFunc,const drawConfig* config)
{
        //draw fitted and dataset function
    RooPlot* plot = var->frame(config->nBins);
    fittedFunc->plotOn(plot,Name(FITFUNCNAME),LineColor(kOrange+7),Normalization(1.0,RooAbsReal::RelativeExpected));

    if (config->fitConf.moreUpsilon)
    {
        fittedFunc->plotOn(plot,Name("dcb1"),Components("dcb_Y1S,bkg"),LineStyle(7),LineColor(kGreen-2),Normalization(1.0,RooAbsReal::RelativeExpected));
        fittedFunc->plotOn(plot,Name("dcb2"),Components("dcb_Y2S,bkg"),LineStyle(7),LineColor(kMagenta+1),Normalization(1.0,RooAbsReal::RelativeExpected));
        fittedFunc->plotOn(plot,Name("dcb3"),Components("dcb_Y3S,bkg"),LineStyle(7),LineColor(kMagenta+2),Normalization(1.0,RooAbsReal::RelativeExpected));
    }
    else
    {
        fittedFunc->plotOn(plot,Name("dcb"),Components("dcb_Y1S"),LineStyle(9),LineColor(13),Normalization(1.0,RooAbsReal::RelativeExpected));
        fittedFunc->plotOn(plot,Name("cb1"),Components("cball_Y1S_1"),LineStyle(7),LineColor(kGreen-2),Normalization(1.0,RooAbsReal::RelativeExpected));
        fittedFunc->plotOn(plot,Name("cb2"),Components("cball_Y1S_2"),LineStyle(7),LineColor(kMagenta+1),Normalization(1.0,RooAbsReal::RelativeExpected));
    }
    
    
    if (config->fitConf.bkgOn)
      fittedFunc->plotOn(plot,Name("bkg"),Components("bkg"),LineStyle(kDashed),LineColor(kBlue),Normalization(1.0,RooAbsReal::RelativeExpected));
    dataset->plotOn(plot,Name(DATASETNAME), MarkerSize(0.4), XErrorSize(0));
    TLegend* legend= drawLegend(plot,config->fitConf.bkgOn,config->fitConf.moreUpsilon);

    setGraphStyle(plot,config);
    plot->Draw("same");
    legend->Draw("same");

    return plot;
}

TLegend* drawLegend(RooPlot* plot,bool bkgOn,bool moreUpsilon)
{
    TLegend* fitleg = new TLegend(0.70,0.65,0.85,0.85);
    fitleg->SetTextSize(12);
    fitleg->SetTextFont(43);
    fitleg->SetBorderSize(0);
    fitleg->AddEntry(plot->findObject(DATASETNAME),"Data","pe");
    fitleg->AddEntry(plot->findObject(FITFUNCNAME),"Total fit","l");
    if(moreUpsilon)
    {
        fitleg->AddEntry(plot->findObject("dcb1"),"Signal Y1S","l");
        fitleg->AddEntry(plot->findObject("dcb2"),"Signal Y2S","l");
        fitleg->AddEntry(plot->findObject("dcb3"),"Signal Y3S","l");
    }
    else
    {
        fitleg->AddEntry(plot->findObject("dcb"),"Signal","l");
        fitleg->AddEntry(plot->findObject("cb1"),"CBall 1","l");
        fitleg->AddEntry(plot->findObject("cb2"),"CBall 2","l");
    }
    

    if (bkgOn)
      fitleg->AddEntry(plot->findObject("bkg"),"Background","l");

    return fitleg;
}

void drawGraphText(const fitParams* fParams,const drawConfig* config)
{
    TextDrawer tdrawer(0.22,0.8);
    
    tdrawer.drawText("#varUpsilon(1S) #rightarrow #mu#mu");
    tdrawer.drawText( Form("#alpha=%.3f",fParams->getDCBParams()->getAlpha()));
    tdrawer.drawText( Form("m=%.3f",fParams->getDCBParams()->getMean()));
    tdrawer.drawText( Form("N=%.3f",fParams->getDCBParams()->getN()));
    tdrawer.drawText( Form("#sigma_{1}=%.4f",fParams->getDCBParams()->getSigma()));
    tdrawer.drawText( Form("#frac{#sigma_{2}}{#sigma_{1}}=%.4f",fParams->getDCBParams()->getX()));
    tdrawer.drawText( Form("f=%.4f",fParams->getDCBParams()->getF()));

    float ptLow=config->fitConf.cut.getPtLow();
    float ptHigh=config->fitConf.cut.getPtHigh();
    float yLow=config->fitConf.cut.getYLow();
    float yHigh=config->fitConf.cut.getYHigh();
    float singleMuPtLow=config->cut.singleMuPtLow;
    float singleMuEtaHigh=config->cut.singleMuEtaHigh;

    TextDrawer tdrawer2(0.45,0.8);
    if (ptLow == 0.0f)
        tdrawer2.drawText(Form("p_{T}^{#mu#mu} < %.1f GeV/c", ptHigh));
    else 
        tdrawer2.drawText(Form("%.1f < p_{T}^{#mu#mu} < %.1f GeV/c",ptLow, ptHigh));
    
    if (yLow == 0.0f)
        tdrawer2.drawText(Form("|y^{#mu#mu}| < %.2f", yHigh));
    else 
        tdrawer2.drawText(Form("%.2f < |y^{#mu#mu}| < %.2f",yLow, yHigh));
    tdrawer2.drawText(Form("p_{T}^{#mu} > %.1f GeV/c", singleMuPtLow));
    tdrawer2.drawText(Form("|#eta^{#mu}| < %.2f",singleMuEtaHigh));
}
//DRAW PULL

RooHist* drawPull(RooPlot* plot, RooRealVar* var, RooFitResult* fitResults,const drawConfig* config)
{
    //draw pull
    RooPlot* pullPlot = var->frame(Title("Pull Distribution"));
    RooHist* pullHist = plot->pullHist(DATASETNAME,FITFUNCNAME);
    pullHist->SetMarkerSize(0.7f);
    
    pullPlot->addPlotable(pullHist,"PSAME");
    
    setPullStyle(pullPlot,config);  
    pullPlot->Draw("same");
    TLine *l1 = new TLine(config->fitConf.massLow,0,config->fitConf.massHigh,0.0);
    l1->SetLineColor(4);
    l1->Draw("same");
    return pullHist;
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