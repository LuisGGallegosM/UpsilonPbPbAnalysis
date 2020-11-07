#ifndef PARAMS
#define PARAMS

#include "Utils/serialize.h"

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
};

struct fitParams
{
    dcbParam dcb;
    float nSig;
    float nBkg;
    float chk4_k1;
    float chk4_k2;
};

struct cutParams
{
    bool isMC;
    bool checkSign;

    unsigned long long trigSelect;
    int sign;

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

    cutParams() : isMC(false), checkSign(false), trigSelect(0LL),
    sign(0), selectionBits(0), minPixels(-1),minTracks(-1), maxDxy(-1.0f),maxDz(-1.0f),
    minVtxProb(-1.0f),ptHigh(-1.0f),ptLow(-1.0f),yLow(-1.0f),yHigh(-1.0f),
    singleMuPtLow(-1.0f), singleMuEtaHigh(-1.0f)
    {
    }

    bool isValid() const
    {
        return (minTracks>=0) && (minPixels>=0) && (ptLow>=0.0f) 
        && (ptHigh>0.0f) && (singleMuPtLow>0.0f) && (maxDxy>0.0f)
        && (maxDz > 0.0f) && (minVtxProb >0.0f);
    }

    void deserialize(const char* filename)
    {
        serializer ser(filename);
        ser.read("isMC",isMC);
        ser.read("trigSelect", trigSelect);
        ser.read("checkSign",checkSign );
        ser.read("sign",sign);

        ser.read("selectionBits",selectionBits);
        ser.read("minTracks",minTracks);
        ser.read("minPixels",minPixels);
        ser.read("ptLow",ptLow);

        ser.read("ptHigh",ptHigh);
        ser.read("yLow",yLow);
        ser.read("yHigh",yHigh);
        ser.read("singleMuPtLow",singleMuPtLow);

        ser.read("singleMuEtaHigh",singleMuEtaHigh);
        ser.read("maxDxy",maxDxy);
        ser.read("maxDz",maxDz);
        ser.read("minVtxProb",minVtxProb);
    }
};

struct fitConfig
{
    bool bkgOn;
    float massLow;
    float massHigh;
    fitParams initialValues;

    fitConfig(): bkgOn(false), massLow(-1.0f), massHigh(-1.0f), initialValues()
    {
    }

    bool isValid() const
    {
        return (massLow>=0.0f) && (massHigh>0.0f);
    }

    void deserialize(const char* filename)
    {
        serializer ser(filename);
        //set background and fit range
        ser.read("bkgOn",bkgOn);
        ser.read("massHigh",massHigh);
        ser.read("massLow",massLow);

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
    cutParams cut;
    fitConfig fitConf;

    bool isValid() const
    {
        return (nBins>0) && (cut.isValid()) && (fitConf.isValid());
    }
    drawConfig(): nBins(0), cut(), fitConf()
    {
    }

    void deserialize(const char* cutfilename, const char* fitfilename)
    {
        cut.deserialize(cutfilename);
        fitConf.deserialize(fitfilename);
        nBins=(fitConf.massHigh- fitConf.massLow)*100;
    }
};

#endif