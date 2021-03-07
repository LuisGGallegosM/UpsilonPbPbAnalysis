#ifndef ONIAWRITER
#define ONIAWRITER

#include"TLorentzVector.h"
#include"../TreeProcessor/TreeWriter.h"
#include"OniaReader.h"
#include"OniaData/OniaDataSimple.h"


class OniaWriter
{
    TreeWriter writer;
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ oniaQQOut;

    public:
    OniaWriter(const char* treeName);

    void writeReco(const OniaReader* dataIn, int index, int entry);
    void writeGen(const OniaReader* dataIn, int index, int entry);
    void writeGen(const OniaReader2* dataIn, int index, int entry);

    void Write() {writer.Write();}
};

class OniaWriterFull
{
    TreeWriter writer;
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ oniaQQOut;
    OniaSimpleMu oniaMuOut;

    public:
    OniaWriterFull(const char* treeName);
    void writeReco(const OniaReader* dataIn, int index, int entry);
    void writeGen(const OniaReader* dataIn, int index, int entry);
    void writeGen(const OniaReader2* dataIn, int index, int entry);

    void Write() {writer.Write();}
};

#endif