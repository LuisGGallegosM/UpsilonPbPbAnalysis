
#include"AccEffSys.h"

#include"../../Utils/Helpers/Helpers.h"
#include "../../Utils/Serialization/Serialization.h"
#include "../../Drawing/Drawing.h"

#include<string>
#include<iostream>
#include<fstream>

#include"TH1.h"

void acceff_systematics(const char* multifitpaths, const char* outputpath, const char* start_str, const char* length_str)
{
    const int start_index= atoi(start_str);
    const int length= atoi(length_str);

    std::string basepath=multifitpaths;

    const float nominals[4]= { 9281.245117, 15710.746094, 42996.648438, 152614.750000 };

    const float xup=5.00f;
    const float xdown=-5.00f;

    //output file
    std::string outfilename = std::string(outputpath) +"/acceff.root";
    TFile* outputfile = OpenFile(outfilename.data(),"CREATE");

    std::string str_ranges[5]={"0.00","0.25","0.50","0.75","1.00"};

    TH1F* ths_acc[4];
    TH1F* ths_eff[4];
    for(int i=0;i<4;i++)
    {
        std::string name_range= str_ranges[i] +" < z^2 < "+str_ranges[i+1];

        std::string name_acc="yield distribution acc systematic";
        ths_acc[i]= new TH1F((name_acc+"_"+std::to_string(i)).data(),(name_acc+" "+name_range).data(),50,xdown,xup);
        ths_acc[i]->SetStats(false);
        //ths_acc[i]->Sumw2();

        std::string name_eff="yield distribution eff systematic";
        ths_eff[i]= new TH1F((name_eff+"_"+std::to_string(i)).data(),(name_eff+" "+name_range).data(),50,xdown,xup);
        ths_eff[i]->SetStats(false);
        //ths_eff[i]->Sumw2();
    }

    std::vector<float> sys_acc[4];
    std::vector<float> sys_eff[4];

    for(int i=start_index;i<length;i++)
    {
        for(int j=0;j<4;j++)
        {
            std::string file_acc = basepath + "/multifit_baseline_jt2030_acc_"+ std::to_string(i) 
                + "/fit" + std::to_string(j) + "/fit"+std::to_string(j)+".fit";

            std::cout << "Reading input file: " << file_acc <<'\n';

            ParameterGroup config;
            config.deserialize(file_acc);

            const float val = config.getFloat("signal.nSigY1S.value");

            const float diff=val - nominals[j];

            const float systematic = 100.0f*diff/nominals[j];

            sys_acc[j].push_back(val);

            ths_acc[j]->Fill(systematic);
            
        }
    }

    for(int i=start_index;i<length;i++)
    {
        for(int j=0;j<4;j++)
        {
            std::string file_eff = basepath + "/multifit_baseline_jt2030_eff_"+ std::to_string(i) 
                + "/fit" + std::to_string(j) + "/fit"+std::to_string(j)+".fit";

            std::cout << "Reading input file: " << file_eff <<'\n';

            ParameterGroup config;
            config.deserialize(file_eff);

            const float val = config.getFloat("signal.nSigY1S.value");

            const float diff=val - nominals[j];

            const float systematic = 100.0f*diff/nominals[j];

            sys_eff[j].push_back(val);

            ths_eff[j]->Fill(systematic);
            
        }
    }

    for(int i=0;i<4;i++)
    {
        writeToCanvas(ths_acc[i],"deviation %","N",std::string(outputpath)+"/acc_fits_"+std::to_string(i));
        writeToCanvas(ths_eff[i],"deviation %","N",std::string(outputpath)+"/eff_fits_"+std::to_string(i));
        ths_acc[i]->Write();
        ths_eff[i]->Write();
    }

    std::cout << "\n";

    std::ofstream ofile(std::string(outputpath)+"/syst.txt");

    for(int i=0;i<4;i++)
    {
        std::ofstream outfile_acc(std::string(outputpath)+"/acc"+std::to_string(i)+".csv");
        float total=0.0f;
        int n=0;
        for(auto f : sys_acc[i])
        {
            outfile_acc << f << "\n";
            if((fabs(f - nominals[i])/nominals[i]) < 0.1f)
            {
                n++;
                total+=(f - nominals[i])*(f - nominals[i]);
            }
        }
        ofile << "acc "<< i << ": " << 100*sqrt(total/n)/nominals[i] << "\n";

        total=0.0f;
        n=0;

        std::ofstream outfile_eff(std::string(outputpath)+"/eff"+std::to_string(i)+".csv");
        for(auto f : sys_eff[i])
        {
            outfile_eff << f << "\n";
            if((fabs(f - nominals[i])/nominals[i]) < 0.1f)
            {
                n++;
                total+=(f - nominals[i])*(f - nominals[i]);
            }
        }
        ofile << "eff "<< i << ": " << 100*sqrt(total/n)/nominals[i] << "\n";
    }
    outputfile->Close();

}