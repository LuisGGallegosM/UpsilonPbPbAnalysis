
#include "Drawing.h"

using namespace RooFit;

void drawGraphText(const fitParamsWithErrors* fParams,const drawConfig* config);
void drawPullText(RooHist* hist, RooFitResult* fitResults);

TLegend* drawLegend(RooPlot* plot, bool bkgOn,bool moreUpsilon);

RooPlot* drawGraphs(RooRealVar* var, RooDataSet* dataset, RooAbsPdf* fittedFunc,const drawConfig* config);
RooHist*  drawPull(RooPlot* plot, RooRealVar* var, RooFitResult* fitResults,const drawConfig* config);

void Drawing(const char* filename,const char* drawfilename, const char* configfilename, const char* cutfilename, const char* fitfilename)
{
    const std::string fitresultfilename = ReplaceExtension(fitfilename,".fit");

    std::cout << "\nDRAWING\n";
    std::cout << "Reading input file: " << filename <<'\n';
    std::cout << "Drawing output file: " << drawfilename <<'\n';
    std::cout << "Reading draw configuration file: " << configfilename <<'\n';
    std::cout << "Reading cut configuration file: " << cutfilename <<'\n';
    std::cout << "Reading fit configuration file: " << fitfilename <<'\n';
    std::cout << "Reading fit result file:" << fitresultfilename  << '\n';

    drawConfig config;
    config.deserialize(configfilename,cutfilename,fitfilename);

    if (!config.isValid())
    {
        std::cerr << "Error: Invalid arguments\n";
        return;
    }

    fitParamsWithErrors fParams;
    fParams.deserialize(fitresultfilename);

    if (!fParams.isValid())
    {
        std::cerr << "Error: incorrect fit result file\n" ;
        return;
    }

    TFile file(filename,"UPDATE");

    if (file.IsZombie())
    {
        std::cerr << "Error: File " << filename << " not found.\n";
        return;
    }

    CopyFile(configfilename, ReplaceExtension(drawfilename,".drawconf").data());

    RooRealVar* massVar = (RooRealVar*) (file.Get("mass"));
    RooAbsPdf* fittedFunc = (RooAbsPdf*) (file.Get("dcb_fit"));
    RooDataSet* dataset = (RooDataSet*) (file.Get("dataset"));
    RooFitResult* fitResults = (RooFitResult*) (file.Get("fitresults"));

    if ((massVar==nullptr) || (fittedFunc==nullptr) || (dataset==nullptr) || (fitResults==nullptr))
    {
        std::cerr << "Error: Needed data not found in file\n" ;
        return;
    }

    //DRAWING START
    for (bool isLog : { false,true })
    {
        TCanvas* canvas = getStyledCanvas();
        if (canvas==nullptr)
        {
            std::cerr << "Error: Canvas not generated\n";
            return;
        }

        canvas->cd();
        TPad* graph = getStyledGraphPad(isLog);
        TPad* pull = getStyledPullPad();

        graph->cd();
        RooPlot* graphPlot =drawGraphs(massVar,dataset,fittedFunc,&config);
        float maxVal=fParams.getNSigY1S()/( fParams.getSigmaY1S() * 1.4142*1.7724 );
        float minVal=fParams.getNBkg()/( config.fitConf.getMassHigh() - config.fitConf.getMassLow() );
        setGraphStyle(graphPlot,&config,maxVal,minVal,isLog);
        drawLegend(graphPlot,config.fitConf.isBkgOn(),config.fitConf.isMoreUpsilon());
        drawGraphText(&fParams,&config);

        pull->cd();
        RooHist* pullHist = drawPull(graphPlot,massVar,fitResults,&config);
        drawPullText(pullHist,fitResults);

        canvas->Update();
        canvas->Write(0,TObject::kOverwrite);
        if (isLog)
            canvas->SaveAs(ReplaceExtension(drawfilename,"_log.pdf").data());
        else
            canvas->SaveAs(drawfilename);
    }

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

RooPlot* drawGraphs(RooRealVar* var, RooDataSet* dataset, RooAbsPdf* fittedFunc,const drawConfig* config)
{
        //draw fitted and dataset function
    RooPlot* plot = var->frame(config->nBins);
    fittedFunc->plotOn(plot,Name(FITFUNCNAME),LineColor(kOrange+7),Normalization(1.0,RooAbsReal::RelativeExpected));

    if (config->fitConf.isMoreUpsilon())
    {
        fittedFunc->plotOn(plot,Name("dcb1"),Components("dcb_Y1S"),LineStyle(7),LineColor(kGreen-2),Normalization(1.0,RooAbsReal::RelativeExpected));
        fittedFunc->plotOn(plot,Name("dcb2"),Components("dcb_Y2S"),LineStyle(7),LineColor(kMagenta+1),Normalization(1.0,RooAbsReal::RelativeExpected));
        fittedFunc->plotOn(plot,Name("dcb3"),Components("dcb_Y3S"),LineStyle(7),LineColor(kMagenta+2),Normalization(1.0,RooAbsReal::RelativeExpected));
    }
    else
    {
        fittedFunc->plotOn(plot,Name("dcb"),Components("dcb_Y1S"),LineStyle(9),LineColor(13),Normalization(1.0,RooAbsReal::RelativeExpected));
        fittedFunc->plotOn(plot,Name("cb1"),Components("cball_Y1S_1"),LineStyle(7),LineColor(kGreen-2),Normalization(1.0,RooAbsReal::RelativeExpected));
        fittedFunc->plotOn(plot,Name("cb2"),Components("cball_Y1S_2"),LineStyle(7),LineColor(kMagenta+1),Normalization(1.0,RooAbsReal::RelativeExpected));
    }
    
    if (config->fitConf.isBkgOn())
      fittedFunc->plotOn(plot,Name("bkg"),Components("bkg"),LineStyle(kDashed),LineColor(kBlue),Normalization(1.0,RooAbsReal::RelativeExpected));
    dataset->plotOn(plot,Name(DATASETNAME), MarkerSize(0.4), XErrorSize(0));
    plot->Draw("same");

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

    fitleg->Draw("same");

    return fitleg;
}

void drawGraphText(const fitParamsWithErrors* fParams,const drawConfig* config)
{
    TextDrawer tdrawer(0.22f,0.8f,9.0f);
    
    tdrawer.drawText("#varUpsilon(1S) #rightarrow #mu#mu");
    tdrawer.drawText( Form("#alpha=%.3f #pm %.3f",fParams->getAlpha(),fParams->getErrors().getAlpha()));
    tdrawer.drawText( Form("m_{Y1S}=%.3f #pm %.3f",fParams->getMeanY1S(),fParams->getErrors().getMeanY1S()));
    tdrawer.drawText( Form("m_{Y2S}=%.3f #pm %.3f",fParams->getMeanY2S(),fParams->getErrors().getMeanY2S()));
    tdrawer.drawText( Form("m_{Y3S}=%.3f #pm %.3f",fParams->getMeanY3S(),fParams->getErrors().getMeanY3S()));
    tdrawer.drawText( Form("n=%.3f #pm %.3f",fParams->getN(),fParams->getErrors().getN()));
    tdrawer.drawText( Form("#sigma_{1}^{Y1S}=%.4f #pm %.4f",fParams->getSigmaY1S(),fParams->getErrors().getSigmaY1S()));
    tdrawer.drawText( Form("#sigma_{1}^{Y2S}=%.4f #pm %.4f",fParams->getSigmaY2S(),fParams->getErrors().getSigmaY2S()));
    tdrawer.drawText( Form("#sigma_{1}^{Y3S}=%.4f #pm %.4f",fParams->getSigmaY3S(),fParams->getErrors().getSigmaY3S()));
    tdrawer.drawText( Form("#frac{#sigma_{2}}{#sigma_{1}}=%.4f #pm %.4f",fParams->getX(),fParams->getErrors().getX()));
    tdrawer.drawText( Form("f=%.4f #pm %.4f",fParams->getF(),fParams->getErrors().getF()));

    float ptLow=config->fitConf.getCut()->getPtLow();
    float ptHigh=config->fitConf.getCut()->getPtHigh();
    float yLow=config->fitConf.getCut()->getYLow();
    float yHigh=config->fitConf.getCut()->getYHigh();
    float singleMuPtLow=config->cut.getMuPtLow();
    float singleMuEtaHigh=config->cut.getMuEtaHigh();

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
    TLine *l1 = new TLine(config->fitConf.getMassLow(),0,config->fitConf.getMassHigh(),0.0);
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