
#include "OniaReader.h"

void addInputs(OniaMCData* data ,TreeReader* reader)
{
    data->recoQQ.addInputs(reader);
    data->recoMu.addInputs(reader);
    data->which.addInputs(reader);
    data->genQQ.addInputs(reader);
    data->genMu.addInputs(reader);
}

void addInputs(OniaRealData* data ,TreeReader* reader)
{
    data->recoQQ.addInputs(reader);
    data->recoMu.addInputs(reader);
}

void addInputs(OniaGenOnlyData* data ,TreeReader* reader)
{
    data->genQQ.addInputs(reader);
    data->genMu.addInputs(reader);
}

void addInputs(OniaJetMCData* data ,TreeReader* reader)
{
    data->recoQQ.addInputs(reader);
    data->recoMu.addInputs(reader);
    data->which.addInputs(reader);
    data->genQQ.addInputs(reader);
    data->genMu.addInputs(reader);
    data->jetInfo.addInputs(reader);
    data->jetRef.addInputs(reader);
}

void addInputs(OniaJetRealData* data ,TreeReader* reader)
{
    data->recoQQ.addInputs(reader);
    data->recoMu.addInputs(reader);
    data->jetInfo.addInputs(reader);
}