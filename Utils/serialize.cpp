#include "serialize.h"

serializer::serializer(const char* filename) : filename_(filename),changed(false)
{
    std::ifstream file(filename_);
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

serializer::~serializer()
{
    if (changed)
    {
        std::ofstream file(filename_,std::ios_base::trunc);
        for (const auto &i : vars)
        {
            file << i.first << " = " << i.second << "\n";
        }
    }
}

template <typename T>
void serializer::write(const char* var, T input)
{
    std::stringstream str;
    str << input;
    vars[var] = str.str();
    changed=true;
}

template <typename T>
void serializer::read(const char* var, T& output)
{
    if (vars.find(var) != vars.end())
        std::stringstream(vars[var]) >> output;
    else
    {
        std::string error ="Error: Variable '";
        error += var;
        error +="' not found in configuration file.\n";
        throw std::invalid_argument(error);
    }     
}

void serializer::write(const char* var, bool input)
{
    if (input)
        vars[var] = "true";
    else
        vars[var] = "false";
    changed=true;
}

void serializer::read(const char* var, bool& output)
{
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
        std::string error ="Error: Variable '";
        error += var;
        error +="' not found in configuration file.\n";
        throw std::invalid_argument(error);
    }  
}

template void serializer::read(const char* var, int& output);
template void serializer::read(const char* var, float& output);
template void serializer::read(const char* var, unsigned long long& output);