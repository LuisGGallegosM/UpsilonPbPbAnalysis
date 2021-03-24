#include "Serializer.h"

#include <iostream>
#include <fstream>
#include <sstream>


Serializer::Serializer(const std::string& filename, Serializer::iotype iot) : filename_(filename), type(iot)
{
    if (!(iot==iotype::read) || (iot==iotype::update))
        return;

    std::ifstream file(filename_);
    if(!file.is_open())
    {
        std::string error ="Error: file '"+filename+"' not open/found.\n";
        throw std::runtime_error(error);
    }

    std::string str;
    char varName[42];
    char varValue[42];
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

Serializer::~Serializer()
{
    if ((type== iotype::write) || (type==iotype::update))
    {
        std::ofstream file(filename_,std::ios_base::trunc);
        for (const auto &i : vars)
        {
            file << i.first << " = " << i.second << "\n";
        }
    }
}

void Serializer::addPrefix(const std::string& prefix)
{
    prefixes.push_back(prefix);
    currentPrefix.clear();
    for(const auto& str : prefixes) currentPrefix.append(str+'.');
}

void Serializer::removePrefix()
{
    prefixes.pop_back();
    currentPrefix.clear();
    for(const auto& str : prefixes) currentPrefix.append(str+'.');
}

template <typename T>
void Serializer::write(const std::string& varname, T input)
{
    std::string var = currentPrefix + varname;
    if (type==iotype::read)
        throw std::runtime_error("Error: File not open for writing");

    std::stringstream str;
    str << input;
    vars[var] = str.str();
}

template <typename T>
void Serializer::read(const std::string& varname, T& output)
{
    std::string var = currentPrefix + varname;
    if (vars.find(var) != vars.end())
        std::stringstream(vars[var]) >> output;
    else
    {
        std::string error ="Error: Variable '"+var+"' not found in configuration file.\n";
        throw std::invalid_argument(error);
    }     
}

template <typename T>
void Serializer::read(const std::string& varname, T& output, T defaultValue)
{
    std::string var = currentPrefix + varname;
    if (vars.find(var) != vars.end())
        std::stringstream(vars[var]) >> output;
    else
    {
        output = defaultValue;
        std::cout << "WARNING: No value found for "<< varname << " default value " << defaultValue <<" used.\n";
    }     
}

void Serializer::write(const std::string& varname, bool input)
{
    std::string var = currentPrefix + varname;
    if (type==iotype::read)
        throw std::runtime_error("Error: File not open for writing");

    if (input)
        vars[var] = "true";
    else
        vars[var] = "false";
}

void Serializer::read(const std::string& varname, bool& output)
{
    std::string var = currentPrefix + varname;
    if (vars.find(var) != vars.end())
    {
        if(vars[var] == "true")
            output=true;
        else if (vars[var] == "false")
            output=false;
        else
            throw std::invalid_argument(std::string("Variable '")+var+" not bool");
    }
    else
    {
        std::string error ="Error: Variable '" +var+"' not found in configuration file.\n";
        throw std::invalid_argument(error);
    }  
}

void Serializer::read(const std::string& varname, bool& output, bool defaultValue)
{
    std::string var = currentPrefix + varname;
    if (vars.find(var) != vars.end())
    {
        if(vars[var] == "true")
            output=true;
        else if (vars[var] == "false")
            output=false;
        else
            throw std::invalid_argument(std::string("Variable '")+var+" not bool");
    }
    else
    {
        output=defaultValue;
        std::string def= defaultValue? "true" :"false";
        std::cout << "WARNING: No value found for "<<varname << " default value " << def << " used.\n";
    }  
}

template void Serializer::read(const std::string& var, int& output);
template void Serializer::read(const std::string& var, float& output);
template void Serializer::read(const std::string& var, unsigned long long& output);
template void Serializer::read(const std::string& var, std::string& output);

template void Serializer::read(const std::string& var, int& output, int defaultValue);
template void Serializer::read(const std::string& var, float& output, float defaultValue);
template void Serializer::read(const std::string& var, unsigned long long& output, unsigned long long defaultValue);
template void Serializer::read(const std::string& var, std::string& output, std::string defaultValue );

template void Serializer::write(const std::string& var, int output);
template void Serializer::write(const std::string& var, float output);
template void Serializer::write(const std::string& var, unsigned long long output);
template void Serializer::write(const std::string& var, const std::string output);