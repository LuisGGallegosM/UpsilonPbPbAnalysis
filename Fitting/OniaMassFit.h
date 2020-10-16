#ifndef ONIAMASSFIT
#define ONIAMASSFIT

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
#include "RooChebychev.h"

//Using in CrystalBall function

#define S1MEANMASS_MAX  (9.56)
#define S1MEANMASS      (9.46)
#define S1MEANMASS_MIN  (9.36)

#define S1SIGMAMASS_MAX     (0.6)
#define S1SIGMAMASS         (0.1)
#define S1SIGMAMASS_MIN     (0.001)

#define S1ALPHAMASS_MAX (8.5)
#define S1ALPHAMASS     (2.522)
#define S1ALPHAMASS_MIN (0.1)

#define S1NMASS_MAX     (8.5)
#define S1NMASS         (1.705)
#define S1NMASS_MIN     (0.1)

struct kineCutParam
{
    int nBins;
    float ptLow;
    float ptHigh;
    float yLow;
    float yHigh;
    float massLow;
    float massHigh;
};

class Chevychev2
{
    RooRealVar ch4_k1;
    RooRealVar ch4_k2;
    RooChebychev chev;

    public:
    Chevychev2(RooRealVar& var,const char* name);

    RooChebychev* getChev();
};

class CrystalBall
{
    RooRealVar mean;
    RooRealVar sigma;
    RooRealVar alpha;
    RooRealVar n;
    RooCBShape cBall;

    public:

    /**
     * @brief Construct a new Crystal Ball object. This represents a crystal ball
     * function including all needed variables.
     * 
     * @param var observable variable in x axis.
     * @param name a unique name to identify this crystal ball and its variables.
     */
    CrystalBall(RooRealVar& var, const char* name);

    /**
     * @brief Get the crystal ball function.
     * 
     * @return RooCBShape* The interally saved crystal ball function.
     */
    RooCBShape* getCB();
};

class OniaMassFitter
{
    const kineCutParam* kineCut;
    std::string kineCutStr;
    TTree* tree;
    RooRealVar mass;
    RooRealVar pT;
    RooRealVar y;
    RooRealVar fs;
    RooRealVar nSig;
    RooRealVar nBkg;
    CrystalBall cball1;
    CrystalBall cball2;
    Chevychev2 bkg;
    RooAddPdf dcball;
    RooAddPdf dcballbkg;
    RooDataSet dataset;
    RooFitResult* results;

    std::string kineCutExp(const kineCutParam* kineCut);

    public:

    /**
     * @brief Construct a new Onia Mass Fitter object. This objects allows
     * to fit a dataset using a double crystal ball function.
     * 
     * @param tree_ Tree from where to get a branch called "mass" to fit
     * @param massLow_ Mass low boundary.
     * @param massHigh_ Mass high boundary.
     */
    OniaMassFitter(TTree* tree_,const kineCutParam* kineCut);

    /**
     * @brief Executes the fit using fitTo function.
     * 
     * @return RooAbsReal* fit function for mass.
     */
    RooAbsReal* fit();

    /**
     * @brief Get the Dataset object
     * 
     * @return RooDataSet* dataset saved from tree.
     */
    RooDataSet* getDataset();

    /**
     * @brief Get the mass observable variable.
     * 
     * @return RooRealVar* 
     */
    RooRealVar* getVar();

    RooFitResult* getResults();
};

#endif