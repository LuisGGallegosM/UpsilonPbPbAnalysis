#ifndef PARAMS
#define PARAMS

#include "serialize.h"

#define ONIATTREENAME ("onia_skimmed")
#define JETTTREENAME ("jet_skimmed")

struct dcbParam
{
    float mean;
    float alpha;
    float n;
    float sigma1;
    float x;
    float f;

    dcbParam(): mean(-1.0f),alpha(-1.0f),n(-1.0f),sigma1(-1.0f),x(-1.0f),f(-1.0f)
    {
    }
};

struct fitParams
{
    dcbParam dcb;
    float nSig;
    float nBkg;
    float chk4_k1;
    float chk4_k2;

    fitParams() : dcb(),nSig(-1.0f),nBkg(-1.0f),chk4_k1(-1.0f),chk4_k2(-1.0f)
    {
    }

    void deserialize(const char* filename)
    {
        serializer ser(filename);
        ser.read("dcb.mean",dcb.mean);
        ser.read("dcb.alpha",dcb.alpha);
        ser.read("dcb.n",dcb.n);
        ser.read("dcb.sigma",dcb.sigma1);
        ser.read("dcb.x",dcb.x);
        ser.read("dcb.f",dcb.f);
        ser.read("nSig",nSig);
        ser.read("nBkg",nBkg);
        ser.read("chk4_k1",chk4_k1);
        ser.read("chk4_k2",chk4_k2);
    }

    void serialize(const char* filename)
    {
        serializer ser(filename,serializer::iotype::write);
        ser.write("dcb.mean",dcb.mean);
        ser.write("dcb.alpha",dcb.alpha);
        ser.write("dcb.n",dcb.n);
        ser.write("dcb.sigma",dcb.sigma1);
        ser.write("dcb.x",dcb.x);
        ser.write("dcb.f",dcb.f);
        ser.write("nSig",nSig);
        ser.write("nBkg",nBkg);
        ser.write("chk4_k1",chk4_k1);
        ser.write("chk4_k2",chk4_k2);
    }
};

struct kinecutParams
{
    float ptLow;
    float ptHigh;
    float yLow;
    float yHigh;

    kinecutParams():
    ptLow(-1.0f),ptHigh(-1.0f),yLow(-1.0f),yHigh(-1.0f)
    {

    }

    bool isValid() const
    {
        return (ptLow>=0.0f) && (ptHigh>0.0f) && (yLow>=0.0f) && (yHigh>0.0f);
    }
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

    void deserialize(const char* filename)
    {
        serializer ser(filename);
        ser.read("isMC",isMC);
        ser.read("trigSelect", trigSelect);
        ser.read("checkSign",checkSign );
        ser.read("sign",sign);
        ser.read("prescale",prescale);

        ser.read("selectionBits",selectionBits);
        ser.read("minTracks",minTracks);
        ser.read("minPixels",minPixels);

        ser.read("maxDxy",maxDxy);
        ser.read("maxDz",maxDz);
        ser.read("minVtxProb",minVtxProb);

        ser.read("ptLow",ptLow);
        ser.read("ptHigh",ptHigh);
        ser.read("yLow",yLow);
        ser.read("yHigh",yHigh);
        ser.read("singleMuPtLow",singleMuPtLow);
        ser.read("singleMuEtaHigh",singleMuEtaHigh);
    }
};

struct fitConfig
{
    bool bkgOn;
    float massLow;
    float massHigh;

    kinecutParams cut;
    fitParams initialValues;

    fitConfig(): bkgOn(false), massLow(-1.0f), massHigh(-1.0f), cut(), initialValues()
    {
    }

    bool isValid() const
    {
        return (massLow>=0.0f) && (massHigh>0.0f) && (cut.isValid());
    }

    void deserialize(const char* filename)
    {
        serializer ser(filename);
        //set background and fit range
        ser.read("bkgOn",bkgOn);
        ser.read("massHigh",massHigh);
        ser.read("massLow",massLow);

        //set kinetic cut selection parameters
        ser.read("cut.ptLow",cut.ptLow);
        ser.read("cut.ptHigh",cut.ptHigh);
        ser.read("cut.yLow",cut.yLow);
        ser.read("cut.yHigh",cut.yHigh);

         //set initial values for fitting parameters
        ser.read("initialValues.nSig",initialValues.nSig);
        ser.read("initialValues.nBkg",initialValues.nBkg);
        ser.read("initialValues.chk4_k1",initialValues.chk4_k1);
        ser.read("initialValues.chk4_k2",initialValues.chk4_k2);

        ser.read("initialValues.dcb.mean",initialValues.dcb.mean);
        ser.read("initialValues.dcb.alpha",initialValues.dcb.alpha);
        ser.read("initialValues.dcb.n",initialValues.dcb.n);
        ser.read("initialValues.dcb.sigma1",initialValues.dcb.sigma1);
        ser.read("initialValues.dcb.x",initialValues.dcb.x);
        ser.read("initialValues.dcb.f",initialValues.dcb.f );
    }
};

struct drawConfig
{
    int nBins;
    float minBinY;
    float maxBinY;
    bool moreUpsilon;
    cutParams cut;
    fitConfig fitConf;

    bool isValid() const
    {
        return (nBins>0) && (minBinY>0) && (maxBinY>0) && (cut.isValid()) && (fitConf.isValid());
    }
    drawConfig(): nBins(-1),minBinY(-1),maxBinY(-1), moreUpsilon(false), cut(), fitConf()
    {
    }

    void deserialize(const char* configfilename,const char* cutfilename, const char* fitfilename)
    {
        cut.deserialize(cutfilename);
        fitConf.deserialize(fitfilename);

        serializer ser(configfilename);

        ser.read("nBins",nBins);
        ser.read("minBinY",minBinY);
        ser.read("maxBinY",maxBinY);
        ser.read("moreUpsilon",moreUpsilon);
    }
};

#endif