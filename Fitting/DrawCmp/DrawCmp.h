#ifndef DRAWINGCMP
#define DRAWINGCMP

#include"../../OniaBase/FitParams.h"
    
struct FitElement
{
    fitParamsWithErrors fits;
    fitConfig configs;
};

struct fitVal
{
    float value;
    float error;
};

typedef float (fitParamsNoLimits::*fitGetter)() const;

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