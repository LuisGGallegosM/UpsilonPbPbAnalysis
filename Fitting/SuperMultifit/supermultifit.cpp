
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
    std::vector<TFile*> opened_files;

    for (int i = 0; i < size; i++)
    {
        const Multifit m(multifitpaths[i]);

        TFile *file = OpenFile(m.fitfile.data(), "READ");
        TList *keys = file->GetListOfKeys();
        opened_files.push_back(file);

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

    for(auto& file: opened_files) file->Close();
    outFile->Close();

    std::cout << "\nsuccess";

    return;
}

void MultifitFileExpand(const char *multifitinput, const char *outputpath, const char* varname, int numvars, const char** vars )
{
    ParameterGroup input;

    const std::string infile=std::string(multifitinput);

    std::cout << "Expand config file parameter\n";
    std::cout << "input file "<< infile << "\n";
    std::cout << "expanding with variable named: " << varname << "\n";

    if(numvars!=0)
    {
        std::cout << "reading " << numvars << " variables\n";
        std::cout << "variable values: \n";
        for(int i=0;i<numvars;i++) std::cout << " '" << vars[i]<<"'";
    }
    

    input.deserialize(infile);

    const std::string extension= infile.substr(infile.find_last_of('.')+1);

    if(numvars!=0)
    {
        for(int i=0;i<numvars;i++)
        {
            ParameterGroup p=input;
            const std::string val= vars[i];
            const std::string outfilename=std::string(outputpath)+"/"+val+"."+extension;
            std::cout <<"output file: " << outfilename << "  for var: " << val <<"\n";
            p.remove(varname);
            p.setString(varname,val);
            p.serialize(outfilename);
        }
    }
    else
    {
        ParameterGroup* var = input.get(varname);
        int i=0;
        while( var->exists(std::to_string(i)) )
        {
            ParameterGroup p=input;
            const std::string val= var->getString(std::to_string(i));
            const std::string outfilename=std::string(outputpath)+"/"+val+"."+extension;
            std::cout <<"output file: " << outfilename << "  for var: " << val <<"\n";
            p.remove(varname);
            p.setString(varname,val);
            p.serialize(outfilename);
            i++;
        }
    }

}