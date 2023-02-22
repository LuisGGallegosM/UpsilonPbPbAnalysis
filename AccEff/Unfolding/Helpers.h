#ifndef UNFOLDING_HELPERS
#define UNFOLDING_HELPERS

#include "TFile.h"
#include "TH1.h"
#include <string>
#include "TMatrixD.h"

const double zbins_low=0.0;
const double zbins_high=1.0;
const int zbins_n=4;

const double jtptbins_low=10.0;
const double jtptbins_high=40.0;
const int jtptbins_n = 3;

const double jtptbins_width= (jtptbins_high-jtptbins_low)/(jtptbins_n);

const std::vector<int> iterations={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,22,24,26,28,30,35,40,50,9999};

const bool v1d_jtpt=true;

void DrawHist(TH1* hist,const std::string& outputfilename);
void DrawHist2(TH1* hist1, TH1* hist2,const std::string& name);
void DrawHist(TMatrixD* hist,const std::string& name);

std::string generatePostfix(int i);

template<typename T>
T* Get( TFile* file ,const char* name)
    {
        TObject* obj=file->Get(name);
        if (obj==nullptr)
        {
            throw std::runtime_error(std::string("object '")+ name + "' dont exist");
        } 
        T* result= dynamic_cast<T*>(obj);
        if(result==nullptr)
        {
            throw std::runtime_error(std::string("object '")+ name + "' dont inherits from stated class");
        }
        return result;
    }

#endif