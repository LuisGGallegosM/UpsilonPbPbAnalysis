
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

std::vector<ParameterGroup*> getGroupVector(ParameterGroup& p, const std::string& name)
{
    std::vector<ParameterGroup*> out;
    int i=0;
    while( p.exists(name+"."+std::to_string(i)) )
    {
        out.push_back(p.get(name+"."+std::to_string(i)));
        i++;
    }
    return out;
}

void setFloatVector(ParameterGroup& p, const std::string& name, const std::vector<float>& vec)
{
    for(int i=0;i<vec.size();i++)
    {
        p.setFloat(name+"."+std::to_string(i),vec[i]);
    }
}
void setStringVector(ParameterGroup& p, const std::string& name,const std::vector<std::string>& vec)
{
    for(int i=0;i<vec.size();i++)
    {
        p.setString(name+"."+std::to_string(i),vec[i]);
    }
}
void setIntVector(ParameterGroup& p, const std::string& name,const std::vector<int>& vec)
{
    for(int i=0;i<vec.size();i++)
    {
        p.setInt(name+"."+std::to_string(i),vec[i]);
    }
}