#ifndef ONIAWRITER
#define ONIAWRITER

#include"RtypesCore.h"
#include"../TreeProcessor/TreeWriter.h"
#include"OniaReader.h"

class OniaWriter : public TreeWriter
{
    private:
    Int_t Evt;
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;
    Int_t pdgId;

    protected:
    void readGenQQ(const OniaReader* dataIn, int index, int entry);
    void readGenQQ(const OniaReader2* dataIn, int index, int entry);
    void readRecoQQ(const OniaReader* dataIn, int index, int entry);

    Int_t getPdgId() const {return pdgId;}

    public:
    OniaWriter(const char* treeName);

    void writeRecoQQ(const OniaReader* dataIn, int index, int entry);
    void writeGenQQ(const OniaReader* dataIn, int index, int entry);
    void writeGenQQ(const OniaReader2* dataIn, int index, int entry);
};

class OniaWriterFull : public OniaWriter
{
    Float_t pT_mi;
    Float_t eta_mi;
    Float_t phi_mi;
    Float_t pT_pl;
    Float_t eta_pl;
    Float_t phi_pl;
    std::map<int,int> pdgIds;

    void readRecoMu(const OniaReader* dataIn, int index, int entry);
    void readGenMu(const OniaReader* dataIn, int index, int entry);
     void readGenMu(const OniaReader2* dataIn, int index, int entry);

    public:
    OniaWriterFull(const char* treeName);
    void writeRecoQQ(const OniaReader* dataIn, int index, int entry);
    void writeGenQQ(const OniaReader* dataIn, int index, int entry);
    void writeGenQQ(const OniaReader2* dataIn, int index, int entry);

    void Write();
};

#endif