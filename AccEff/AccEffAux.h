#ifndef ACCEFFAUX
#define ACCEFFAUX

#include"TH2.h"
#include"TEfficiency.h"

TH2F* createTH2QQ(const std::string& name,const std::string& title);
TH2F* createTH2Mu(const std::string& name,const std::string& title);
TH1F* createTH1(const std::string& name,const std::string& title);
std::unique_ptr<TEfficiency> createTEff(TH1* num, TH1* den, const std::string& name);

void writeToCanvas(TH2* hist,const std::string& xname,const std::string& yname, const std::string& outname);
void writeToCanvas(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname);
void writeToCanvasEff(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname);
void writeToCanvasEff2D(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname);

#endif