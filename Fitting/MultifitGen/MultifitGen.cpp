
#include<iostream>
#include"../../Utils/Serialization/Serialization.h"

void MultifitGen(const char* multifitfile, const char* outputpath)
{
    std::cout << "\nMULTIFIT FITCONF GENERATOR\n";
    std::cout << "Reading input file: " << multifitfile <<'\n';
    std::cout << "Writing to directory: " << outputpath <<'\n';

    ParameterGroup multifit;
    multifit.deserialize(multifitfile);

    std::vector<ParameterGroup> outputs;

    const std::vector<std::string> diffVar = getStringVector(multifit,"diffVar.name");
    const std::vector<ParameterGroup*> diffVarValueRows = getGroupVector(multifit,"diffVar.values");

    ParameterGroup fixedGroup=multifit;
    fixedGroup.remove("diffVar");

    std::vector<float> diffVarValues= getFloatVector(*(diffVarValueRows[0]),"0");
    int size=diffVarValues.size()-1;
    
    for(int i=0;i<size;i++)
    {
        ParameterGroup g=fixedGroup;

        g.setFloat("cut."+diffVar[0]+".low",diffVarValues[i]);
        g.setFloat("cut."+diffVar[0]+".high",diffVarValues[i+1]);

        std::string filename=std::string(outputpath)+"/fit"+std::to_string(i)+".fitconf";
        g.serialize(filename);
        std::cout << "generated file: " << filename << '\n';
    }
}