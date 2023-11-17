
#include"YieldAgg.h"
#include<vector>
#include"TFile.h"

#include "../../Utils/Helpers/Helpers.h"
#include "../../OniaBase/Params/Params.h"
#include "../Common/AccEffAux.h"
#include "../../Drawing/Drawing.h"
#include "Helpers.h"

void YieldAgg(const char** filenames, int file_num, const char* outputdir)
{
    std::cout <<"\nyield agregation\n";
    std::vector<TFile*> files;
    for(int i=0;i<file_num;i++)
    {
        files.push_back( OpenFile(filenames[i],"READ") );
        std::cout << "reading file :" <<filenames[i] << "\n";
    }
    std::cout << "total files: "<< file_num <<"\n";

    std::vector<TH1*> ths;

    for(int i=0;i<file_num;i++)
    {
        ths.push_back( Get<TH1>(files[i],"signal_nSigY1S") );
    }

    const std::string outdir=outputdir;

    TFile* outputfile=OpenFile((outdir+"/yagg.root").data(),"RECREATE");
    std::cout << "output to file :" << outputdir << "\n";

    TH2D output("yields","yields",zbins_n,zbins_low,zbins_high,jtptbins_n,jtptbins_low,jtptbins_high);

    for(int i=0;i<jtptbins_n;i++)
    {
        for(int j=0;j<zbins_n;j++)
        {
            output.SetBinContent(j+1,i+1, ths[i]->GetBinContent(j+1) );
            output.SetBinError(j+1,i+1, ths[i]->GetBinError(j+1));
        }
    }

    outputTH(&output,"yields1S",outdir);

    output.Write();

    outputfile->Close();
    delete outputfile;
}