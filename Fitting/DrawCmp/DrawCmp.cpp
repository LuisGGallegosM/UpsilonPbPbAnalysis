
#include"DrawCmp.h"
#include"../../Utils/Helpers/Helpers.h"

#include <vector>
#include<algorithm>

#include"TH1.h"
#include"TCanvas.h"

void drawCompGraph(const std::string& varname, const std::vector<FitElement>& fits,TH1F* graph);
std::vector<double> generateBinBoundaries(const std::vector<FitElement>& configs);
std::vector<toGet> fillVariables(const ParameterGroup* fit);

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
    }

    //sort fit bins in case they are unordered
    std::sort(fits.begin(),fits.end(),
            [](FitElement& l,FitElement& r) {return l.configs.getFloat("cut.pt.low") < r.configs.getFloat("cut.pt.high");});

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
std::vector<toGet> fillVariables(const ParameterGroup* fit)
{
    std::vector<toGet> getters;
    const std::vector<std::string> subgroups= {"signal","bkg"};
    for(auto subgroup : subgroups )
    {
        const auto vars=fit->get(subgroup)->getSubgroupNames();
        for(auto& var : vars)
        {
            if (!(fit->exists(subgroup+"."+var+".fixed") && fit->getBool(subgroup+"."+var+".fixed")))
            {
                getters.push_back(toGet(subgroup+"_"+var, subgroup+"."+var));
            }
        }
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
    xbins.push_back(configs[0].configs.getFloat("cut.pt.low"));
    for(const auto& config : configs)
    {
        xbins.push_back(config.configs.getFloat("cut.pt.high"));
    }
    return xbins;
}

/**
 * @brief draw the plot corresponding to a parameter
 * 
 * @param varname pointer to member function that obtains the parameter from the fit result object.
 * @param fits the vector of fit results objects.
 * @param graph histogram to fill with data.
 */
void drawCompGraph(const std::string& varname, const std::vector<FitElement>& fits,TH1F* graph)
{
    int i=0;
    for (const auto& fit : fits)
    {
        float value = fit.fits.getFloat(varname+".value");
        if (value == -1.0f) continue;
        float pt =0.5f*(fit.configs.getFloat("cut.pt.high") + fit.configs.getFloat("cut.pt.low"));
        graph->Fill(pt,value);
        graph->SetBinError(i+1,fit.fits.getFloat(varname+".error"));
        i++;
    }

    return;
}