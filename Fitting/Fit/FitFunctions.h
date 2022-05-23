#ifndef FITFUNCTIONS
#define FITFUNCTIONS

#include "../../OniaBase/Params/Params.h"

#include "RooRealVar.h"
#include "RooCBShape.h"
#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooFormulaVar.h"
#include "RooGenericPdf.h"
#include "RooExponential.h"
#include "RooGaussian.h"

class FitFunc
{
    public:
    virtual RooAbsReal* getFunc() = 0;
    virtual ParameterGroup getBkgParams() const = 0;

    virtual ~FitFunc() = default;
};

class NoFitFunc : public FitFunc
{
    RooAbsReal* getFunc() override;
    ParameterGroup getBkgParams() const override;
};

class Chevychev : public FitFunc
{
    std::vector<RooRealVar> chk;
    RooChebychev chev;

    public:
    Chevychev(RooRealVar& var,const char* name,const ParameterGroup* init);

    //getters
    RooAbsReal* getFunc() override {return &chev;}
    ParameterGroup getBkgParams() const override;
};

class ExpChevychev : public FitFunc
{
    std::vector<RooRealVar> chk;
    std::string expr;
    RooGenericPdf expo;

    public:
    ExpChevychev(RooRealVar& var,const char* name,const ParameterGroup* init);

    //getters
    RooAbsReal* getFunc() override {return &expo;}
    ParameterGroup getBkgParams() const override;
};

class SpecialBkg : public FitFunc
{
    RooRealVar mu;
    RooRealVar sigma;
    RooRealVar lambda;
    std::unique_ptr<RooGenericPdf> bkgPdf;
    public:
    SpecialBkg(RooRealVar& var,const char* name,const ParameterGroup* init);

    //getters
    RooAbsReal* getFunc() override {return bkgPdf.get();}
    ParameterGroup getBkgParams() const override;
};

class ExponentialBkg : public FitFunc
{
    RooRealVar lambda;
    std::unique_ptr<RooGenericPdf> bkgPdf;
    public:
    ExponentialBkg(RooRealVar& var,const char* name,const ParameterGroup* init);

    RooAbsReal* getFunc() override {return bkgPdf.get();}
    ParameterGroup getBkgParams() const override;
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
    CrystalBall(RooRealVar& var, const char* name, const ParameterGroup* g);

    /**
     * @brief Get the crystal ball function.
     * 
     * @return RooCBShape* The interally saved crystal ball function.
     */
    RooCBShape* getCB() {return &cBall;};

    ParameterGroup getParams() const;
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
    RooCBShape* getCB() {return &cBall;}
};

class SignalFitFunc
{
    public:
    virtual RooAbsPdf* getDCB()=0;
    virtual ParameterGroup getParams() const =0;

    virtual ~SignalFitFunc()=default;
};

class DoubleCrystalBall : public CrystalBall, public SignalFitFunc
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
    DoubleCrystalBall(RooRealVar& var,const char* name, const ParameterGroup* g);

    //getters
    RooAbsPdf* getDCB() override {return &dcball;}
    ParameterGroup getParams() const override;

    friend class DoubleCrystalBallSlave;
};

class TripleCrystalBall : public CrystalBall, public SignalFitFunc
{
    RooFormulaVar mean_2;
    RooFormulaVar mean_3;
    RooRealVar x_1;
    RooRealVar x_2;
    RooFormulaVar alpha_2;
    RooFormulaVar alpha_3;
    RooFormulaVar n_2;
    RooFormulaVar n_3;
    RooFormulaVar sigma_2;
    RooFormulaVar sigma_3;
    RooRealVar f_1;
    RooRealVar f_2;
    RooCBShape cBall_2;
    RooCBShape cBall_3;
    RooAddPdf tcball;

    public:
    TripleCrystalBall(RooRealVar& var,const char* name, const ParameterGroup* g);

    //getters
    RooAbsPdf* getDCB() override {return &tcball;}
    ParameterGroup getParams() const override;

    friend class TripleCrystalBallSlave;
};

class DoubleCrystalBallSlave : protected CrystalBallSlave, public SignalFitFunc
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
    RooAbsPdf* getDCB() override { return &dcball;}
    ParameterGroup getParams() const override { throw std::runtime_error("ERROR:DCBS");}
};

class TripleCrystalBallSlave : protected CrystalBallSlave, public SignalFitFunc
{
    RooFormulaVar mean_2;
    RooFormulaVar mean_3;
    RooFormulaVar x_1;
    RooFormulaVar x_2;
    RooFormulaVar alpha_2;
    RooFormulaVar alpha_3;
    RooFormulaVar n_2;
    RooFormulaVar n_3;
    RooFormulaVar sigma_2;
    RooFormulaVar sigma_3;
    RooFormulaVar f_1;
    RooFormulaVar f_2;
    RooCBShape cBall_2;
    RooCBShape cBall_3;
    RooAddPdf tcball;

    public:
    TripleCrystalBallSlave(RooRealVar& var,const char* name,TripleCrystalBall& triplecb,float ratio);

    //getter
    RooAbsPdf* getDCB() override { return &tcball;}
    ParameterGroup getParams() const override { throw std::runtime_error("ERROR:TCBS");}
};

class CrystalBallGauss : public CrystalBall, public SignalFitFunc
{
    public:
    CrystalBallGauss(RooRealVar& var,const char* name, const ParameterGroup* g);
    RooAbsPdf* getDCB() override {return &cbexpball;}
    ParameterGroup getParams() const override;
    private:
    RooFormulaVar mean_2;
    RooRealVar x;
    RooFormulaVar sigma_2;
    RooRealVar f;
    RooGaussian gauss;
    RooAddPdf cbexpball;

    friend class CrystalGaussSlave;
    
};

class CrystalGaussSlave : protected CrystalBallSlave, public SignalFitFunc
{
    RooFormulaVar mean_2;
    RooFormulaVar x;
    RooFormulaVar sigma_2;
    RooFormulaVar f;
    RooGaussian gauss;
    RooAddPdf dcball;

    public:
    CrystalGaussSlave(RooRealVar& var,const char* name,CrystalBallGauss& doublecb,float ratio);

    //getter
    RooAbsPdf* getDCB() { return &dcball;}
    ParameterGroup getParams() const override { throw std::runtime_error("ERROR:DCBS");}
};

void ParameterWrite( ParameterGroup& p, const RooRealVar& var, const std::string& name);
FitFunc* BkgFactory(RooRealVar& var, const ParameterGroup* config);

#if defined(__CLING__)
#include "FitFunctions.cpp"
#endif

#endif