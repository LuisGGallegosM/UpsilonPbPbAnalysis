#ifndef BASECANVAS
#define BASECANVAS

#include"TH2.h"
#include"TEfficiency.h"

const float canvasWidth = 1200.0f;
const float canvasHeight = 900.0f;
const float padSizes[] = { 0.02f,0.1f,0.98f,0.9f };

TH2F* createTH2QQ(const std::string& name,const std::string& title);
TH2F* createTH2Mu(const std::string& name,const std::string& title);
TH1F* createTH1(const std::string& name,const std::string& title);
std::unique_ptr<TEfficiency> createTEff(TH1* num, TH1* den, const std::string& name, const std::string& title);

void writeToCanvas(TH2* hist,const std::string& xname,const std::string& yname, const std::string& outname);
void writeToCanvas(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname, bool yLog=false);
void writeToCanvasEff(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname);
void writeToCanvasEff2D(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname);

void writeToCanvas(std::vector<TH1*>& hists,const std::string& title,const std::string& xname,const std::string& yname, const std::string& outname, bool yLog=false);

#endif