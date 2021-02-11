
#include "SkimParams.h"

void cutParams::deserialize(const std::string& filename)
{
    serializer ser(filename);
    ser.read("isMC",isMC);
    ser.read("trigSelect", trigSelect);
    ser.read("checkSign",checkSign );
    ser.read("sign",sign);
    ser.read("prescale",prescale);
    ser.read("genLoop",genLoop,false);

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