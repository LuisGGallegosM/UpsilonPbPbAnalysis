
#include"LLRtest.h"
#include"../Common/Common.h"
#include"../../Utils/Helpers/Helpers.h"

#include <vector>
#include<algorithm>

#include"TH1.h"
#include"TCanvas.h"
#include"RooDataSet.h"
#include"RooAddPdf.h"

struct fit_result
{
    std::string name;
    int binNum;
    double nll;
    int npar;
};

struct fit_model
{
    std::string directory;
    std::vector<fit_result> fits;
};

const double pvalcut=0.05;

/**
 * @brief Recopile multiple fit results and generate drawings of parameters as function of pT
 * 
 * @param outputpath path to where place output plots and root file
 * @param size number of pT bins to draw
 * @param fitfilepaths array of paths to fit results, each corresponding to a different pT bin
 */
void LLRtest(const char** multifitpaths,const char* outputpath,int size)
{
    std::cout << "\nLLR TEST\n";

    for(int i=0;i< size;i++)
    {
        std::cout << "Input multifit directory: " <<  multifitpaths[i] << " \n";
    }

    std::cout << "Output to folder : " << outputpath <<'\n';
    std::vector<std::string> fitpaths;

    std::vector<fit_model> multifits;

    multifits.reserve(size);

    for(int i=0;i< size;i++)
    {
        ParameterGroup multifit;
        const std::string multifitconfig_file_path=multifitpaths[i];
        multifit.deserialize(multifitconfig_file_path);
        fit_model model;
        model.directory= multifitconfig_file_path.substr(0,multifitconfig_file_path.rfind('/'));

        const std::vector<float> fit_ranges= getFloatVector(multifit,"diffVar.values.0");
        const int numOfFits=fit_ranges.size()-1;

        for(int j=0;j<numOfFits;j++)
        {
            const std::string fitpath=model.directory+"/fit"+std::to_string(j)+"/fit"+std::to_string(j)+".root";
            TFile* fitfile= OpenFile( fitpath.data(),"READ");
            RooDataSet* dataset= dynamic_cast<RooDataSet*>(fitfile->Get("dataset"));
            RooAddPdf* fitpdf= dynamic_cast<RooAddPdf*>(fitfile->Get("dcb_fit"));
            int npar= (*fitpdf->getComponents())["bkg"].getParameters(*dataset)->getSize();
            RooAbsReal* nll= fitpdf->createNLL(*dataset);
            
            fit_result fresult;
            fresult.name= getBasename(model.directory)+"/fit"+std::to_string(j);
            fresult.binNum= j;
            fresult.nll= nll->getVal();
            fresult.npar=npar;
            model.fits.push_back(fresult);
        }
        multifits.push_back(model);
    }

    for(auto& multifit : multifits)
    {
        std::cout << multifit.directory << "\n";
        for(auto& fit : multifit.fits)
        {
            std::cout << ">>" << fit.name << "  NLL:" << fit.nll << "  NPAR:" << fit.npar << "\n";
        }
    }

    const int binNumber=multifits[0].fits.size();

    std::vector<std::vector<fit_result>> modelsPerBin;

    for(int i=0;i< binNumber;i++)
    {
        std::vector<fit_result> bin;
        for(int j=0;j< multifits.size();j++)
        {
            bin.push_back(multifits[j].fits[i]);
        }

        std::sort(bin.begin(),bin.end(),[](const fit_result& l,const fit_result& r){ return l.npar < r.npar; } );

        modelsPerBin.push_back(std::move(bin));
    }

    std::vector<fit_result> answer;

    for(auto& bin : modelsPerBin)
    {
        std::vector<int> cnt(bin.size(),0);
        std::cout << "Bin: " << bin[0].binNum <<"\n" ;
        for(int i=0;i< bin.size();i++)
        {
            std::cout << ">>" << bin[i].name << "  NLL:" << bin[i].nll << "  NPAR:" << bin[i].npar << "AIC" << 2*(bin[i].npar+bin[i].nll) << "\n";
            int     nParB      = bin[i].npar;
            double  NLLB       = bin[i].nll;
            double  AICB       = 2*(nParB + NLLB);
            for(int j=0;j< bin.size();j++)
            {
                int     nParA      = bin[j].npar;
                double  NLLA       = bin[j].nll;
                double  AICA       = 2*(nParA + NLLA);

                if((nParA >= bin[i].npar) && ( j!=i ))
                {
                    double  diffNLL    = -2.0*(NLLA - NLLB);
                    double  diffNPar   =  2.0*(nParA-nParB);
                    double  probChi2   = TMath::Prob(diffNLL, diffNPar);

                    if (diffNLL<0) probChi2 = 1.0;
                    if (probChi2>pvalcut && (nParA-nParB)<=2) cnt[i]++;

                    std::cout   << "  " << bin[j].name << "\n";
                    std::cout   << "    NLL: " << NLLA<< "\n"
                                << "    Diff:" << diffNLL << "\n"
                                << "    Prob:" << probChi2*100.0f << " % \n"
                                << "    AIC :" << -(AICA-AICB) << "\n";
                
                }
            }
        }
        // which is the best model for this bin?
    int minok=999; 
    int maxpar=0; 
    int bestModelIndex=0;
    for (int i=0;i< bin.size();i++) {
       if (bin[i].npar > maxpar) maxpar = bin[i].npar;
       if (cnt[i]>=2 && bin[i].npar<minok) {
          bestModelIndex=i;
          minok = bin[i].npar;
       }
    }
    if (minok==999) { // sometimes the best model is one of the two highest orders...
       for (int i=0;i< bin.size();i++) {
          if ((cnt[i]>=maxpar-bin[i].npar) && (bin[i].npar<minok)) {
             bestModelIndex=i;
             minok = bin[i].npar;
          }
       }
    }

    std::cout << "****The winner is: " << bin[bestModelIndex].name << "\n" ;
    std::cout << "NLL :" << bin[bestModelIndex].nll  << "\n";
    std::cout << "nPar:" << bin[bestModelIndex].npar << "\n" << "\n";
    answer.push_back(bin[bestModelIndex]);

    }

    std::cout << "winners:\n";

    for(auto& winner : answer)
    {
        std::cout << winner.name << "\n";
    }

    std::cout << "\nsuccess";

    return;
}