#ifndef ONIAMASSFIT
#define ONIAMASSFIT

#include <string>

#include "TTree.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooFitResult.h"

#include "FitFunctions.h"
#include "../OniaBase/FitParams.h"


class OniaMassFitter
{
    protected:
    fitConfig config;
    TTree* tree;
    RooRealVar mass;
    RooRealVar nSig_Y1S;
    RooRealVar nBkg;
    DoubleCrystalBall dcball1;
    std::unique_ptr<BkgFunc> bkg;
    std::unique_ptr<RooDataSet> dataset;
    std::unique_ptr<RooFitResult> results;
    std::unique_ptr<RooAbsPdf> output;

    std::string getKineCutExpr() const;

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
    OniaMassFitter(TTree* tree_,const fitConfig* fitConfig);

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
    virtual fitParamsWithErrors getFitParams();
};

class OniaMassFitter2 : public OniaMassFitter
{
    private:
    RooRealVar nSig_Y2S;
    RooRealVar nSig_Y3S;
    DoubleCrystalBallSlave dcball2;
    DoubleCrystalBallSlave dcball3;

    /**
     * @brief Same as base method, this method includes Y2S and Y3S fields
     * 
     */
    void combinePdf() override;

    public:
    OniaMassFitter2(TTree* tree_,const fitConfig* fitConf);

    fitParamsWithErrors getFitParams() override;
};

#if defined(__CLING__)
#include "OniaMassFit.cpp"
#endif

#endif