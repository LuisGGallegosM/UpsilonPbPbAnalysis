
#include"ParameterGroupAux.h"


std::vector<float> getFloatVector(const ParameterGroup& p, const std::string& name)
{
    std::vector<float> out;
    int i=0;
    while( p.exists(name+"."+std::to_string(i)) )
    {
        out.push_back(p.getFloat(name+"."+std::to_string(i)));
        i++;
    }
    return out;
}

std::vector<std::string> getStringVector(const ParameterGroup& p, const std::string& name)
{
    std::vector<std::string> out;
    int i=0;
    while( p.exists(name+"."+std::to_string(i)) )
    {
        out.push_back(p.getString(name+"."+std::to_string(i)));
        i++;
    }
    return out;
}

std::vector<int> getIntVector(const ParameterGroup& p, const std::string& name)
{
    std::vector<int> out;
    int i=0;
    while( p.exists(name+"."+std::to_string(i)) )
    {
        out.push_back(p.getInt(name+"."+std::to_string(i)));
        i++;
    }
    return out;
}