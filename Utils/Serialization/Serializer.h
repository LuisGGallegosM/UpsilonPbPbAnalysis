#ifndef SERIALIZER
#define SERIALIZER

#include <string>
#include <map>
#include <vector>

/**
 * @brief class used to serialize objects
 * (transform a c++ object variables into a human readable text file)
 */
class Serializer
{
    private:
    std::map<std::string,std::string> vars;
    
    public:
    Serializer();
    Serializer(const std::string& filename);

    void write(const std::string& name, const std::string& value);
    const std::string& read(const std::string& name) const;
    std::vector<std::string> getNames() const;
    bool exists(const std::string& varname) const { return vars.find(varname)!=vars.end(); }

    void serialize(const std::string& filename) const;

    template <typename T>
    void read(const std::string& varname, T& output)
    {
    }

    template <typename T>
    void write(const std::string& varname, T input)
    {
    }

    void addPrefix(const std::string& prefix) {};
    void removePrefix() {};
};


#if defined(__CLING__)
#include "../Utils/serialize.cpp"
#endif

#endif