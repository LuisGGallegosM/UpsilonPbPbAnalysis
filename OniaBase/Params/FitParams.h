#ifndef FITPARAMS
#define FITPARAMS

#include "../../Utils/Serialization/Serialization.h"
#include"TEfficiency.h"

#define RATIO_Y2S (10.023/9.460)
#define RATIO_Y3S (10.355/9.460)


enum class BkgType {error, none, chev, special,exponential,expchev };

BkgType getBkgByName(const std::string& name);

/**
 * @brief stores the configuration for do a fit, including mass ranges,
 * initial values, parameter fixed and cuts for fitting.
 * 
 */

class WeightFunc
{
    public:
    virtual float getWeight(float x) const =0;
    virtual double getWeight(double x) const =0;
    virtual ~WeightFunc() = default;
};

class WeightFunc2D
{
    public:
    virtual float getWeight(float x, float y) const =0;
    virtual double getWeight(double x, double y) const =0;
    virtual ~WeightFunc2D() = default;
};

class WeightFuncRooAbs : public WeightFunc
{
    RooAbsReal* f;
    public:
    float getWeight(float x) const override;
    double getWeight(double x) const override;

    WeightFuncRooAbs(RooAbsReal* func);
};

class WeightFuncTEff : public WeightFunc
{
    TEfficiency* hist;

    public:
    float getWeight(float x) const override;
    double getWeight(double x) const override;

    WeightFuncTEff(TEfficiency* h) : hist(h) {};
};

class WeightFuncTH2 : public WeightFunc2D
{
    TH2* hist;

    public:
    float getWeight(float x, float y) const override;
    double getWeight(double x, double y) const override;

    WeightFuncTH2(TH2* h) : hist(h) {};
};

#if defined(__CLING__)
#include "Params.cpp"
#endif

#endif