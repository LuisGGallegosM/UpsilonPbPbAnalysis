
#include<iostream>
#include"../../Utils/Serialization/Serialization.h"

std::vector<std::vector<float>> generateVals(const std::vector<std::string>& diffVars, const ParameterGroup& multifit);
void generateFile( const std::string& varName,const std::vector<float>& diffVarValues,const ParameterGroup* fixedGroup, const std::string& outputpath );

void MultifitGen(const char* multifitfile, const char* outputpath)
{
    std::cout << "\nMULTIFIT FITCONF GENERATOR\n";
    std::cout << "Reading input file: " << multifitfile <<'\n';
    std::cout << "Writing to directory: " << outputpath <<'\n';

    ParameterGroup multifit;
    multifit.deserialize(multifitfile);

    const std::vector<std::string> diffVars = getStringVector(multifit,"diffVar.name");

    const std::vector<std::vector<float>> diffVarValues = generateVals(diffVars,multifit);

    ParameterGroup fixedGroup=multifit;
    fixedGroup.remove("diffVar");

    generateFile(diffVars[0],diffVarValues[0],&fixedGroup,outputpath);
}

void generateFile( const std::string& varName,const std::vector<float>& diffVarValues,const ParameterGroup* fixedGroup, const std::string& outputpath )
{
    int size=diffVarValues.size()-1;
    
    for(int i=0;i<size;i++)
    {
        ParameterGroup g=*fixedGroup;

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