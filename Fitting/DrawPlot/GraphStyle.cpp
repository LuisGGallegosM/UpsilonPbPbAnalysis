

#include "GraphStyle.h"
#include "TH1.h"

void setGraphStyle(RooPlot* plot, const ParameterGroup* config, float topValue, float bottomValue, bool isLog)
{
    float massHigh = config->getFloat("cut.mass.high");
    float massLow = config->getFloat("cut.mass.low");
    float div= (massHigh - massLow)/(config->getInt("nBins"));
    float maxValue = topValue*div;
    float minValue = bottomValue*div;

    //round to 
    maxValue = ceil(maxValue/10.0f)*10.0+10.0f;
    minValue = ceil(minValue);
    if (isLog)
    {
        minValue *= config->getFloat("log.minValMultiplier");
        maxValue *= config->getFloat("log.maxValMultiplier");
    }
    else
    {
        minValue = 0.0f;
        maxValue *=config->getFloat("linear.maxValMultiplier");
    }

    if (config->getBool("isMC"))
        plot->SetTitle(" ");
    else
        plot->SetTitle(" ");
    plot->SetFillStyle(4000);
    plot->SetMarkerStyle(2);
    plot->SetMarkerSize(0.01);
    plot->SetAxisRange(massLow,massHigh);

    plot->GetXaxis()->SetLabelSize(0);
    plot->GetXaxis()->SetTitleSize(0);
    plot->GetXaxis()->SetRangeUser(massLow,massHigh);

    plot->GetYaxis()->SetTitleOffset(1.0);
    plot->GetYaxis()->CenterTitle();
    plot->GetYaxis()->SetTitleSize(0.048);
    plot->GetYaxis()->SetTitle( Form("Events / ( %.3f GeV/c^{2} )",div));
    plot->GetYaxis()->SetRangeUser(minValue,maxValue);

  return;
}

void setPullStyle(RooPlot* pullPlot, const ParameterGroup* config)
{
    pullPlot->SetTitleSize(0);
    pullPlot->SetMarkerStyle(2);
    pullPlot->SetMarkerSize(0.1);
    
    pullPlot->SetDrawOption("HIST");
    pullPlot->GetYaxis()->SetTitleOffset(0.31) ;
    pullPlot->GetYaxis()->SetTitle("Pull") ;
    pullPlot->GetYaxis()->SetTitleSize(0.07) ;
    pullPlot->GetYaxis()->SetLabelSize(0.05) ;
    pullPlot->GetYaxis()->SetRangeUser(-8,8) ;
    pullPlot->GetYaxis()->CenterTitle();
    pullPlot->GetYaxis()->SetTickSize(0.02);
    pullPlot->GetYaxis()->SetNdivisions(505);

    pullPlot->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} (GeV/c^{2})");
    pullPlot->GetXaxis()->SetTitleOffset(1.30) ;
    pullPlot->GetXaxis()->SetLabelOffset(0.03) ;
    pullPlot->GetXaxis()->SetLabelSize(0.06) ;
    pullPlot->GetXaxis()->SetTitleSize(0.06) ;
    pullPlot->GetXaxis()->CenterTitle();
    pullPlot->GetXaxis()->SetRangeUser(config->getFloat("cut.mass.low"),config->getFloat("cut.mass.high"));
    pullPlot->GetXaxis()->SetTickSize(0.03);
    return;
}

TPad* getStyledGraphPad(bool isLog)
{
    TPad* graph = new TPad("graph_pad","mass fit", 0, 0.45, 0.98, 1.0);
    graph->SetTicks(1,1);
    graph->SetLeftMargin(0.15);
    if (isLog)
        graph->SetLogy();
    graph->Draw();
    return graph;
}

TCanvas* getStyledCanvas()
{
    TCanvas* canvas = new TCanvas("Fit_plot","mass fit",4,45,550,520);
    canvas->SetLeftMargin(2.6);
    return canvas;
}

TPad* getStyledPullPad()
{
    TPad* pull = new TPad("pull_pad","mass fit pull", 0.00, 0.00, 0.98, 0.5);
    pull->SetTopMargin(0); // Upper and lower plot are joined
    pull->SetBottomMargin(0.63); 
    pull->SetLeftMargin(0.15);
    pull->SetTicks(1,1);
    pull->Draw();
    return pull;
}