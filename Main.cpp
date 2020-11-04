
#include "Main.h"

using namespace std;

void SetCutParams(cutParams& cut);
void SetFitConfig(fitConfig& fitConf);
void SetDrawConfig(drawConfig& drawConf, const cutParams* cut, const fitConfig* fitConf);

void Main(const char* filename="../rootfiles/merged_HiForestAOD.root", const char*  outfilename = "files/merged_HiForestAOD_skimmed.root")
{
    try
    {
        string data_filename = filename;
        string skimmed_filename = outfilename;
        string folder_name   = "files/test";

        string fit_filename = folder_name + "/oniafit.root";
        string drawing_filename = folder_name + "/oniafit.pdf";
    
        cutParams cut;
        SetCutParams(cut);

        fitConfig fitConf;
        SetFitConfig(fitConf);

        drawConfig drawConf;
        SetDrawConfig(drawConf,&cut,&fitConf);

        gSystem->mkdir(folder_name.data(),true);

        //Skimming function
        Skimming(data_filename.data(),skimmed_filename.data(),&cut);

        //Mass Fitting function
        //Fitting(skimmed_filename.data(),fit_filename.data(),&fitConf);

        //Generate drawings
        //Drawing(fit_filename.data(),drawing_filename.data(),&drawConf);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    return;
}

void SetCutParams(cutParams& kineCut)
{
    kineCut.isMC = false;
    kineCut.trigSelect = HLT_HIL1DoubleMuOpen_v1;
    kineCut.checkSign=true;

    kineCut.sign = 0;
    kineCut.selectionBits = ((1 <<1) | (1 << 3));
    kineCut.minTracks =6;
    kineCut.minPixels =1;

    kineCut.ptLow=0.0f;
    kineCut.ptHigh=50.0f;
    kineCut.yLow=0.0f;
    kineCut.yHigh=2.4f;
    kineCut.singleMuPtLow=3.5f;
    kineCut.singleMuEtaHigh=2.4f;

    kineCut.maxDxy = 0.3f;
    kineCut.maxDz = 20.0f;
    kineCut.minVtxProb = 0.01f;
}

void SetFitConfig(fitConfig& fitConf)
{
    //set background and fit range
    fitConf.bkgOn =true;
    fitConf.massHigh = 10.0f;
    fitConf.massLow = 8.5f;

    //set initial values for fitting parameters
    fitConf.initialValues.nSig = 5000000.0f;
    fitConf.initialValues.nBkg = 500000.0f;
    fitConf.initialValues.chk4_k1=0.2f;
    fitConf.initialValues.chk4_k2=-0.1f;

    fitConf.initialValues.dcb.mean=9.46f;
    fitConf.initialValues.dcb.alpha = 2.522f;
    fitConf.initialValues.dcb.n =1.705f;
    fitConf.initialValues.dcb.sigma1 = 0.1f;
    fitConf.initialValues.dcb.x = 0.5f;
    fitConf.initialValues.dcb.f = 0.5f;
}

void SetDrawConfig(drawConfig& drawConf, const cutParams* cut, const fitConfig* fitConf)
{
    drawConf.nBins = (fitConf->massHigh- fitConf->massLow)*100;
    drawConf.cut = cut;
    drawConf.fitConf = fitConf;
}


#if !defined(__CLING__)

int main(int argc, char **argv)
{
    if (argc > 2)
        Main(argv[1],argv[2]);
    else
        Main();
    return 0;
}

#endif