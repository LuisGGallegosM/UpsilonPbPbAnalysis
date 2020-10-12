#pragma once

#include "TH1.h"
#include "RooRealVar.h"
#include "RooCBShape.h"
#include "RooDataSet.h"
#include "RooGlobalFunc.h"
#include "RooFitResult.h"
#include "RooAddPdf.h"

using namespace RooFit;

#define S1MeanMass_max (9.69)
#define S1MeanMass (9.46)
#define S1MeanMass_min (9.26)

#define S1SigmaMass_max (0.6)
#define S1SigmaMass (0.1)
#define S1SigmaMass_min (0.001)

#define S1AlphaMass_max (8.5)
#define S1AlphaMass (2.522)
#define S1AlphaMass_min (0.1)

#define S1NMass_max (8.5)
#define S1NMass (1.705)
#define S1NMass_min (0.1)

class CrystalBall
{
    RooRealVar mean;
    RooRealVar sigma;
    RooRealVar alpha;
    RooRealVar n;
    RooCBShape cBall;

    public:

    CrystalBall(RooRealVar& var):
    mean("mean","mean of gaussian PDF",S1MeanMass,S1MeanMass_min,S1MeanMass_max),
    sigma("sigma","width of gaussian",S1SigmaMass, S1SigmaMass_min, S1SigmaMass_max),
    alpha("alpha","tail shift", S1AlphaMass,S1AlphaMass_min,S1AlphaMass_max),
    n("n","power order",S1NMass, S1NMass_min, S1NMass_max),
    cBall("cball","crystalBall",var,mean,sigma,alpha,n)
    {
    }

    RooCBShape* getCB()
    {
        return &cBall;
    }
};

class OniaMassFitter
{
    float massLow; 
    float massHigh;
    RooRealVar mass;
    RooRealVar coeff; 
    CrystalBall cball1;
    CrystalBall cball2;
    RooAddPdf dcball;
    RooDataSet dataset;

    public:
    OniaMassFitter(TTree* tree,float massLow_=8.5,float massHigh_=10.0):
    massLow(massLow_), massHigh(massHigh_),
    coeff("f1s","1S CB fraction", 0.5, 0.0, 1),
    mass("mass","onia mass",massLow,massHigh),
    dataset("mass dataset","mass dataset",tree,RooArgSet(mass)),
    cball1(mass),
    cball2(mass),
    dcball("dcb","double crystal ball", RooArgList(*(cball1.getCB()),*(cball2.getCB()) ),RooArgList(coeff) )
    {

    }

    TH1* fit()
    {
        RooFitResult* fitRes = dcball.fitTo(dataset,Save(), Hesse(kTRUE),Timer(kTRUE));
        TH1* result= dcball.createHistogram("mass_fit", mass);
        result->SetTitle("Invariant mass fit : double crystal ball");
        result->SetOption("C");
        return result;
    }

    TH1* getOriginalHist()
    {
        TH1* result = dataset.createHistogram("mass",mass);
        result->SetOption("E1");
        result->SetTitle("Invariant mass : skimmed data");
        return result;
    }
};