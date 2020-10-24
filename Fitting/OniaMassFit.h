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
#include "RooFormulaVar.h"
#include "RooExtendPdf.h"

//Using in CrystalBall function

#define S1MEANMASS_MAX  (9.56)
#define S1MEANMASS      (9.46)
#define S1MEANMASS_MIN  (9.36)

#define S1SIGMAMASS_MAX     (0.4)
#define S1SIGMAMASS         (0.1)
#define S1SIGMAMASS_MIN     (0.01)

#define S1ALPHAMASS_MAX (5.5)
#define S1ALPHAMASS     (2.522)
#define S1ALPHAMASS_MIN (0.5)

#define S1NMASS_MAX     (5.5)
#define S1NMASS         (1.705)
#define S1NMASS_MIN     (0.5)

struct kineCutParam
{
    int nBins;
    float ptLow;
    float ptHigh;
    float yLow;
    float yHigh;
    float singleMuPtLow;
    float singleMuEtaHigh;
    float massLow;
    float massHigh;
    bool bkgOn;
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
    protected:
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

class DoubleCrystalBall : protected CrystalBall
{
    RooFormulaVar mean_2;
    RooRealVar sigma_2;
    RooFormulaVar alpha_2;
    RooFormulaVar n_2;
    RooRealVar fs;
    RooCBShape cBall_2;
    RooAddPdf dcball;

    public:
    DoubleCrystalBall(RooRealVar& var, const char* name1, const char* name2);
    RooAbsPdf* getDCB();
};

class OniaMassFitter
{
    const kineCutParam* kineCut;
    std::string kineCutStr;
    TTree* tree;
    RooRealVar mass;
    RooRealVar pT;
    RooRealVar y;
    RooRealVar pT_mi;
    RooRealVar eta_mi;
    RooRealVar pT_pl;
    RooRealVar eta_pl;
    RooRealVar nSig;
    RooRealVar nBkg;
    DoubleCrystalBall dcball;
    Chevychev2 bkg;
    RooDataSet dataset;
    RooFitResult* results;
    std::unique_ptr<RooAbsPdf> output;

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
    RooAbsReal* fit(bool bkgOn=false);

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