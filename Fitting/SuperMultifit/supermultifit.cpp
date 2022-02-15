
#include "supermultifit.h"
#include "../Common/Common.h"
#include "../../Utils/Helpers/Helpers.h"
#include "../../Drawing/Drawing.h"

#include <vector>
#include <algorithm>

#include "TH1.h"
#include "TCanvas.h"
#include "RooDataSet.h"
#include "RooAddPdf.h"
#include "TKey.h"

struct Multifit
{
    std::string directory;
    std::string fitfile;
    std::string basen;
    ParameterGroup multifitConf;

    Multifit(const std::string &dir) : directory(dir), fitfile(dir + "/fit.root"), basen( getBasename(dir) )
    {
        multifitConf.deserialize(directory + "/config.multifit");
    }
};


void Supermultifit(const char **multifitpaths, const char *outputpath, int size)
{
    std::cout << "\nLLR TEST\n";

    for (int i = 0; i < size; i++)
    {
        std::cout << "Input multifit directory: " << multifitpaths[i] << " \n";
    }

    std::cout << "Output to folder : " << outputpath << '\n';

    std::map<std::string, std::vector<TH1*>> histograms;

    for (int i = 0; i < size; i++)
    {
        const Multifit m(multifitpaths[i]);

        TFile *file = OpenFile(m.fitfile.data(), "READ");
        TList *keys = file->GetListOfKeys();

        for (int j = 0; j < keys->GetSize(); j++)
        {
            TKey *key = dynamic_cast<TKey *>(keys->At(j));
            if (key)
            {
                const char* name=key->GetName();
                TH1 *hist = dynamic_cast<TH1 *>(file->Get(name));
                if(hist)
                {
                    hist->SetName(m.basen.data());
                    hist->SetTitle(m.basen.data());
                    histograms[name].push_back(hist);
                }
            }
        }
    }

    const std::string outfilename=std::string(outputpath)+"/supermultifit.root";

    TFile* outFile=OpenFile(outfilename.data(),"CREATE");

    for(auto& hists: histograms)
    {

        auto& hist= hists.second;

        const std::string outname=outputpath+("/"+hists.first)+".pdf";

        std::cout << "Detected parameter :" << hists.first <<"\n";

        writeToCanvas(hist,hists.first,hist[0]->GetXaxis()->GetTitle(),hist[0]->GetYaxis()->GetTitle(),outname);

    }

    std::cout << "\nsuccess";

    return;
}