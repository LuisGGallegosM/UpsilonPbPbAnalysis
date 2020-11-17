

#include"DrawingCmp.h"
#include <vector>
#include<algorithm>
#include"../Utils/Params.h"
#include"../Utils/utils.h"
#include"TH1.h"
#include"TCanvas.h"

struct FitElement
{
    fitParams fits;
    fitConfig configs;
};

void drawCompGraph(float (*func) (const FitElement&), std::vector<FitElement>& fits,TH1F* graph);
std::vector<double> generateBinBoundaries(std::vector<FitElement>& configs);
void fillVariables(  std::vector<const char*>& names,std::vector<float (*) (const FitElement&)>& functors);

void DrawingCmp(const char* outputfilename,int size,const char** fitfilenames)
{
    std::cout << "\nDRAWING\n";
    std::cout << "Drawing output file: " << outputfilename <<'\n';
    for(int i=0;i<size;i++)
    {
        std::cout << "Reading fit results file: " << fitfilenames[i] <<'\n';
        std::cout << "Reading fit configuration file: " <<ReplaceExtension(fitfilenames[i], ".fitconf") <<"\n";
    }
        
    std::vector<FitElement> fits;

    for(int i=0;i<size;i++)
    {
        FitElement fit;
        fit.fits.deserialize(fitfilenames[i]);
        fit.configs.deserialize( ReplaceExtension(fitfilenames[i], ".fitconf").data()  );
        fits.push_back(fit);
    }
    std::sort(fits.begin(),fits.end(),[](FitElement& l,FitElement& r) {return l.configs.cut.ptLow < r.configs.cut.ptLow;});

    std::vector<double> xbins = generateBinBoundaries(fits);
    std::vector<const char*> names;
    std::vector<float (*) (const FitElement&)> functors;

    fillVariables(names,functors);

    for(int i=0;i< names.size();i++)
    {
        TCanvas canvas("Fit_plot","fit",4,45,550,520);
        canvas.cd();

        TPad pad("pad","fit", 0.02, 0.02, 0.98, 0.98);
        
        pad.Draw();
        pad.cd();
        TH1F compGraph(names[i],names[i],fits.size(),xbins.data());
        compGraph.GetXaxis()->SetTitle("p_{T} ( GeV/c )");
        compGraph.GetYaxis()->SetTitle(names[i]);
        drawCompGraph(functors[i],fits,&compGraph);
        compGraph.Draw();
        std::string outfilename = (ReplaceExtension(outputfilename,"_")+names[i]+".pdf");
        canvas.SaveAs(outfilename.data());
        std::cout << "output file: "<< outfilename << "\n";
    }

    return;
}

void fillVariables(  std::vector<const char*>& names,std::vector<float (*) (const FitElement&)>& functors)
{
    names.push_back("alpha");
    functors.push_back([](const FitElement& param) { return param.fits.dcb.alpha;  });

    names.push_back("n");
    functors.push_back([](const FitElement& param) { return param.fits.dcb.n;  });

    names.push_back("f");
    functors.push_back([](const FitElement& param) { return param.fits.dcb.f;  });

    names.push_back("sigma");
    functors.push_back([](const FitElement& param) { return param.fits.dcb.sigma1;  });

    names.push_back("x");
    functors.push_back([](const FitElement& param) { return param.fits.dcb.x;  });

    names.push_back("nSigY1S");
    functors.push_back([](const FitElement& param) { return param.fits.nSigY1S;  });

    names.push_back("nSigY2S");
    functors.push_back([](const FitElement& param) { return param.fits.nSigY2S;  });

    names.push_back("nSigY3S");
    functors.push_back([](const FitElement& param) { return param.fits.nSigY3S;  });

    names.push_back("nBkg");
    functors.push_back([](const FitElement& param) { return param.fits.nBkg;  });

    names.push_back("chk4_k1");
    functors.push_back([](const FitElement& param) { return param.fits.chk4_k1;  });

    names.push_back("chk4_k2");
    functors.push_back([](const FitElement& param) { return param.fits.chk4_k2;  });
}

std::vector<double> generateBinBoundaries(std::vector<FitElement>& configs)
{
    std::vector<double> xbins;
    xbins.push_back(configs[0].configs.cut.ptLow);
    for(int i=0;i< configs.size();i++)
    {
        xbins.push_back(configs[i].configs.cut.ptHigh);
    }
    return xbins;
}

void drawCompGraph(float (*func) (const FitElement&), std::vector<FitElement>& fits,TH1F* graph)
{
    int i=0;
    for (const auto& fit : fits)
    {
        float pt =0.5f*(fit.configs.cut.ptHigh + fit.configs.cut.ptLow);
        graph->Fill(pt,func(fit));
        graph->SetBinError(i+1,0.1);
        i++;
    }

    return;
}