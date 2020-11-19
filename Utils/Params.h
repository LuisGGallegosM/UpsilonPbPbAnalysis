#ifndef PARAMS
#define PARAMS

#include "serialize.h"

#define ONIATTREENAME ("onia_skimmed")
#define JETTTREENAME ("jet_skimmed")

class dcbParam
{
    float mean;
    float alpha;
    float n;
    float sigma;
    float x;
    float f;

    public:

    dcbParam(): mean(-1.0f),alpha(-1.0f),n(-1.0f),sigma(-1.0f),x(-1.0f),f(-1.0f)
    {
    }

    void setParams(float mean_,float alpha_,float n_,float sigma_, float x_, float f_);

    float getMean()  const {return mean;}
    float getAlpha() const {return alpha;}
    float getN()     const {return n;}
    float getSigma() const { return sigma;}
    float getX()     const {return x;}
    float getF()     const {return f;}

    void deserialize(serializer& ser);
    void serialize(serializer& ser) const;
};

class externParams
{
    float nSigY1S;
    float nSigY2S;
    float nSigY3S;
    float nBkg;
    float chk4_k1;
    float chk4_k2;

    public:
    externParams():nSigY1S(-1.0f),nSigY2S(-1.0f),nSigY3S(-1.0f),
                    nBkg(-1.0f),chk4_k1(-1.0f),chk4_k2(-1.0f) 
    {
    }

    float getNSigY1S() const {return nSigY1S;}
    float getNSigY2S() const {return nSigY2S;}
    float getNSigY3S() const {return nSigY3S;}
    float getNBkg()    const {return nBkg;}
    float getChk4_k1() const {return chk4_k1;}
    float getChk4_k2() const {return chk4_k2;}

    void setNSig(float Y1S,float Y2S, float Y3S) {nSigY1S=Y1S; nSigY2S=Y2S; nSigY3S=Y3S;}
    void setNBkg(float value) {nBkg=value;}
    void setChk4(float k1,float k2) { chk4_k1=k1; chk4_k2=k2;  }

    void deserialize(serializer& ser);

    void serialize(serializer& ser) const;

};

class fitParams
{
    dcbParam dcb;
    externParams extParam;

    public:
    fitParams() : dcb(),extParam()
    {
    }

    void setNSig(float Y1S,float Y2S, float Y3S) { extParam.setNSig(Y1S,Y2S,Y3S);}
    void setNBkg(float value) {extParam.setNBkg(value);}
    void setChk4(float k1,float k2) { extParam.setChk4(k1,k2);}

    float getNSigY1S() const {return extParam.getNSigY1S();}
    float getNSigY2S() const {return extParam.getNSigY2S();}
    float getNSigY3S() const {return extParam.getNSigY3S();}
    float getNBkg()    const {return extParam.getNBkg();}
    float getChk4_k1() const {return extParam.getChk4_k1();}
    float getChk4_k2() const {return extParam.getChk4_k2();}

    const dcbParam* getDCBParams() const {return &dcb;}
    dcbParam* getDCBParams() {return &dcb;}

    void deserialize(serializer& ser);

    void deserialize(const std::string& filename);

    void serialize(const std::string& filename) const;
};

class kinecutParams
{
    float ptLow;
    float ptHigh;
    float yLow;
    float yHigh;

    public:
    kinecutParams():ptLow(-1.0f),ptHigh(-1.0f),yLow(-1.0f),yHigh(-1.0f)
    {
    }

    float getPtLow() const {return ptLow;}
    float getPtHigh() const {return ptHigh;}
    float getYLow() const {return yLow;}
    float getYHigh() const {return yHigh;}

    bool isValid() const
    {
        return (ptLow>=0.0f) && (ptHigh>0.0f) && (yLow>=0.0f) && (yHigh>0.0f);
    }

    void deserialize(serializer& ser);
};

struct cutParams
{
    bool isMC;
    bool checkSign;
    unsigned long long trigSelect;
    int sign;
    int prescale;

    int selectionBits;
    int minTracks;
    int minPixels;
    float maxDxy;
    float maxDz;
    float minVtxProb;

    float ptLow;
    float ptHigh;
    float yLow;
    float yHigh;
    float singleMuPtLow;
    float singleMuEtaHigh;

    cutParams() : isMC(false), checkSign(false), trigSelect(0LL), prescale(0),
    sign(0), selectionBits(0), minPixels(-1),minTracks(-1), maxDxy(-1.0f),maxDz(-1.0f),
    minVtxProb(-1.0f),ptHigh(-1.0f),ptLow(-1.0f),yLow(-1.0f),yHigh(-1.0f),
    singleMuPtLow(-1.0f), singleMuEtaHigh(-1.0f)
    {
    }

    bool isValid() const
    {
        return (minTracks>=0) && (minPixels>=0) && (ptLow>=0.0f) 
        && (ptHigh>0.0f) && (singleMuPtLow>=0.0f) && (maxDxy>0.0f)
        && (maxDz > 0.0f) && (minVtxProb >0.0f) && (prescale>0);
    }

    void deserialize(const std::string& filename);
};

struct fitConfig
{
    bool bkgOn;
    bool moreUpsilon;
    float massLow;
    float massHigh;

    kinecutParams cut;
    fitParams initialValues;

    fitConfig(): bkgOn(false),moreUpsilon(false), massLow(-1.0f), massHigh(-1.0f), cut(), initialValues()
    {
    }

    bool isValid() const
    {
        return (massLow>=0.0f) && (massHigh>0.0f) && (cut.isValid());
    }

    void deserialize(const std::string& filename);
};

struct drawConfig
{
    int nBins;
    float minBinY;
    float maxBinY;
    cutParams cut;
    fitConfig fitConf;

    bool isValid() const
    {
        return (nBins>0) && (minBinY>0) && (maxBinY>0) && (cut.isValid()) && (fitConf.isValid());
    }
    drawConfig(): nBins(-1),minBinY(-1),maxBinY(-1), cut(), fitConf()
    {
    }

    void deserialize(const std::string& configfilename,const std::string& cutfilename, const std::string& fitfilename);
};

#if defined(__CLING__)
#include "Params.cpp"
#endif

#endif