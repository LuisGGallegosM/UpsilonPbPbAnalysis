#ifndef FITFUNCTIONS
#define FITFUNCTIONS

#include "../Utils/Params.h"

#include "RooRealVar.h"
#include "RooCBShape.h"
#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooFormulaVar.h"
#include "RooGenericPdf.h"
#include "RooExponential.h"

//Using in CrystalBall function

#define S1_MEAN_MAX     (9.56f)
#define S1_MEAN_MIN     (9.36f)

#define S1_SIGMA_MAX    (0.60f)
#define S1_SIGMA_MIN    (0.01f)

#define S1_ALPHA_MAX    (5.50f)
#define S1_ALPHA_MIN    (0.50f)

#define S1_N_MAX        (15.00f)
#define S1_N_MIN        (0.05f)

#define S1_CH4_MAX      (4.0f)
#define S1_CH4_MIN      (-4.0f)

#define S1_NSIG_MAX     (10000000.0f)
#define S1_NBKG_MAX     (1000000.0f)

#define BKG_LAMBDA_MAX (30.0f)
#define BKG_SIGMA_MAX  (4.0f)
#define BKG_MU_MAX     (20.0f)

class BkgFunc
{
    public:
    virtual RooAbsReal* getFunc() { throw std::runtime_error("Error: accesing no bkg funcion"); }
    virtual BkgParams getBkgParams();
    virtual BkgParams getBkgParamsErrors();
    virtual BkgParams getBkgParamsHigh();
    virtual BkgParams getBkgParamsLow();
};

class Chevychev2 : public BkgFunc
{
    RooRealVar ch4_k1;
    RooRealVar ch4_k2;
    RooChebychev chev;

    public:
    Chevychev2(RooRealVar& var,const char* name,float* k, float* low, float* high);

    //getters
    RooAbsReal* getFunc() override {return &chev;}
    RooRealVar* getCh4_k1() {return &ch4_k1;}
    RooRealVar* getCh4_k2() {return &ch4_k2;}
    BkgParams getBkgParams() override;
    BkgParams getBkgParamsErrors() override;
    BkgParams getBkgParamsHigh() override;
    BkgParams getBkgParamsLow() override;
};

class SpecialBkg : public BkgFunc
{
    RooRealVar mu;
    RooRealVar sigma;
    RooRealVar lambda;
    std::unique_ptr<RooGenericPdf> bkgPdf;
    public:
    SpecialBkg(RooRealVar& var,const char* name,float* initial,float* low, float* high);

    //getters
    RooAbsReal* getFunc() override {return bkgPdf.get();}
    RooRealVar* getMu()  {return &mu;}
    RooRealVar* getSigma() {return &sigma;}
    RooRealVar* getLambda() {return &lambda;}
    BkgParams getBkgParams() override;
    BkgParams getBkgParamsErrors() override;
    BkgParams getBkgParamsHigh() override;
    BkgParams getBkgParamsLow() override;
};

class ExponentialBkg : public BkgFunc
{
    RooRealVar lambda;
    std::unique_ptr<RooGenericPdf> bkgPdf;
    public:
    ExponentialBkg(RooRealVar& var,const char* name,float* initial,float* low, float* high);

    RooAbsReal* getFunc() override {return bkgPdf.get();}
    RooRealVar* getLambda() {return &lambda;}
    BkgParams getBkgParams() override;
    BkgParams getBkgParamsErrors() override;
    BkgParams getBkgParamsHigh() override;
    BkgParams getBkgParamsLow() override;
};

class ExpChev2Bkg : public BkgFunc
{
    RooRealVar ch4_k1;
    RooRealVar ch4_k2;
    std::unique_ptr<RooGenericPdf> bkgPdf;
    public:
    ExpChev2Bkg(RooRealVar& var,const char* name,float* initial,float* low, float* high);

    RooRealVar* getCh4_k1() {return &ch4_k1;}
    RooRealVar* getCh4_k2() {return &ch4_k2;}

    RooAbsReal* getFunc() override {return bkgPdf.get();}
    BkgParams getBkgParams() override;
    BkgParams getBkgParamsErrors() override;
    BkgParams getBkgParamsHigh() override;
    BkgParams getBkgParamsLow() override;
};

class CrystalBall
{
    protected:
    RooRealVar mean;
    RooRealVar sigma;
    RooRealVar alpha;
    RooRealVar n;
    RooCBShape cBall;

    friend class CrystalBallSlave;

    public:

    /**
     * @brief Construct a new Crystal Ball object. This represents a crystal ball
     * function including all needed variables.
     * 
     * @param var observable variable in x axis.
     * @param name a unique name to identify this crystal ball and its variables.
     */
    CrystalBall(RooRealVar& var, const char* name, const dcbParam* initial, const dcbParam* low, const dcbParam* high,bool fixAlpha, bool fixN);

    /**
     * @brief Get the crystal ball function.
     * 
     * @return RooCBShape* The interally saved crystal ball function.
     */
    RooCBShape* getCB();
};

class CrystalBallSlave
{
    protected:
    RooFormulaVar mean;
    RooFormulaVar sigma;
    RooFormulaVar alpha;
    RooFormulaVar n;
    RooCBShape cBall;

    public:

    /**
     * @brief Construct a new Crystal Ball object. This represents a crystal ball
     * function including all needed variables.
     * 
     * @param var observable variable in x axis.
     * @param name a unique name to identify this crystal ball and its variables.
     */
    CrystalBallSlave(RooRealVar& var, CrystalBall& cball, const char* name, float ratio);

    /**
     * @brief Get the crystal ball function.
     * 
     * @return RooCBShape* The interally saved crystal ball function.
     */
    RooCBShape* getCB();
};

class DoubleCrystalBall : public CrystalBall
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
    DoubleCrystalBall(RooRealVar& var,const char* name, const dcbParam* initial, const dcbParam* low, const dcbParam* high,bool fixAlpha, bool fixN);

    //getters
    RooAbsPdf* getDCB() {return &dcball;}
    dcbParam getFitParams() const;
    dcbParam getFitParamsErrors() const;
    dcbParam getFitParamsHigh() const;
    dcbParam getFitParamsLow() const;
    void getFitParamsErrors(dcbParam* params);

    friend class DoubleCrystalBallSlave;
};

class DoubleCrystalBallSlave : protected CrystalBallSlave
{
    RooFormulaVar mean_2;
    RooFormulaVar x;
    RooFormulaVar alpha_2;
    RooFormulaVar n_2;
    RooFormulaVar sigma_2;
    RooFormulaVar f;
    RooCBShape cBall_2;
    RooAddPdf dcball;

    public:
    DoubleCrystalBallSlave(RooRealVar& var,const char* name,DoubleCrystalBall& doublecb,float ratio);

    //getter
    RooAbsPdf* getDCB();
};

BkgFunc* BkgFactory(RooRealVar& var, const fitConfig& config);

#if defined(__CLING__)
#include "FitFunctions.cpp"
#endif

#endif