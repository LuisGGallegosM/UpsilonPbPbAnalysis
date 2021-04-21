#ifndef PARAMETERGROUP
#define PARAMETERGROUP

#include "Serialization.h"

class ParameterGroup
{
    std::map<std::string,ParameterGroup> subgroups;
    std::map<std::string,std::string> data;

    const std::string& read(const std::string& name) const;
    void write(const std::string& name, const std::string& value);

    public:
    ParameterGroup() = default;

    bool exists(const std::string& name) const;

    std::string getString(const std::string& name) const;
    float getFloat(const std::string& name) const;
    bool getBool(const std::string& name) const;
    int getInt(const std::string& name) const;

    void setString(const std::string& name, const std::string& str);
    void setFloat(const std::string& name, float value);
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);

    std::vector<std::string> getVarNames() const;
    std::vector<std::string> getSubgroupNames() const;
    ParameterGroup* get(const std::string& name);
    const ParameterGroup* get(const std::string& name) const;
    void addGroup(const ParameterGroup&);
    void addGroup(const ParameterGroup&,const std::string& name);

    void deserialize(const std::string& filename,const std::string& prefix="");
    void deserialize(const Serializer* ser,const std::string& prefix="");
    void serialize(const std::string& filename,const std::string& prefix="") const;
    void serialize(Serializer* ser,const std::string& prefix="") const;
};

#endif