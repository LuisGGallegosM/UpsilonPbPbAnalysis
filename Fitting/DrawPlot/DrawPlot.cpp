
#include "DrawPlot.h"

using namespace RooFit;

void drawGraphText(const fitParamsWithErrors* fParams,const drawConfig* config);
void drawPullText(RooHist* hist, RooFitResult* fitResults);

TLegend* drawLegend(RooPlot* plot, bool bkgOn,bool moreUpsilon);

RooPlot* drawGraphs(RooRealVar* var, RooDataSet* dataset, RooAbsPdf* fittedFunc,const drawConfig* config);
RooHist*  drawPull(RooPlot* plot, RooRealVar* var, const drawConfig* config);

/**
 * @brief Generate the plots for a Fit output
 * 
 * @param inputdirectoryname path to the directory where fit results saved.
 * @param drawconfigfilename path to the draw configuration file.
 */
void DrawPlot(const char* inputdirectoryname, const char* drawconfigfilename  )
{
    const std::string inputdir = inputdirectoryname;

    //inputs reading from directory with same name as directory but with corresponding extension.
    const std::string filename = generateNames(inputdir,".root");
    const std::string fitresultfilename = generateNames(inputdir,".fit");
    const std::string cutfilename = generateNames(inputdir,".cutconf");
    const std::string fitfilename = generateNames(inputdir,".fitconf");

    //output pdf file draw placed in the same input directory.
    const std::string drawfilename =generateNames(inputdir,".pdf");

    std::cout << "\nDRAWING\n";
    std::cout << "Reading draw configuration file: " << drawconfigfilename <<'\n';
    std::cout << "Reading root input file: " << filename <<'\n';
    std::cout << "Reading cut configuration file: " << cutfilename <<'\n';
    std::cout << "Reading fit configuration file: " << fitfilename <<'\n';
    std::cout << "Reading fit result file:" << fitresultfilename  << '\n';

    std::cout << "Drawing output file: " << drawfilename <<'\n';

    //read draw configuration file (.drawconf)
    drawConfig config;
    config.deserialize(drawconfigfilename,cutfilename,fitfilename);

    if (!config.isValid())
    {
        std::cerr << "Error: Invalid arguments\n";
        return;
    }

    //read fit result file (.fit)
    fitParamsWithErrors fParams;
    fParams.deserialize(fitresultfilename);

    if (!fParams.isValid())
    {
        std::cerr << "Error: incorrect fit result file\n" ;
        return;
    }

    //read root result file (.root)
    TFile* fittedfile = OpenFile(filename.data(),"READ");

    RooRealVar* massVar = (RooRealVar*) (fittedfile->Get("mass"));
    RooAbsPdf* fittedFunc = (RooAbsPdf*) (fittedfile->Get("dcb_fit"));
    RooDataSet* dataset = (RooDataSet*) (fittedfile->Get("dataset"));
    RooFitResult* fitResults = (RooFitResult*) (fittedfile->Get("fitresults"));

    if ((massVar==nullptr) || (fittedFunc==nullptr) || (dataset==nullptr) || (fitResults==nullptr))
    {
        std::cerr << "Error: Needed data not found in file\n" ;
        return;
    }

    //copy draw configuration used to the input/output directory
    CopyFile(drawconfigfilename, generateNames(inputdir,".drawconf").data());

    //DRAWING START, draw two plots, one linear scale and one log scale
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
        RooHist* pullHist = drawPull(graphPlot,massVar,&config);
        drawPullText(pullHist,fitResults);

        //output to pdf file, log scale file with _log.pdf suffix
        canvas->Update();
        if (isLog)
            canvas->SaveAs(generateNames(inputdir,"_log.pdf").data());
        else
            canvas->SaveAs(drawfilename.data());
    }

    return;
}

/**
 * @brief Draw the main plot
 * 
 * @param var variable for x axis (mass)
 * @param dataset dataset to draw in plot
 * @param fittedFunc result fit function to draw
 * @param config draw configuration
 * @return RooPlot* result plot
 */
RooPlot* drawGraphs(RooRealVar* var, RooDataSet* dataset, RooAbsPdf* fittedFunc,const drawConfig* config)
{
    RooPlot* plot = var->frame(config->nBins);
    fittedFunc->plotOn(plot,Name(fitFuncName),LineColor(kOrange+7),Normalization(1.0,RooAbsReal::RelativeExpected));

    if (config->fitConf.isMoreUpsilon())
    {
        //draw 1S 2S and 3S Upsilon mass
        fittedFunc->plotOn(plot,Name("dcb1"),Components("dcb_Y1S"),LineStyle(7),LineColor(kGreen-2),Normalization(1.0,RooAbsReal::RelativeExpected));
        fittedFunc->plotOn(plot,Name("dcb2"),Components("dcb_Y2S"),LineStyle(7),LineColor(kMagenta+1),Normalization(1.0,RooAbsReal::RelativeExpected));
        fittedFunc->plotOn(plot,Name("dcb3"),Components("dcb_Y3S"),LineStyle(7),LineColor(kMagenta+2),Normalization(1.0,RooAbsReal::RelativeExpected));
    }
    else
    {
        //draw just 1S Upsilon with corresponding crystal balls
        fittedFunc->plotOn(plot,Name("dcb"),Components("dcb_Y1S"),LineStyle(9),LineColor(13),Normalization(1.0,RooAbsReal::RelativeExpected));
        fittedFunc->plotOn(plot,Name("cb1"),Components("cball_Y1S_1"),LineStyle(7),LineColor(kGreen-2),Normalization(1.0,RooAbsReal::RelativeExpected));
        fittedFunc->plotOn(plot,Name("cb2"),Components("cball_Y1S_2"),LineStyle(7),LineColor(kMagenta+1),Normalization(1.0,RooAbsReal::RelativeExpected));
    }
    
    //draw background
    if (config->fitConf.isBkgOn())
      fittedFunc->plotOn(plot,Name("bkg"),Components("bkg"),LineStyle(kDashed),LineColor(kBlue),Normalization(1.0,RooAbsReal::RelativeExpected));
    dataset->plotOn(plot,Name(datasetName), MarkerSize(0.4), XErrorSize(0));
    plot->Draw("same");

    return plot;
}

