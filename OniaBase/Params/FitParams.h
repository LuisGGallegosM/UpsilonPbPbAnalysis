#ifndef FITPARAMS
#define FITPARAMS

#include "../../Utils/Serialization/Serialization.h"

#define RATIO_Y2S (10.023/9.460)
#define RATIO_Y3S (10.355/9.460)

/**
 * @brief stores the values of double crystal ball function parameters.
 */
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

    bool isValid() const
    {
        return (mean>=0.0f) && (alpha>=0.0f) && (n>=0.0f) && (sigma>=0.0f) && (x>=0.0f) && (f>=0.0f);
    }

    void setParams(float mean_,float alpha_,float n_,float sigma_, float x_, float f_);

    float getMean()  const {return mean;}
    float getAlpha() const {return alpha;}
    float getN()     const {return n;}
    float getSigma() const { return sigma;}
    float getX()     const {return x;}
    float getF()     const {return f;}

    void deserialize(Serializer& ser);
    void serialize(Serializer& ser) const;
};

/**
 * @brief stores the values of background parameters.
 */
class BkgParams
{
    public:
    enum class BkgType {error, none, chev, special,exponential,expChev2 };

    private:
    float chk4_k1;
    float chk4_k2;
    float mu;
    float sigma;
    float lambda;
    BkgType bkgType;

    struct bkgPair
    {
        std::string name;
        BkgType type;
    };

    const static std::vector<bkgPair> bkgNames;

    public:
    BkgParams():chk4_k1(-1.0f),chk4_k2(-1.0f) ,mu(-1.0f),sigma(-1.0f),lambda(-1.0f),bkgType(BkgType::error)
    {}

    bool isValid() const { return bkgType!= BkgType::error;}

    void setChk4(float k1,float k2) { chk4_k1=k1; chk4_k2=k2;}
    void setSpBkg(float mu_, float sigma_, float lambda_) {mu=mu_;sigma=sigma_;lambda=lambda_;}
    void setLambda(float lambda_) { lambda=lambda_;}
    void setBkgType(BkgType type) {bkgType = type;}

    float getChk4_k1() const {return chk4_k1;}
    float getChk4_k2() const {return chk4_k2;}
    float getMu() const {return mu;}
    float getSigma() const {return sigma;}
    float getLambda() const {return lambda;}
    BkgType getBkgType() const {return bkgType;}

    void deserialize(Serializer& ser);
    void serialize(Serializer& ser) const;

    static std::string toStr(BkgType type);
    static BkgType fromStr(const std::string& str);
};

/**
 * @brief stores the values for signal yields and background parameters.
 */
class externParams
{
    float nSigY1S;
    float nSigY2S;
    float nSigY3S;
    float nBkg;
    bool moreUpsilon;

    public:
    externParams():nSigY1S(-1.0f),nSigY2S(-1.0f),nSigY3S(-1.0f),nBkg(-1.0f), moreUpsilon(false)
    {
    }

    bool isValid() const;
    
    bool isMoreUpsilon() const {return moreUpsilon;}
    float getNSigY1S() const {return nSigY1S;}
    float getNSigY2S() const {return nSigY2S;}
    float getNSigY3S() const {return nSigY3S;}
    float getNBkg()    const {return nBkg;}

    void setNSig(float Y1S) {nSigY1S=Y1S;}
    void setNSig(float Y1S,float Y2S, float Y3S) {nSigY1S=Y1S; nSigY2S=Y2S; nSigY3S=Y3S;}
    void setNBkg(float value) {nBkg=value;}
    void setMoreUpsilon(bool more) { moreUpsilon=more; }

    void deserialize(Serializer& ser);

    void serialize(Serializer& ser) const;

};

/**
 * @brief stores values of the parameters of a fit.
 */
class fitParamsNoLimits
{
    dcbParam dcb;
    externParams extParam;
    BkgParams bkg;

    public:
    fitParamsNoLimits() : dcb(),extParam(), bkg() {}

    bool isValid() const { return bkg.isValid() && dcb.isValid() && extParam.isValid();}

    //setters
    void setNSig(float Y1S) {extParam.setNSig(Y1S);}
    void setNSig(float Y1S,float Y2S, float Y3S) { extParam.setNSig(Y1S,Y2S,Y3S);}
    void setNBkg(float value) {extParam.setNBkg(value);}
    void setDCBParams(const dcbParam& params) {dcb = params;}
    void setBkgParams(const BkgParams& params) {bkg = params;}
    void setMoreUpsilon(bool more) { extParam.setMoreUpsilon(more); }

