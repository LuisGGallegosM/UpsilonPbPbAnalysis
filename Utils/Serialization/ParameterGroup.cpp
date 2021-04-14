
#include"ParameterGroup.h"
#include<string>


ParameterGroup::ParameterGroup(const Serializer* ser,const std::string& groupName) :
    ParameterGroup(groupName)
{
    auto vars = ser->getNames();
    for( const auto& var : vars)
    {
        write(var,ser->read(var));
    }
}

const std::string& ParameterGroup::read(const std::string& name) const
{
    int index= name.find_first_of('.');
    if (index==std::string::npos)
    {
        return data.at(name);
    }
    else
    {
        return subgroups.at(name.substr(0,index)).read(name.substr(index+1));
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
        auto it =subgroups.emplace(subg,subg);
        it.first->second.write(subsubg,value);
    }   
}

ParameterGroup::ParameterGroup(const Serializer* ser,const std::string& groupName, const std::string& prefix) :
    ParameterGroup(groupName)
{
    const std::string prefixWithDot=prefix+"."+groupName+".";
    const auto vars = ser->getNames();
    for( const auto& var : vars)
    {
        if( var.compare(0,prefixWithDot.length(), prefixWithDot)==0)
        {
            write(var.substr(prefixWithDot.length()),ser->read(var));
        }
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

std::vector<std::string> ParameterGroup::getNames() const
{
    std::vector<std::string> names;
    names.reserve(data.size());
    for(const auto& name : data)
    {
        names.push_back(name.first);
    }
    return names;
}

void ParameterGroup::save(Serializer* ser) const
{
    for (const auto& var : data)
    {
        ser->write(var.first,var.second);
    }
}

void serialize(const ParameterGroup& g, const std::string& filename)
{
    Serializer ser;
    g.save(&ser);
    ser.serialize(filename);
}