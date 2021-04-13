
#include"ParameterGroup.h"
#include<string>


ParameterGroup::ParameterGroup(const Serializer* ser,const std::string& groupName) :
    ParameterGroup(groupName)
{
    auto vars = ser->getNames();
    for( const auto& var : vars)
    {
        data[var] = ser->read(var);
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
            data[var.substr(prefixWithDot.length())] = ser->read(var);
        }
    }
}

std::string ParameterGroup::getString(const std::string& name) const
{
    return data.at(name);
}

void ParameterGroup::setString(const std::string& name, const std::string& str)
{
    data[name]=str;
}

float ParameterGroup::getValue(const std::string& name) const
{
    return std::stof(data.at(name));
}

void ParameterGroup::setValue(const std::string& name, float value)
{
    data[name] = std::to_string(value);
}

int ParameterGroup::getInt(const std::string& name) const
{
    return std::stoi(data.at(name));
}

void ParameterGroup::setInt(const std::string& name, int value)
{
    data[name] = std::to_string(value);
}

bool ParameterGroup::getBool(const std::string& name) const
{
    const std::string& var= data.at(name);
    if (var=="true") return true;
    else if (var=="false") return false;
    throw std::invalid_argument(std::string("Variable '")+var+" not bool");
}

void ParameterGroup::setBool(const std::string& name, bool value)
{
    data[name] = value ? "true" : "false";
}

ParameterGroup ParameterGroup::get(const std::string& name, const std::string& prefix) const
{
    ParameterGroup g(name);
    const std::string prefixAndDot=prefix+".";
    for( const auto& var : data)
    {
        if (var.first.compare(0,prefixAndDot.length(),prefixAndDot)==0)
        {
            g.setString(var.first.substr(prefixAndDot.length()),var.second);
        }
    }
    return g;
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