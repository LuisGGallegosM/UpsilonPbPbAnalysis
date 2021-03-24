
#include"FitParams.h"

//DCB

void dcbParam::deserialize(Serializer& ser)
{
    ser.read("mean",mean);
    ser.read("alpha",alpha);
    ser.read("n",n);
    ser.read("sigma",sigma);
    ser.read("x",x);
    ser.read("f",f);
}

void dcbParam::serialize(Serializer& ser) const
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

//BkgParams

void BkgParams::deserialize(Serializer& ser)
{
    std::string str;
    ser.read("bkgType",str);
    bkgType= fromStr(str);
    switch (bkgType)
    {
        case BkgType::chev:
            ser.read("chk4_k1",chk4_k1);
            ser.read("chk4_k2",chk4_k2);
        break;
        case BkgType::expChev2:
            ser.read("chk4_k1",chk4_k1);
            ser.read("chk4_k2",chk4_k2);
        break;
        case BkgType::special:
            ser.read("lambda",lambda);
            ser.read("mu",mu);
            ser.read("sigma",sigma);
        break;
        case BkgType::exponential:
            ser.read("lambda",lambda);
        break;
    }
}

void BkgParams::serialize(Serializer& ser) const
{
    ser.write("bkgType", toStr(bkgType));
    switch(bkgType)
    {
        case BkgType::chev:
            ser.write("chk4_k1",chk4_k1);
            ser.write("chk4_k2",chk4_k2);
        break;
        case BkgType::expChev2:
            ser.write("chk4_k1",chk4_k1);
            ser.write("chk4_k2",chk4_k2);
        break;
        case BkgType::special:
            ser.write("lambda",lambda);
            ser.write("mu",mu);
            ser.write("sigma",sigma);
        break;
        case BkgType::exponential:
            ser.write("lambda",lambda);
        break;
    }
}

const std::vector<BkgParams::bkgPair> BkgParams::bkgNames
{
    {"none",BkgParams::BkgType::none},
     {"chev",BkgParams::BkgType::chev},
    {"special",BkgParams::BkgType::special},
    {"exponential",BkgParams::BkgType::exponential},
    {"expChev2",BkgParams::BkgType::expChev2}
};

std::string BkgParams::toStr(BkgType type)
{
    std::string str;
    bool found=false;
    for(const auto& var : bkgNames)
    {
        if(var.type == type)
        {
            str= var.name;
            found=true;
            break;
        }
    }
    if (!found) throw std::runtime_error("Error: type does not exist");
    return str;
}
BkgParams::BkgType BkgParams::fromStr(const std::string& str)
{
    BkgParams::BkgType type;
    bool found=false;
    for(const auto& var : bkgNames)
    {
        if(var.name == str)
        {
            type=var.type;
            found=true;
            break;
        }
    }

    if (!found) throw std::invalid_argument(std::string("Error: argument for BkgType '")+str+"' not valid");
    return type;
}

//externParams

void externParams::deserialize(Serializer& ser)
{
    ser.read("moreUpsilon",moreUpsilon);
    ser.read("nSigY1S",nSigY1S);
    if (moreUpsilon)
    {
        ser.read("nSigY2S",nSigY2S);
        ser.read("nSigY3S",nSigY3S);
    }

   ser.read("nBkg",nBkg);
}

void externParams::serialize(Serializer& ser) const
{
    ser.write("moreUpsilon",moreUpsilon);
    ser.write("nSigY1S",nSigY1S);
    if (moreUpsilon)
    {
        ser.write("nSigY2S",nSigY2S);
        ser.write("nSigY3S",nSigY3S);
    }
    ser.write("nBkg",nBkg);
}

bool externParams::isValid() const
{
    if (nSigY1S <0) return false;
    if(moreUpsilon)
    {
       return ((nSigY2S >= 0.0f) && (nSigY3S >= 0.0f));
    }
    return true;
}

//fitParamsNoLimits

void fitParamsNoLimits::deserialize(Serializer& ser)
{
    extParam.deserialize(ser);

    ser.addPrefix("dcb");
    dcb.deserialize(ser);
    ser.removePrefix();

    ser.addPrefix("bkg");
    bkg.deserialize(ser);
    ser.removePrefix();
}

void fitParamsNoLimits::serialize(Serializer& ser) const
{
    extParam.serialize(ser);

    ser.addPrefix("dcb");
    dcb.serialize(ser);
    ser.removePrefix();

    ser.addPrefix("bkg");
    bkg.serialize(ser);
    ser.removePrefix();
}

//fitParams

void fitParams::deserialize(Serializer& ser)
{
    fitParamsNoLimits::deserialize(ser);

    ser.addPrefix("limits.high");
    high.deserialize(ser);
    ser.removePrefix();

    ser.addPrefix("limits.low");
    low.deserialize(ser);
    ser.removePrefix();
}

void fitParams::deserialize(const std::string& filename)
{
    Serializer ser(filename);
    deserialize(ser);
}

void fitParams::serialize(const std::string& filename) const
{
    Serializer ser(filename,Serializer::iotype::write);
    serialize(ser);
}

void fitParams::serialize(Serializer& ser) const
{
    fitParamsNoLimits::serialize(ser);

    ser.addPrefix("limits.high");
    high.serialize(ser);
    ser.removePrefix();

    ser.addPrefix("limits.low");
    low.serialize(ser);
    ser.removePrefix();
}

//fitParamsWithErrors
void fitParamsWithErrors::deserialize(Serializer& ser)
{
    fitParams::deserialize(ser);

    ser.addPrefix("error");
    errors.deserialize(ser);
    ser.removePrefix();
}

void fitParamsWithErrors::deserialize(const std::string& filename)
{
    Serializer ser(filename);
    deserialize(ser);
}

void fitParamsWithErrors::serialize(const std::string& filename) const
{
    Serializer ser(filename,Serializer::iotype::write);
    fitParams::serialize(ser);

    ser.addPrefix("error");
    errors.serialize(ser);
    ser.removePrefix();
}


//kinecutParams

void kinecutParams::deserialize(Serializer& ser)
{
    ser.read("ptLow",ptLow);
    ser.read("ptHigh",ptHigh);
    ser.read("yLow",yLow);
    ser.read("yHigh",yHigh);
}

//fitConfig

void fitConfig::deserialize(const std::string& filename)
{
    Serializer ser(filename);
    //set background and fit range
    ser.read("massHigh",massHigh);
    ser.read("massLow",massLow);

    ser.addPrefix("cut");
    cut.deserialize(ser);
    ser.removePrefix();

    ser.addPrefix("initialValues");
    initialValues.deserialize(ser);
    ser.removePrefix();

    ser.addPrefix("fix.dcb");
    ser.read("alpha",fixAlpha);
    ser.read("n",fixN);
    ser.removePrefix();
}