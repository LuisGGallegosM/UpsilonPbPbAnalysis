#ifndef PARAMETERGROUP
#define PARAMETERGROUP

#include "Serialization.h"

class IParameterGroup
{
    public:
    virtual std::string getString(const std::string& name) const=0;
    virtual float getValue(const std::string& name) const=0;
    virtual bool getBool(const std::string& name) const=0;

    virtual void setString(const std::string& name, const std::string& str)=0;
    virtual void setValue(const std::string& name, float value)=0;
    virtual void setBool(const std::string& name, bool value)=0;

    virtual ~IParameterGroup() = default;
};

class ParameterGroup : public IParameterGroup
{
    std::map<std::string,std::string> data;
    std::string name;
    std::string basePrefix;

    public:
    ParameterGroup(const std::string& groupName) :
        data(),basePrefix(groupName+"."), name(groupName)
    {
    }

    ParameterGroup(Serializer* ser,const std::string& groupName);
    ParameterGroup(Serializer* ser,const std::string& groupName, const std::string& prefix);

    std::string getString(const std::string& name) const;
    float getValue(const std::string& name) const;
    bool getBool(const std::string& name) const;

    void setString(const std::string& name, const std::string& str);
    void setValue(const std::string& name, float value);
    void setBool(const std::string& name, bool value);

    std::vector<std::string> getNames() const;
    ParameterGroup get(const std::string& name, const std::string& prefix) const;
    void save(Serializer* ser) const;
};

class ParameterSubgroup : public IParameterGroup
{
    ParameterGroup* group;
    std::string prefixing;
    public:
    ParameterSubgroup(ParameterGroup* g,const std::string& prefix): group(g), prefixing(prefix+".")
    {}

    std::string getString(const std::string& name) const { return group->getString(prefixing+name);}
    float getValue(const std::string& name) const { return group->getValue(prefixing+name);}
    bool getBool(const std::string& name) const { return group->getBool(prefixing+name);}

    void setString(const std::string& name, const std::string& str) { group->setString(prefixing+name,str);}
    void setValue(const std::string& name, float value) { group->setValue(prefixing+name,value);}
    void setBool(const std::string& name, bool value) { group->setBool(prefixing+name,value);}
};

void serialize(const ParameterGroup& g, const std::string& filename);

#endif