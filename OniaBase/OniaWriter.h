#ifndef ONIAWRITER
#define ONIAWRITER

#include"RtypesCore.h"
#include"../TreeProcessor/TreeWriter.h"
#include"OniaReader.h"

enum class QQtype { Reco, Gen };

class OniaWriter
{
    public:
    virtual void Write()=0;
    virtual void writeEntries(const OniaReader* dataIn, int index, int entry)=0;
    virtual QQtype getType() const =0;

    virtual ~OniaWriter()=default;
};

class OniaWriterBase : public TreeWriter, public OniaWriter
{
    private:
    Int_t Evt;
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;
    Int_t pdgId;
    QQtype qqType;

    protected:
    void readRecoQQ(const OniaReader* dataIn, int index, int entry);
    void readGenQQ(const OniaReader* dataIn, int index, int entry);

    public:
    OniaWriterBase(const char* treeName, QQtype type);

    void writeEntries(const OniaReader* dataIn, int index, int entry) override;
    void Write() override { TreeWriter::Write(); }
    QQtype getType() const override {return qqType;}

    virtual ~OniaWriterBase() =default;
};

class OniaWriterFull : public OniaWriterBase
{
    Float_t pT_mi;
    Float_t eta_mi;
    Float_t phi_mi;
    Float_t pT_pl;
    Float_t eta_pl;
    Float_t phi_pl;

    void readRecoMu(const OniaReader* dataIn, int index, int entry);
    void readGenMu(const OniaReader* dataIn, int index, int entry);

    public:
    OniaWriterFull(const char* treeName, QQtype type);
    void writeEntries(const OniaReader* dataIn, int index, int entry) override;
};

#endif