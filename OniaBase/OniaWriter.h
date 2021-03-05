#ifndef ONIAWRITER
#define ONIAWRITER

#include"TLorentzVector.h"
#include"../TreeProcessor/TreeWriter.h"
#include"OniaReader.h"

struct OniaSimpleInfo
{
    Int_t Evt;
    Int_t pdgId;

    OniaSimpleInfo(TreeWriter* writer);
    void Write(int entry, int id)
    {
        Evt=entry;
        pdgId=id;
    }
};

struct OniaSimpleQQ
{
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;

    OniaSimpleQQ(TreeWriter* writer);
    void Write(TLorentzVector* dimuon);
};

struct OniaSimpleMu
{
    Float_t pT_mi;
    Float_t eta_mi;
    Float_t phi_mi;
    Float_t pT_pl;
    Float_t eta_pl;
    Float_t phi_pl;

    OniaSimpleMu(TreeWriter* writer);
    void Write(TLorentzVector* muonPl, TLorentzVector* muonMi);
};

class OniaWriter : public TreeWriter
{
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ oniaQQOut;

    public:
    OniaWriter(const char* treeName);

    void writeReco(const OniaReader* dataIn, int index, int entry);
    void writeGen(const OniaReader* dataIn, int index, int entry);
    void writeGen(const OniaReader2* dataIn, int index, int entry);
};

class OniaWriterFull : public TreeWriter
{
    OniaSimpleInfo oniaInfoOut;
    OniaSimpleQQ oniaQQOut;
    OniaSimpleMu oniaMuOut;

    public:
    OniaWriterFull(const char* treeName);
    void writeReco(const OniaReader* dataIn, int index, int entry);
    void writeGen(const OniaReader* dataIn, int index, int entry);
    void writeGen(const OniaReader2* dataIn, int index, int entry);
};

#endif