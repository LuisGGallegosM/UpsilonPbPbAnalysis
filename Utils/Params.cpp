
#include"Params.h"

//DCB

    void dcbParam::deserialize(serializer& ser)
    {
        ser.read("mean",mean);
        ser.read("alpha",alpha);
        ser.read("n",n);
        ser.read("sigma",sigma);
        ser.read("x",x);
        ser.read("f",f);
    }

    void dcbParam::serialize(serializer& ser) const
    {
        ser.write("mean",mean);
        ser.write("alpha",alpha);
        ser.write("n",n);
        ser.write("sigma",sigma);
        ser.write("x",x);
        ser.write("f",f);
    }

    void dcbParam::setParams(float mean_,float alpha_,float n_,float sigma_, float x_, float f_)
    {
        mean=mean_;
        alpha=alpha_;
        n=n_;
        sigma=sigma_;
        x=x_;
        f=f_;
    }

//externParams

    void externParams::deserialize(serializer& ser)
    {
        ser.read("nSigY1S",nSigY1S);
        ser.read("nSigY2S",nSigY2S);
        ser.read("nSigY3S",nSigY3S);
        ser.read("nBkg",nBkg);
        ser.read("chk4_k1",chk4_k1);
        ser.read("chk4_k2",chk4_k2);
    }

    void externParams::serialize(serializer& ser) const
    {
        ser.write("nSigY1S",nSigY1S);
        ser.write("nSigY2S",nSigY2S);
        ser.write("nSigY3S",nSigY3S);
        ser.write("nBkg",nBkg);
        ser.write("chk4_k1",chk4_k1);
        ser.write("chk4_k2",chk4_k2);
    }

//fitParams

    void fitParams::deserialize(serializer& ser)
    {
        extParam.deserialize(ser);
        ser.addPrefix("dcb");
        dcb.deserialize(ser);
        ser.removePrefix();
    }

    void fitParams::deserialize(const std::string& filename)
    {
        serializer ser(filename);
        deserialize(ser);
    }

    void fitParams::serialize(const std::string& filename) const
    {
        serializer ser(filename,serializer::iotype::write);
        extParam.serialize(ser);

        ser.addPrefix("dcb");
        dcb.serialize(ser);
        ser.removePrefix();
    }

//kinecutParams

    void kinecutParams::deserialize(serializer& ser)
    {
        ser.read("ptLow",ptLow);
        ser.read("ptHigh",ptHigh);
        ser.read("yLow",yLow);
        ser.read("yHigh",yHigh);
    }

//cutParams

    void cutParams::deserialize(const std::string& filename)
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

//fitConfig

    void fitConfig::deserialize(const std::string& filename)
    {
        serializer ser(filename);
        //set background and fit range
        ser.read("bkgOn",bkgOn);
        ser.read("moreUpsilon",moreUpsilon);
        ser.read("massHigh",massHigh);
        ser.read("massLow",massLow);

        ser.addPrefix("cut");
        cut.deserialize(ser);
        ser.removePrefix();

        ser.addPrefix("initialValues");
        initialValues.deserialize(ser);
        ser.removePrefix();
    }

//drawConfig

    void drawConfig::deserialize(const std::string& configfilename,const std::string& cutfilename, const std::string& fitfilename)
    {
        cut.deserialize(cutfilename);
        fitConf.deserialize(fitfilename);

        serializer ser(configfilename);

        ser.read("nBins",nBins);
        ser.read("minBinY",minBinY);
        ser.read("maxBinY",maxBinY);
        ser.read("isLog",isLog);
    }