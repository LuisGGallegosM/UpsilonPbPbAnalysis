#ifndef PARAMETERGROUP
#define PARAMETERGROUP

#include "Serialization.h"

class ParameterGroup
{
    std::map<std::string,ParameterGroup> subgroups;
    std::map<std::string,std::string> data;
    std::string name;
    std::string basePrefix;

    const std::string& read(const std::string& name) const;
    void write(const std::string& name, const std::string& value);

    public:
    ParameterGroup(const std::string& groupName) :
        data(),basePrefix(groupName+"."), name(groupName)
    {
    }

    ParameterGroup(const Serializer* ser,const std::string& groupName);
    ParameterGroup(const Serializer* ser,const std::string& groupName, const std::string& prefix);

    std::string getString(const std::string& name) const;
    float getFloat(const std::string& name) const;
    bool getBool(const std::string& name) const;
    int getInt(const std::string& name) const;

    void setString(const std::string& name, const std::string& str);
    void setFloat(const std::string& name, float value);
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);

    std::vector<std::string> getNames() const;
    ParameterGroup* get(const std::string& name);
    void save(Serializer* ser) const;
};

void serialize(const ParameterGroup& g, const std::string& filename);

#endif