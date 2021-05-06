#include "Serializer.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::vector<std::string> tokenize(const std::string& str);
void deserializeArray(std::map<std::string,std::string>& vars, const std::vector<std::string>& tokens);

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
    readStream(file);
}

Serializer::Serializer(std::istream& stream)
{
    readStream(stream);
}

void Serializer::readStream(std::istream& stream)
{
    std::string str;
    while (std::getline(stream,str))
    {
        if(str.empty()) continue;
        if((str[0] =='/') || (str[0]=='#')) continue;
        auto tokens= tokenize(str);
        if (tokens.size()>2)
        {
            if ((tokens.size()==3) && (tokens[1]=="="))
            {
                vars[tokens[0]]= tokens[2];
            }
            else
            {
                if ((tokens[2]=="[") && (tokens[tokens.size()-1]=="]") )
                {
                    deserializeArray(vars,tokens);
                }
            }
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

void deserializeArray(std::map<std::string,std::string>& vars, const std::vector<std::string>& tokens)
{
    for(int i=3;i<(tokens.size()-1);i++)
    {
        std::string varname=tokens[0]+"."+std::to_string(i-3);
        vars[varname]= tokens[i];
    }
}

std::vector<std::string> tokenize(const std::string& str)
{
    std::vector<std::string> tokens;
    int lastTokenPos=0;
    int i=0;
    std::string token;
    for(i=0;i<str.length();i++)
    {
        if (str[i]==' ')
        {
            if (lastTokenPos!= i)
            {
                tokens.push_back( str.substr(lastTokenPos,i-lastTokenPos) );
                lastTokenPos=i+1;
            }
        }        
    }
    if (lastTokenPos!= i)
        tokens.push_back( str.substr(lastTokenPos,i-lastTokenPos) );
    return tokens;
}