#ifndef ONIAMASSFIT
#define ONIAMASSFIT

#include <string>

#include "TTree.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooFitResult.h"

#include "FitFunctions.h"
#include "../../OniaBase/Params/Params.h"


class OniaMassFitter
{
    protected:
    ParameterGroup config;
    TTree* tree;
    RooRealVar mass;
    RooRealVar nSig_Y1S;
    RooRealVar nBkg;
    std::unique_ptr<SignalFitFunc> dcball1;
    std::unique_ptr<FitFunc> bkg;
    std::unique_ptr<RooDataSet> dataset;
    std::unique_ptr<RooFitResult> results;
    std::unique_ptr<RooAbsPdf> output;

    /**
     * @brief this function makes the total fitting function by combining correctly its components
     * including bkg if bkgOn is enabled.
     * 
     */
    virtual void combinePdf();

    public:
    
    /**
     * @brief Construct a new Onia Mass Fitter object. This objects allows
     * to fit a dataset using a double crystal ball function.
     * 
     * @param tree_ Tree from where to get a branch called "mass" to fit
     * @param kineCut Kinect cut for parameters
     * @param initialGuess Initial value (first guess) for the fitting process
     */
    OniaMassFitter(TTree* tree_,const ParameterGroup * fitConfig);

    virtual ~OniaMassFitter();

    /**
     * @brief Executes the fit using fitTo function and stores results.
     * 
     * @return RooAbsReal* fit function for mass. This result is owned by this object.
     */
    RooAbsReal* fit();

    /**
     * @brief Get the cutted dataset object used for the last fit.
     * 
     * @return RooDataSet* dataset saved from tree. This result is owned by this object.
     */
    RooDataSet* getDataset() const {return dataset.get();}

    /**
     * @brief Get the mass observable variable.
     * 
     * @return RooRealVar*  This result is owned by this object.
     */
    RooRealVar* getVar() {return &mass;}

    /**
     * @brief Get the RooFitResult object of last fit.
     * 
     * @return RooFitResult* This result is owned by this object. 
     */
    RooFitResult* getResults() const { return results.get();}


    /**
     * @brief Get the Fit Params by writing it to paramOut.
     * 
     * @param paramOut fitParams where to fill the got values
     */
    virtual ParameterGroup getFitParams() const;
};

class OniaMassFitter2 : public OniaMassFitter
{
    private:
    RooRealVar nSig_Y2S;
    RooRealVar nSig_Y3S;
    std::unique_ptr<SignalFitFunc> dcball2;
    std::unique_ptr<SignalFitFunc> dcball3;

    /**
     * @brief Same as base method, this method includes Y2S and Y3S fields
     * 
     */
    void combinePdf() override;

    public:
    OniaMassFitter2(TTree* tree_,const ParameterGroup* fitConf);

    ParameterGroup getFitParams() const override;
};

std::unique_ptr<OniaMassFitter> createMassFitter(TTree* input, const ParameterGroup* config);
std::string getKineCutExpr(const ParameterGroup* cut);

#if defined(__CLING__)
#include "OniaMassFit.cpp"
#endif

#endif