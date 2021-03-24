#ifndef DRAWINGCMP
#define DRAWINGCMP

#include"../../OniaBase/Params/Params.h"

/**
 * @brief describes the results of a fit.
 * fit variable stores the parameters results.
 * configs stores the fit configuration used to produce the fit.
 */
struct FitElement
{
    fitParamsWithErrors fits;
    fitConfig configs;
};

typedef float (fitParamsNoLimits::*fitGetter)() const;

/**
 * @brief Describes the name of a parameter and what function to use
 * to retrieve it from a fitParamsNoLimit object.
 * 
 */
struct toGet
{
    std::string name;
    fitGetter getter;
    toGet(const char* name_,fitGetter getter_): name(name_), getter(getter_) {}
};

#if defined(__CLING__)
#include "DrawingCmp.cpp"
#endif

#endif