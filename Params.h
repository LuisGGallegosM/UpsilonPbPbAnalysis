#ifndef PARAMS
#define PARAMS

#include <iostream>
#include <fstream>
#include "TTree.h"

struct dcbParam
{
    float mean;
    float alpha;
    float n;
    float sigma1;
    float x;
    float f;
};

struct fitParams
{
    dcbParam dcb;
    float nSig;
    float nBkg;
    float chk4_k1;
    float chk4_k2;
};

struct cutParams
{
    bool isMC;
    bool checkSign;

    ULong64_t trigSelect;
    int sign;

    int selectionBits;
    int minTracks;
    int minPixels;
    float maxDxy;
    float maxDz;
    float minVtxProb;

    float ptLow;
    float ptHigh;
    float yLow;
    float yHigh;
    float singleMuPtLow;
    float singleMuEtaHigh;
};

struct fitConfig
{
    bool bkgOn;
    float massLow;
    float massHigh;
    fitParams initialValues;
};

struct drawConfig
{
    int nBins;
    const cutParams* cut;
    const fitConfig* fitConf;
};

void serialize(std::ofstream& file,const char* name,float value);
void serialize(std::ofstream& file,const char* name,bool value);
void serialize(std::ofstream& file,const char* name,int value);
void serialize(std::ofstream& file,const char* name,ULong64_t value);
#endif