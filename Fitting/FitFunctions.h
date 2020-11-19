#ifndef FITFUNCTIONS
#define FITFUNCTIONS

#include "../Utils/Params.h"

#include "RooRealVar.h"
#include "RooCBShape.h"
#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooFormulaVar.h"

//Using in CrystalBall function

#define S1_MEAN_MAX     (9.56f)
#define S1_MEAN_MIN     (9.36f)

#define S1_SIGMA_MAX    (0.40f)
#define S1_SIGMA_MIN    (0.01f)

#define S1_ALPHA_MAX    (5.50f)
#define S1_ALPHA_MIN    (0.50f)

#define S1_N_MAX        (12.00f)
#define S1_N_MIN        (0.05f)

#define S1_CH4_MAX      (4.0f)
#define S1_CH4_MIN      (-4.0f)

#define S1_NSIG_MAX     (2000000.0f)
#define S1_NBKG_MAX     (200000.0f)


#define RATIO_Y2S (10.023/9.460)
#define RATIO_Y3S (10.355/9.460)

class Chevychev2
{
    RooRealVar ch4_k1;
    RooRealVar ch4_k2;
    RooChebychev chev;

    public:
    Chevychev2(RooRealVar& var,const char* name,float k1, float k2);

    //getters
    RooChebychev* getChev() {return &chev;}
    RooRealVar* getCh4_k1() {return &ch4_k1;}
    RooRealVar* getCh4_k2() {return &ch4_k2;}
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
    CrystalBall(RooRealVar& var, const char* name, const dcbParam* initial);

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
    DoubleCrystalBall(RooRealVar& var,const char* name, const dcbParam* initial);

    //getters
    RooAbsPdf* getDCB() {return &dcball;}
    void getFitParams(dcbParam* params);
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

#if defined(__CLING__)
#include "FitFunctions.cpp"
#endif

#endif