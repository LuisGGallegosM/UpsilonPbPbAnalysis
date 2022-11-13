
#include"ParameterGroup.h"
#include<string>

bool ParameterGroup::exists(const std::string& name) const
{
    int index= name.find_first_of('.');
    if (index==std::string::npos)
    {
        if (data.find(name)!=data.end()) return true;
        return subgroups.find(name)!=subgroups.end();
    }
    else
    {
        const std::string str=name.substr(0,index);
        auto found= subgroups.find(str);
        if (found==subgroups.end()) return false;
        return found->second.exists(name.substr(index+1));
    }
}

const std::string& ParameterGroup::read(const std::string& name) const
{
    int index= name.find_first_of('.');
    if (index==std::string::npos)
    {
        auto found= data.find(name);
        if (found==data.end()) throw std::invalid_argument("Error: variable '"+name+"' not found.");
        return data.at(name);
    }
    else
    {
        const std::string str=name.substr(0,index);
        auto found= subgroups.find(str);
        if (found==subgroups.end()) throw std::invalid_argument("Error: subgroup '"+name+"' not found.");
        return found->second.read(name.substr(index+1));
    }
}

void ParameterGroup::write(const std::string& name, const std::string& value)
{
    int index= name.find_first_of('.');
    if (index==std::string::npos)
    {
        data[name]=value;
    }
    else
    {
        const std::string subg= name.substr(0,index);
        const std::string subsubg= name.substr(index+1);
        auto it =subgroups.emplace(subg,ParameterGroup());
        it.first->second.write(subsubg,value);
    }   
}

std::string ParameterGroup::getString(const std::string& name) const
{
    return read(name);
}

void ParameterGroup::setString(const std::string& name, const std::string& str)
{
    write(name,str);
}

float ParameterGroup::getFloat(const std::string& name) const
{
    return std::stof(read(name));
}

void ParameterGroup::setFloat(const std::string& name, float value)
{
    write(name, std::to_string(value));
}

int ParameterGroup::getInt(const std::string& name) const
{
    return std::stoi(read(name));
}

void ParameterGroup::setInt(const std::string& name, int value)
{
    write(name,std::to_string(value));
}

bool ParameterGroup::getBool(const std::string& name) const
{
    const std::string& var= read(name);
    if (var=="true") return true;
    else if (var=="false") return false;
    throw std::invalid_argument(std::string("Variable '")+var+" not bool");
}

void ParameterGroup::setBool(const std::string& name, bool value)
{
    write(name,value ? "true" : "false");
}

const ParameterGroup* ParameterGroup::get(const std::string& name) const
{
    int index= name.find_first_of('.');

    if (index==std::string::npos)
    {
        auto found=subgroups.find(name);
        if (found != subgroups.end())
            return &(found->second);
        else
            throw std::invalid_argument("subgroup "+name+" not found.");
    }
    else
    {
        auto found=subgroups.find(name.substr(0,index));
        const std::string sub=name.substr(index+1);
        if (found!= subgroups.end())
            return found->second.get(sub);
        else
            throw std::invalid_argument("subgroup '"+sub+"' in '"+ name +"' not found.");
    }
}

ParameterGroup* ParameterGroup::get(const std::string& name)
{
    int index= name.find_first_of('.');

    if (index==std::string::npos)
    {
        auto found=subgroups.find(name);
        if (found != subgroups.end())
            return &(found->second);
        else
            throw std::invalid_argument("subgroup "+name+" not found.");
    }
    else
    {
        auto found=subgroups.find(name.substr(0,index));
        const std::string sub=name.substr(index+1);
        if (found!= subgroups.end())
            return found->second.get(sub);
        else
            throw std::invalid_argument("subgroup '"+sub+"' in '"+ name +"' not found.");
    }
}

ParameterGroup* ParameterGroup::set(const std::string& name,const ParameterGroup* val)
{
    int index= name.find_first_of('.');

    if (index==std::string::npos)
    {
        subgroups[name]=*val;
        return &(subgroups.at(name));
    }
    else
    {
        const std::string gname=name.substr(0,index);
        auto found=subgroups.find(gname);
        const std::string sub=name.substr(index+1);

        return subgroups[gname].set(sub,val);
    }
}

void ParameterGroup::addGroup(const ParameterGroup& g)
{
    for(const auto& var : g.data)
        data[var.first]=var.second;
    for(const auto& subg : g.subgroups)
        addGroup(subg.second,subg.first);
}

void ParameterGroup::addGroup(const ParameterGroup& g,const std::string& name)
{
    subgroups[name].addGroup(g);
}

void ParameterGroup::remove(const std::string& name)
{
    int index= name.find_first_of('.');
    if (index==std::string::npos)
    {
        data.erase(name);
        subgroups.erase(name);
    }
    else
    {
        const std::string str=name.substr(0,index);
        auto found= subgroups.find(str);
        if (found!=subgroups.end())
            found->second.remove(name.substr(index+1));
    }
}

std::vector<std::string> ParameterGroup::getVarNames() const
{
    std::vector<std::string> names;
    names.reserve(data.size());
    for(const auto& name : data)
    {
        names.push_back(name.first);
    }
    return names;
}

std::vector<std::string> ParameterGroup::getSubgroupNames() const
{
    std::vector<std::string> names;
    names.reserve(subgroups.size());
    for(const auto& name : subgroups)
    {
        names.push_back(name.first);
    }
    return names;
}

void ParameterGroup::serialize(const std::string& filename,const std::string& prefix) const
{
    Serializer ser;
    serialize(&ser,prefix);
    ser.serialize(filename);
}

void ParameterGroup::deserialize(const std::string& filename,const std::string& prefix)
{
    Serializer ser(filename);
    deserialize(&ser,prefix);
}

void ParameterGroup::serialize(Serializer* ser,const std::string& prefix) const
{
    const std::string prefixWithDot= prefix.empty() ? "" : prefix+".";
    for (const auto& var : data)
    {
        ser->write(prefixWithDot+ var.first,var.second);
    }
    for(const auto& subg : subgroups)
    {
        subg.second.serialize(ser,prefixWithDot+subg.first);
    }
}

void ParameterGroup::deserialize(const Serializer* ser,const std::string& prefix)
{
    const std::string prefixWithDot= prefix.empty() ? "" : prefix+".";
    auto vars = ser->getNames();
    for( const auto& var : vars)
    {
        write(prefixWithDot+var,ser->read(var));
    }
}