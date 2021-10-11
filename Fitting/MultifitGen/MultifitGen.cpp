
#include<iostream>
#include"../../Utils/Serialization/Serialization.h"

std::vector<std::vector<float>> generateVals(const std::vector<std::string>& diffVars, const ParameterGroup& multifit);
void generateFile( const std::string& varName,const std::vector<float>& diffVarValues,const ParameterGroup& multifit, const std::string& outputpath);
std::vector<std::string> findDiffParameters(const ParameterGroup& multifit);

void MultifitGen(const char* multifitfile, const char* outputpath)
{
    std::cout << "\nMULTIFIT FITCONF GENERATOR\n";
    std::cout << "Reading input file: " << multifitfile <<'\n';
    std::cout << "Writing to directory: " << outputpath <<'\n';

    ParameterGroup multifit;
    multifit.deserialize(multifitfile);

    const std::vector<std::string> diffVars = getStringVector(multifit,"diffVar.name");

    const std::vector<std::vector<float>> diffVarValues = generateVals(diffVars,multifit);

    generateFile(diffVars[0],diffVarValues[0],multifit,outputpath);
}

void generateFile( const std::string& varName,const std::vector<float>& diffVarValues,const ParameterGroup& multifit, const std::string& outputpath)
{
    const auto diffParams= findDiffParameters(multifit);
    
    for(int i=0;i<(diffVarValues.size()-1);i++)
    {
        ParameterGroup g=multifit;
        for(int j=0;j<diffParams.size();j++)
        {
            float value = getFloatIndex(multifit,diffParams[j],i);
            g.remove(diffParams[j]);
            g.setFloat(diffParams[j],value);
        }
        g.remove("diffVar");

        g.setFloat("cut."+varName+".low",diffVarValues[i]);
        g.setFloat("cut."+varName+".high",diffVarValues[i+1]);

        std::string filename=outputpath+"/fit"+std::to_string(i)+".fitconf";
        g.serialize(filename);
        std::cout << "generated file: " << filename << '\n';
    }
}

std::vector<std::vector<float>> generateVals(const std::vector<std::string>& diffVars, const ParameterGroup& multifit)
{
    std::vector<std::vector<float>> diffVarValues;
    for(int i=0;i<diffVars.size();i++)
    {
        diffVarValues.push_back(getFloatVector(multifit,"diffVar.values."+std::to_string(i)));

        std::cout << "differential variable: " << diffVars[i] << " = [";
        for(int j=0;j<diffVarValues[i].size();j++)
        {
            std::cout << " " << diffVarValues[i][j] << " ";
        }
        std::cout << "] \n";
    }
    return diffVarValues;
}

std::vector<std::string> findDiffParameters(const ParameterGroup& multifit)
{
    std::vector<std::string> result;
    for(auto g : {"signal","bkg"})
    {
        auto pg = multifit.get(g);
        auto params= pg->getSubgroupNames();
        for(auto param : params)
        {
            if (pg->exists(param+".value.0"))
                result.push_back(std::string(g)+"."+param+".value");
        }
    }
    return result;
}