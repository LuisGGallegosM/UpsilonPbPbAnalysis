#include "serialize.h"

serializer::serializer(const char* filename, serializer::iotype iot) : filename_(filename), type(iot)
{
    if (!(iot==iotype::read) || (iot==iotype::update))
        return;

    std::ifstream file(filename_);
    if(!file.is_open())
    {
        std::string error ="Error: file '";
        error += filename;
        error +="' not open/found.\n";
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

serializer::~serializer()
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

template <typename T>
void serializer::write(const char* var, T input)
{
    if (type==iotype::read)
        throw std::runtime_error("Error: File not open for writing");

    std::stringstream str;
    str << input;
    vars[var] = str.str();
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
    if (type==iotype::read)
        throw std::runtime_error("Error: File not open for writing");

    if (input)
        vars[var] = "true";
    else
        vars[var] = "false";
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

template void serializer::write(const char* var, int output);
template void serializer::write(const char* var, float output);
template void serializer::write(const char* var, unsigned long long output);