#include<fstream>
#include"../../Utils/Serialization/Serialization.h"
#include"../Common/Common.h"

std::vector<ParameterGroup> getFits(const std::vector<std::string>& fitpaths);
float weightedAverage(const std::vector<ParameterGroup>& fits, const std::string& varname);

void ExtraAnalysis(const char* multifitpath)
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

    for(const auto& var : { "alpha", "n" , "sigma","f1","f2","x1","x2" })
    {
        float val=weightedAverage(fits,var);
        std::cout << "average " << var <<": " << val << "\n";
        output.setFloat(var,val);
    }

    const std::string outputfilename=std::string(multifitpath)+"/an.txt";
    output.serialize(outputfilename,"avg");

}

float weightedAverage(const std::vector<ParameterGroup>& fits, const std::string& varname)
{
    float numerator=0.0f;
    float denominator=0.0f;

    for(const auto& fit : fits)
    {
        float value= fit.getFloat("signal."+varname+".value");
        float error= fit.getFloat("signal."+varname+".error");

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