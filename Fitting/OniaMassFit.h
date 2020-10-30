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
#include "../Params.h"

//Using in CrystalBall function

#define S1_MEAN_MAX     (9.56f)
#define S1_MEAN_MIN     (9.36f)

#define S1_SIGMA_MAX    (0.40f)
#define S1_SIGMA_MIN    (0.01f)

#define S1_ALPHA_MAX    (5.50f)
#define S1_ALPHA_MIN    (0.50f)

#define S1_N_MAX        (25.50f)
#define S1_N_MIN        (0.05f)

#define S1_CH4_MAX      (4.0f)
#define S1_CH4_MIN      (-4.0f)

#define S1_NSIG_MAX     (10000000.0f)
#define S1_NBKG_MAX     (1000000.0f)


class Chevychev2
{
    RooRealVar ch4_k1;
    RooRealVar ch4_k2;
    RooChebychev chev;

    public:
    Chevychev2(RooRealVar& var,const char* name,float k1, float k2);

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
    CrystalBall(RooRealVar& var, const char* name, const dcbParam* initial);

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
    RooRealVar x;
    RooFormulaVar alpha_2;
    RooFormulaVar n_2;
    RooFormulaVar sigma_2;
    RooRealVar f;
    RooCBShape cBall_2;
    RooAddPdf dcball;

    public:
    DoubleCrystalBall(RooRealVar& var, const dcbParam* initial);
    RooAbsPdf* getDCB();
};

class OniaMassFitter
{
    fitConfig config;
    TTree* tree;
    RooRealVar mass;
    RooRealVar nSig;
    RooRealVar nBkg;
    DoubleCrystalBall dcball;
    Chevychev2 bkg;
    std::unique_ptr<RooDataSet> dataset;
    std::unique_ptr<RooFitResult> results;
    std::unique_ptr<RooAbsPdf> output;

    std::string getKineCutExpr() const;

    public:

    /**
     * @brief Construct a new Onia Mass Fitter object. This objects allows
     * to fit a dataset using a double crystal ball function.
     * 
     * @param tree_ Tree from where to get a branch called "mass" to fit
     * @param kineCut Kinect cut for parameters
     * @param initialGuess Initial value (first guess) for the fitting process
     */
    OniaMassFitter(TTree* tree_,const fitConfig* fitConfig);

    /**
     * @brief Executes the fit using fitTo function and stores results.
     * 
     * @return RooAbsReal* fit function for mass. This result is owned by this object.
     */
    RooAbsReal* fit();

    /**
     * @brief Get the cutted dataset object used for the last fit.
     * 
     * @return RooDataSet* dataset saved from tree. This result is owned by this object.
     */
    RooDataSet* getDataset() const;

    /**
     * @brief Get the mass observable variable.
     * 
     * @return RooRealVar*  This result is owned by this object.
     */
    RooRealVar* getVar();

    /**
     * @brief Get the RooFitResult object of last fit.
     * 
     * @return RooFitResult* This result is owned by this object. 
     */
    RooFitResult* getResults() const;
};

#endif