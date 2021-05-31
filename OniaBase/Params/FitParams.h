#ifndef FITPARAMS
#define FITPARAMS

#include "../../Utils/Serialization/Serialization.h"

#define RATIO_Y2S (10.023/9.460)
#define RATIO_Y3S (10.355/9.460)


enum class BkgType {error, none, chev, special,exponential,expChev2 };

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

#if defined(__CLING__)
#include "Params.cpp"
#endif

#endif