/**
 * @brief Draw plot legend
 * 
 * @param plot rooPlot from where to draw legend
 * @param bkgOn is there any background signal?
 * @param moreUpsilon is there more than 1S upsilon?
 * @return TLegend* 
 */
TLegend* drawLegend(RooPlot* plot,bool bkgOn,bool moreUpsilon)
{
    TLegend* fitleg = new TLegend(0.70,0.65,0.85,0.85);
    fitleg->SetTextSize(12);
    fitleg->SetTextFont(43);
    fitleg->SetBorderSize(0);
    fitleg->AddEntry(plot->findObject(datasetName),"Data","pe");
    fitleg->AddEntry(plot->findObject(fitFuncName),"Total fit","l");
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

/**
 * @brief Draw the texts over the plots corresponding to fit parameters and cuts used.
 * 
 * @param fParams Fit parameters results of the fit.
 * @param config Draw configurations.
 */
void drawGraphText(const fitParamsWithErrors* fParams,const drawConfig* config)
{
    TextDrawer tdrawer(0.22f,0.8f,9.0f);
    
    tdrawer.drawText("#varUpsilon(1S) #rightarrow #mu#mu");
    tdrawer.drawText( Form("m_{Y1S}=%.3f #pm %.3f",fParams->getMeanY1S(),fParams->getErrors().getMeanY1S()));
    if (!config->fitConf.getFixAlpha())
        tdrawer.drawText( Form("#alpha=%.3f #pm %.3f",fParams->getAlpha(),fParams->getErrors().getAlpha()));
    if (!config->fitConf.getFixN())
        tdrawer.drawText( Form("n=%.3f #pm %.3f",fParams->getN(),fParams->getErrors().getN()));
    tdrawer.drawText( Form("#sigma_{1}^{Y1S}=%.4f #pm %.4f",fParams->getSigmaY1S(),fParams->getErrors().getSigmaY1S()));
    tdrawer.drawText( Form("#frac{#sigma_{2}}{#sigma_{1}}=%.4f #pm %.4f",fParams->getX(),fParams->getErrors().getX()));
    tdrawer.drawText( Form("f=%.4f #pm %.4f",fParams->getF(),fParams->getErrors().getF()));
    
    if (config->fitConf.getBkgType() == BkgParams::BkgType::chev)
    {
        tdrawer.drawText( Form("chk_k1=%.4f #pm %.4f",fParams->getChk4_k1(),fParams->getErrors().getChk4_k1()));
        tdrawer.drawText( Form("chk_k2=%.4f #pm %.4f",fParams->getChk4_k2(),fParams->getErrors().getChk4_k2()));
    }

    float ptLow=config->fitConf.getCut()->getPtLow();
    float ptHigh=config->fitConf.getCut()->getPtHigh();
    float yLow=config->fitConf.getCut()->getYLow();
    float yHigh=config->fitConf.getCut()->getYHigh();
    float singleMuPtLow=config->cut.getMuPtLow();
    float singleMuEtaHigh=config->cut.getMuEtaHigh();

    TextDrawer tdrawer2(0.45,0.8);
    
    tdrawer2.drawText(Form("%.2f < p_{T}^{#mu#mu} < %.2f",ptLow, ptHigh));
    
    if (yLow == 0.0f)
        tdrawer2.drawText(Form("|y^{#mu#mu}| < %.2f", yHigh));
    else 
        tdrawer2.drawText(Form("%.2f < |y^{#mu#mu}| < %.2f",yLow, yHigh));
    tdrawer2.drawText(Form("p_{T}^{#mu} > %.1f GeV/c", singleMuPtLow));
    tdrawer2.drawText(Form("|#eta^{#mu}| < %.2f",singleMuEtaHigh));
    //tdrawer2.drawText(" 25.0 < jet p_{T} < 35.0");
    //tdrawer2.drawText(" |jet #eta| < 2.0");
}

/**
 * @brief Draw the pull
 * 
 * @param plot plot where to get the pull
 * @param var variable used as x (mass variable)
 * @param config draw configuration.
 * @return RooHist* with output plot
 */
RooHist* drawPull(RooPlot* plot, RooRealVar* var,const drawConfig* config)
{
    //draw pull
    RooPlot* pullPlot = var->frame(Title("Pull Distribution"));
    RooHist* pullHist = plot->pullHist(datasetName,fitFuncName);
    pullHist->SetMarkerSize(0.7f);
    
    pullPlot->addPlotable(pullHist,"PSAME");
    
    setPullStyle(pullPlot,config);  
    pullPlot->Draw("same");
    TLine *l1 = new TLine(config->fitConf.getMassLow(),0,config->fitConf.getMassHigh(),0.0);
    l1->SetLineColor(4);
    l1->Draw("same");
    return pullHist;
}

/**
 * @brief Draw the text over the pull (Chi-sqr and DOF)
 * 
 * @param hist plot where to get the pull
 * @param fitResults RooFitResult object to get DOF info.
 */
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