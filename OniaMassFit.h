#pragma once

#include <string>

#include "TH1.h"
#include "TTree.h"
#include "RooRealVar.h"
#include "RooCBShape.h"
#include "RooDataSet.h"
#include "RooGlobalFunc.h"
#include "RooFitResult.h"
#include "RooAddPdf.h"
#include "RooPlot.h"

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

    CrystalBall(RooRealVar& var, const char* name):
    mean(Form("mean_%s",name),"mean of gaussian PDF",S1MeanMass,S1MeanMass_min,S1MeanMass_max),
    sigma(Form("sigma_%s",name),"width of gaussian",S1SigmaMass, S1SigmaMass_min, S1SigmaMass_max),
    alpha(Form("alpha_%s",name),"tail shift", S1AlphaMass,S1AlphaMass_min,S1AlphaMass_max),
    n(Form("n_%s",name),"power order",S1NMass, S1NMass_min, S1NMass_max),
    cBall(Form("cball_%s",name),"crystalBall",var,mean,sigma,alpha,n)
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
    TTree* tree;
    RooRealVar mass;
    RooRealVar coeff1;
    RooRealVar coeff2;
    CrystalBall cball1;
    CrystalBall cball2;
    RooAddPdf dcball;
    RooDataSet dataset;

    public:
    OniaMassFitter(TTree* tree_,float massLow_=8.5,float massHigh_=10.0):
    massLow(massLow_), massHigh(massHigh_),tree(tree_),
    coeff1("c1","1S CB fraction", 10001.0, 0.0, 10000000.0),
    coeff2("c2","1S CB fraction", 10000.0, 0.0, 10000000.0),
    mass("mass","onia mass",massLow,massHigh),
    dataset("mass dataset","mass dataset",tree_,mass),
    cball1(mass,"1"),
    cball2(mass,"2"),
    dcball("dcb","double crystal ball", RooArgList(*(cball1.getCB()),*(cball2.getCB()) ),RooArgList(coeff1,coeff2) )
    {
    }

    RooAbsReal* fit()
    {
        dcball.fitTo(dataset, Hesse(kTRUE),Timer(kTRUE),Extended());
        return &dcball;
    }

    RooDataSet* getDataset()
    {
        return &dataset;
    }

    RooRealVar* getVar()
    {
        return &mass;
    }
};