
#include"DrawCmp.h"
#include"../../Utils/Helpers/Helpers.h"

#include <vector>
#include<algorithm>

#include"TH1.h"
#include"TCanvas.h"

void drawCompGraph(fitGetter func, const std::vector<FitElement>& fits,TH1F* graph);
std::vector<double> generateBinBoundaries(const std::vector<FitElement>& configs);
std::vector<toGet> fillVariables(const fitConfig* fit);

/**
 * @brief Recopile multiple fit results and generate drawings of parameters as function of pT
 * 
 * @param outputpath path to where place output plots and root file
 * @param size number of pT bins to draw
 * @param fitfilepaths array of paths to fit results, each corresponding to a different pT bin
 */
void DrawCmp(const char* outputpath,int size,const char** fitfilepaths)
{
    std::cout << "\nDRAWING\n";
    std::cout << "Output to folder : " << outputpath <<'\n';
    std::vector<std::string> fitpaths;
    for(int i=0;i<size;i++)
    {
        std::cout << "Reading fit results from: " << fitfilepaths[i] <<'\n';
        fitpaths.push_back(fitfilepaths[i]);
    }
        
    std::vector<FitElement> fits;

    for(const auto& fitpath : fitpaths)
    {
        fits.emplace_back();
        FitElement& fit = fits.back();
        std::string basename= getBasename(fitpath);
        fit.fits.deserialize(fitpath+"/"+basename+".fit");
        fit.configs.deserialize(fitpath+"/"+basename+".fitconf");
        assert(fit.fits.isValid());
        assert(fit.configs.isValid());
    }

    //sort fit bins in case they are unordered
    std::sort(fits.begin(),fits.end(),
            [](FitElement& l,FitElement& r) {return l.configs.getCut()->getPtLow() < r.configs.getCut()->getPtHigh();});

    const std::vector<double> xbins = generateBinBoundaries(fits);
    const std::vector<toGet> getters = fillVariables(&(fits[0].configs));

    //root output file
    std::string outfilename= std::string(outputpath)+"/fit.root";
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    for(const auto& getter : getters)
    {
        const char* name= getter.name.data();
        std::string outfilename = std::string(outputpath)+"/"+name+".pdf";
        std::string plotname = std::string(name)+" plot";

        TCanvas canvas(plotname.data(),plotname.data(),4,45,550,520);
        canvas.cd();

        TPad pad("pad","fit", 0.02, 0.02, 0.98, 0.98);
        
        pad.Draw();
        pad.cd();
        TH1F compGraph(name,name,fits.size(),xbins.data());
        compGraph.GetXaxis()->SetTitle("p_{T} ( GeV/c )");
        compGraph.GetYaxis()->SetTitle(name);
        compGraph.GetXaxis()->SetLabelSize(0.03f);
        compGraph.GetYaxis()->SetLabelSize(0.03f);
        drawCompGraph(getter.getter,fits,&compGraph);
        compGraph.Draw();
        
        compGraph.Write();
        canvas.Write();
        canvas.SaveAs(outfilename.data());
        std::cout << "output file: "<< outfilename << "\n";
    }

    outputfile->Close();

    return;
}

/**
 * @brief configure which variables will be plotted and how to get them from fitParams
 * 
 * @param fit 
 * @return std::vector<toGet> 
 */
std::vector<toGet> fillVariables(const fitConfig* fit)
{
    std::vector<toGet> getters;
    getters.push_back(toGet{"alpha", &fitParams::getAlpha});
    getters.push_back(toGet{"n",&fitParams::getN});
    getters.push_back(toGet{"f",&fitParams::getF});
    getters.push_back(toGet{"sigma_Y1S",&fitParams::getSigmaY1S});
    getters.push_back(toGet{"x",&fitParams::getX});
    getters.push_back(toGet{"nSigY1S",&fitParams::getNSigY1S});
    getters.push_back(toGet{"mass_Y1S",&fitParams::getMeanY1S});

    if (fit->isMoreUpsilon())
    {
        getters.push_back(toGet{"nSigY2S",&fitParams::getNSigY2S});
        getters.push_back(toGet{"nSigY3S",&fitParams::getNSigY3S});

        getters.push_back(toGet{"mass_Y2S",&fitParams::getMeanY2S});
        getters.push_back(toGet{"mass_Y3S",&fitParams::getMeanY3S});

        getters.push_back(toGet{"sigma_Y2S",&fitParams::getSigmaY2S});
        getters.push_back(toGet{"sigma_Y3S",&fitParams::getSigmaY3S});
    }
 
    if (fit->isBkgOn())
    {
        getters.push_back(toGet{"nBkg",&fitParams::getNBkg});
    }

    switch (fit->getBkgType())
    {
        case BkgParams::BkgType::chev:
        getters.push_back(toGet{"chk4_k1",&fitParams::getChk4_k1});
        getters.push_back(toGet{"chk4_k2",&fitParams::getChk4_k2});
        break;
        case BkgParams::BkgType::expChev2:
        getters.push_back(toGet{"chk4_k1",&fitParams::getChk4_k1});
        getters.push_back(toGet{"chk4_k2",&fitParams::getChk4_k2});
        break;
        case BkgParams::BkgType::special:
        getters.push_back(toGet{"lambda_bkg",&fitParams::getLambda});
        getters.push_back(toGet{"sigma_bkg",&fitParams::getSigmaBkg});
        getters.push_back(toGet{"mu_bkg",&fitParams::getMu});
        break;

        case BkgParams::BkgType::exponential:
        getters.push_back(toGet{"lambda_bkg",&fitParams::getLambda});
        break;
    }
    return getters;
}

/**
 * @brief Generate the bins ranges from fits readed
 * 
 * @param configs vector of fits
 * @return std::vector<double> sequence of bins boundaries locations
 */
std::vector<double> generateBinBoundaries(const std::vector<FitElement>& configs)
{
    std::vector<double> xbins;
    xbins.push_back(configs[0].configs.getCut()->getPtLow());
    for(const auto& config : configs)
    {
        xbins.push_back(config.configs.getCut()->getPtHigh());
    }
    return xbins;
}

/**
 * @brief draw the plot corresponding to a parameter
 * 
 * @param func pointer to member function that obtains the parameter from the fit result object.
 * @param fits the vector of fit results objects.
 * @param graph histogram to fill with data.
 */
void drawCompGraph(fitGetter func, const std::vector<FitElement>& fits,TH1F* graph)
{
    int i=0;
    for (const auto& fit : fits)
    {
        float value = (fit.fits.*func)();
        if (value == -1.0f) continue;
        float pt =0.5f*(fit.configs.getCut()->getPtHigh() + fit.configs.getCut()->getPtLow());
        graph->Fill(pt,value);
        graph->SetBinError(i+1,(fit.fits.getErrors().*func)());
        i++;
    }

    return;
}