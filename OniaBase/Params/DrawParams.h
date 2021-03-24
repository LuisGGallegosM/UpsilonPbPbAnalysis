#ifndef DRAWPARAMS
#define DRAWPARAMS

#include "CutParams.h"
#include "FitParams.h"

struct drawConfig
{
    int nBins;
    CutParams cut;
    fitConfig fitConf;

    bool isValid() const
    {
        return (nBins>0) && (cut.isValid()) && (fitConf.isValid());
    }
    drawConfig(): nBins(-1),cut(), fitConf()
    {
    }

    void deserialize(const std::string& configfilename,const std::string& cutfilename, const std::string& fitfilename);
};

#endif