    //getters
    float getNSigY1S() const {return extParam.getNSigY1S();}
    float getNSigY2S() const {return extParam.getNSigY2S();}
    float getNSigY3S() const {return extParam.getNSigY3S();}
    float getNBkg()    const {return extParam.getNBkg();}
    bool isMoreUpsilon() const {return extParam.isMoreUpsilon();}
    
    BkgParams::BkgType getBkgType() const {return bkg.getBkgType();}
    float getChk4_k1() const {return bkg.getChk4_k1();}
    float getChk4_k2() const {return bkg.getChk4_k2();}
    float getMu() const {return bkg.getMu();}
    float getSigmaBkg() const {return bkg.getSigma();}
    float getLambda() const {return bkg.getLambda();}

    const dcbParam* getDCBParams() const {return &dcb;}
    float getAlpha() const {return dcb.getAlpha();}
    float getN() const {return dcb.getN();}
    float getF() const {return dcb.getF();}
    float getSigmaY1S() const {return dcb.getSigma();}
    float getSigmaY2S() const {return dcb.getSigma()*RATIO_Y2S;}
    float getSigmaY3S() const {return dcb.getSigma()*RATIO_Y3S;}
    float getX() const {return dcb.getX();}
    float getMeanY1S() const {return dcb.getMean();}
    float getMeanY2S() const {return dcb.getMean()*RATIO_Y2S;}
    float getMeanY3S() const {return dcb.getMean()*RATIO_Y3S;}

    void deserialize(Serializer& ser);

    void serialize(Serializer& ser) const;
};

/**
 * @brief stores the values of parameters of a fit and also
 * the corresponding parameter limits.
 */
class fitParams : public fitParamsNoLimits
{
    fitParamsNoLimits low;
    fitParamsNoLimits high;

    public:
    fitParams() : fitParamsNoLimits(), low(), high()
    {
    }

    bool isValid() const { return fitParamsNoLimits::isValid() && low.isValid() && high.isValid();}
    
    const fitParamsNoLimits& getHighLimit() const {return high;}
    const fitParamsNoLimits& getLowLimit() const {return low;}

    fitParamsNoLimits& getHighLimit() {return high;}
    fitParamsNoLimits& getLowLimit() {return low;}

    void deserialize(Serializer& ser);
    void deserialize(const std::string& filename);

    void serialize(const std::string& filename) const;
    void serialize(Serializer& ser) const;
};

/**
 * @brief stores the values,limits and also the error
 * for each parameter of a fit. 
 */
class fitParamsWithErrors : public fitParams
{
    fitParamsNoLimits errors;

    public:
    fitParamsWithErrors() : fitParams(),errors()
    {
    }

    bool isValid() const { return fitParams::isValid() && errors.isValid();}

    void setMoreUpsilon(bool more) { fitParams::setMoreUpsilon(more); errors.setMoreUpsilon(more); }

    fitParamsNoLimits& getErrors() {return errors;}
    const fitParamsNoLimits& getErrors() const {return errors;}

    void deserialize(Serializer& ser);
    void deserialize(const std::string& filename);

    void serialize(const std::string& filename) const;
};

/**
 * @brief stores the cut variables for fitting.
 */
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

    void deserialize(Serializer& ser);
};

/**
 * @brief stores the configuration for do a fit, including mass ranges,
 * initial values, parameter fixed and cuts for fitting.
 * 
 */
class fitConfig
{
    float massLow;
    float massHigh;

    kinecutParams cut;
    fitParams initialValues;

    bool fixAlpha;
    bool fixN;

    public:

    fitConfig(): massLow(-1.0f), massHigh(-1.0f), cut(), initialValues(), fixAlpha(false),fixN(false)
    {
    }

    bool isBkgOn() const { return initialValues.getBkgType() != BkgParams::BkgType::none;}
    bool isMoreUpsilon() const {return initialValues.isMoreUpsilon();}
    float getMassLow() const {return massLow;}
    float getMassHigh() const {return massHigh;}
    bool getFixAlpha() const {return fixAlpha;}
    bool getFixN() const {return fixN;}
    const kinecutParams* getCut() const {return &cut;}
    const fitParams* getInitValues() const {return &initialValues;}
    BkgParams::BkgType getBkgType() const {return initialValues.getBkgType();}

    bool isValid() const
    {
        return (massLow>=0.0f) && (massHigh>0.0f) && (cut.isValid()) && (initialValues.isValid());
    }

    void deserialize(const std::string& filename);
};


#if defined(__CLING__)
#include "Params.cpp"
#endif

#endif