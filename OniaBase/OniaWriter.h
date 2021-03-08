#ifndef ONIAWRITER
#define ONIAWRITER

#include"TLorentzVector.h"
#include"../TreeProcessor/TreeWriter.h"
#include"OniaReader.h"
#include"OniaData/OniaDataSimple.h"

template<typename Reader>
class OniaWriter
{
    public:
    virtual void writeQQ(const Reader* dataIn, int index, int entry) = 0;
    virtual void Write() = 0;
    ~OniaWriter() = default;
};

template<typename Reader>
class OniaWriterReco :public OniaWriter<Reader>
{
    TreeWriter writer;
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ oniaQQOut;
    OniaSimpleMu oniaMuOut;

    public:
    OniaWriterReco(const char* treeName);
    void writeQQ(const Reader* dataIn, int index, int entry) override;
    void Write() override {writer.Write();}
};

template<typename Reader>
class OniaWriterGen :public OniaWriter<Reader>
{
    TreeWriter writer;
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ oniaQQOut;
    OniaSimpleMu oniaMuOut;

    public:
    OniaWriterGen(const char* treeName);
    void writeQQ(const Reader* dataIn, int index, int entry) override;
    void Write() override {writer.Write();}
};

using OniaWriterGenOnly = OniaWriterGen<OniaReaderGenOnly>;

#endif