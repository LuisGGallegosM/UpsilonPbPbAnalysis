#ifndef UNFOLDER
#define UNFOLDER

#include "../../OniaBase/TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../../Utils/Helpers/FileHelpers.h"
#include "../Common/AccCutter.h"
#include "../Common/AccEffAux.h"
#include <array>

#include "RooUnfold.h"
#include "RooUnfoldBayes.h"
#include "Helpers.h"

template<typename T>
class Unfolder
{
    using Reader=OniaJetMCData;
    private:
    OniaReader<Reader> oniaReader;
    T truth_train;
    T measured_train;
    T truth_test;
    T measured_test;
    RooUnfoldResponse response;
    AccCutter accCutter;
    JetCorrector JEC;
    JetUncertainty JEU;

    public:
    Unfolder(TTree* tree);

    void Train();

    const RooUnfoldResponse* getResponse() const
    {
        return &response;
    }

    T* getMeasuredTraining()
    {
        return &measured_train;
    }

    T* getTruthTraining()
    {
        return &truth_train;
    }

    T* getMeasuredTest()
    {
        return &measured_test;
    }

    T* getTruthTest()
    {
        return &truth_test;
    }

    void Fill(float z_measured, float z_truth, float jtpt_measured, float jtpt_truth, bool isForTrain);

    void ProcessEvent(Long64_t entry);

};

class THFiller_DATA
{
    using Reader=OniaJetQQRealData;
    private:
    OniaReader<Reader> oniaReader;
    TH2D measured;
    

    public:
    THFiller_DATA(TTree* tree);

    TH2D* fill();

    TH2D* getMeasured() {return &measured;}

    void ProcessEvent(Long64_t entry);
};

using Unfolder2D = Unfolder<TH2D>;
using Unfolder1D = Unfolder<TH1D>;

#endif