
#include "DrawParams.h"
#include "../../Utils/Serialization/Serialization.h"

void drawConfig::deserialize(const std::string& configfilename,const std::string& cutfilename, const std::string& fitfilename)
{
    cut.deserialize(cutfilename);
    fitConf.deserialize(fitfilename);

    Serializer ser(configfilename);

    ser.read("nBins",nBins);
}