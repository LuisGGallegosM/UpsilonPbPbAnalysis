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
    public:
    enum class iotype { read, write, update};

    private:
    std::string filename_;
    std::map<std::string,std::string> vars;
    std::vector<std::string> prefixes;
    iotype type;
    std::string currentPrefix;
    
    public:
    Serializer(const std::string& filename, Serializer::iotype iot= Serializer::iotype::read);

    ~Serializer();

    void addPrefix(const std::string& prefix);
    void removePrefix();

    template <typename T>
    void write(const std::string& varname, T input);

    template <typename T>
    void read(const std::string& varname, T& output);

    template <typename T>
    void read(const std::string& varname, T& output, T defaultValue);

    void write(const std::string& varname, bool input);

    void read(const std::string& varname, bool& output);

    void read(const std::string& varname, bool& output, bool defaultValue);
};


#if defined(__CLING__)
#include "../Utils/serialize.cpp"
#endif

#endif