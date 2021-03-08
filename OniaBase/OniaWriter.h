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
    OniaWriterReco(const char* treeName):
        writer(treeName)
    {
        oniaInfoOut.addOutputs(&writer);
        oniaQQOut.addOutputs(&writer);
        oniaMuOut.addOutputs(&writer);
    }
    void writeQQ(const Reader* dataIn, int index, int entry) override
    {
        int pdgId=0;
        oniaInfoOut.Write(entry,pdgId);
        oniaQQOut.Write((TLorentzVector*) dataIn->recoQQ.mom4->At(index));
        int mumi_idx= dataIn->recoQQ.mumi_idx[index];
        int mupl_idx= dataIn->recoQQ.mupl_idx[index];
        TLorentzVector* mumi = (TLorentzVector*) dataIn->recoMu.mom4->At(mumi_idx);
        TLorentzVector* mupl = (TLorentzVector*) dataIn->recoMu.mom4->At(mupl_idx);
        oniaMuOut.Write(mupl,mumi);
        writer.FillEntries(); 
    }
    void Write() override 
    {
        writer.Write();
    }
};

template<typename Reader>
class OniaWriterGen :public OniaWriter<Reader>
{
    TreeWriter writer;
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ oniaQQOut;
    OniaSimpleMu oniaMuOut;

    public:
    OniaWriterGen(const char* treeName):
        writer(treeName)
    {
        oniaInfoOut.addOutputs(&writer);
        oniaQQOut.addOutputs(&writer);
        oniaMuOut.addOutputs(&writer);
    }
    void writeQQ(const Reader* dataIn, int index, int entry) override
    {
        oniaInfoOut.Write(entry,dataIn->genQQ.id[index]);
        oniaQQOut.Write((TLorentzVector*) dataIn->genQQ.mom4->At(index));
        int mumi_idx= dataIn->genQQ.mumi_idx[index];
        int mupl_idx= dataIn->genQQ.mupl_idx[index];
        TLorentzVector* mumi = (TLorentzVector*) dataIn->genMu.mom4->At(mumi_idx);
        TLorentzVector* mupl = (TLorentzVector*) dataIn->genMu.mom4->At(mupl_idx);
        oniaMuOut.Write(mupl,mumi);
        writer.FillEntries(); 
    }
    void Write() override 
    {
        writer.Write();
    }
};

template<>
void OniaWriterGen<OniaReaderGenOnly>::writeQQ(const OniaReaderGenOnly* dataIn, int index, int entry);

using OniaWriterGenOnly = OniaWriterGen<OniaReaderGenOnly>;

#endif