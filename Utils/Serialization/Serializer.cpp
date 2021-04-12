#include "Serializer.h"

#include <iostream>
#include <fstream>
#include <sstream>

Serializer::Serializer()
{
}


Serializer::Serializer(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        std::string error ="Error: cannot open file '"+filename+"'.\n";
        throw std::runtime_error(error);
    }

    std::string str;
    char varName[64];
    char varValue[64];
    while (std::getline(file,str))
    {
        if(str.empty()) continue;
        if(str[0] =='/') continue;
        int result =sscanf(str.data(),"%s = %s",varName,varValue);
        if (result ==2)
        {
            vars[varName]= varValue;
        }
    };
}

void Serializer::serialize(const std::string& filename) const
{
    std::ofstream file(filename,std::ios_base::trunc);
    for (const auto &i : vars)
    {
        file << i.first << " = " << i.second << "\n";
    }
}

void Serializer::write(const std::string& name, const std::string& var)
{
    vars[name] = var;
}

const std::string& Serializer::read(const std::string& name) const
{
    return vars.at(name);
}

std::vector<std::string> Serializer::getNames() const
{
    std::vector<std::string> names;
    names.reserve(vars.size());
    for(const auto& var : vars)
    {
        names.push_back(var.first);
    }
    return names;
}