#include<fstream>
#include"../../Utils/Serialization/Serialization.h"
#include"../Common/Common.h"

std::vector<ParameterGroup> getFits(const std::vector<std::string>& fitpaths);
float weightedAverage(const std::vector<ParameterGroup>& fits, const std::string& varname);

void ExtraAnalysis(const char* multifitpath, const char* multifitinput, const char* multifitoutput)
{
    ParameterGroup multifit;
    multifit.deserialize(std::string(multifitpath)+"/config.multifit");

    int size= getFloatVector(multifit,"diffVar.values.0").size()-1;

    std::vector<std::string> fitpaths;
    fitpaths.reserve(size);
    for(int i=0;i<size;i++) 
        fitpaths.push_back(std::string(multifitpath)+"/fit"+std::to_string(i)+"/fit"+std::to_string(i)+".fit");
    std::sort(fitpaths.begin(),fitpaths.end());

    std::vector<ParameterGroup> fits=getFits(fitpaths);

    std::cout << "Custom Analysis\ninput files:\n";
    for(const auto& fitp : fitpaths) std::cout << ">>" << fitp << "\n";

    std::cout << "OUTPUT:\n";

    ParameterGroup output;

    ParameterGroup multifitout;
    multifitout.deserialize(multifitinput);

    for(const auto& var : { "signal.alpha", "signal.n" , "signal.sigma","signal.f1","signal.f2","signal.x1","signal.x2" })
    {
        float val=weightedAverage(fits,var);
        std::cout << "average " << var <<": " << val << "\n";
        output.setFloat(var,val);
        multifitout.setFloat(std::string(var)+".value",val);
    }

    const std::string outputfilename=std::string(multifitpath)+"/an.txt";
    output.serialize(outputfilename,"avg");
    multifitout.serialize(multifitoutput);

}

float weightedAverage(const std::vector<ParameterGroup>& fits, const std::string& varname)
{
    float numerator=0.0f;
    float denominator=0.0f;

    for(const auto& fit : fits)
    {
        float value= fit.getFloat(varname+".value");
        float error= fit.getFloat(varname+".error");

        numerator+=value/error;
        denominator+=1.0f/error;

    }
    return numerator/denominator;
}

std::vector<ParameterGroup> getFits(const std::vector<std::string>& fitpaths)
{
    int size=fitpaths.size();
    std::vector<ParameterGroup> fits;
    fits.reserve(size);

    for(int i=0;i<size;i++)
    {
        fits.emplace_back();
        std::string fitfile= fitpaths[i];
        fits.back().deserialize(fitfile);
    }
    return fits;
}

// float weightedAverage(const float* values,const float* errors, int num)
// {
//     float numerator=0.0f;
//     float denominator=0.0f;

//     for(int i=0;i<num;i++)
//     {
//         float value= values[i];
//         float error= errors[i];

//         numerator+=value/error;
//         denominator+=1.0f/error;

//     }
//     return numerator/denominator;
// }