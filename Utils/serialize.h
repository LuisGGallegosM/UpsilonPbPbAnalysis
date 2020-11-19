#ifndef SERIALIZE
#define SERIALIZE

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

class serializer
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
    serializer(const std::string& filename, serializer::iotype iot= serializer::iotype::read);

    ~serializer();

    void addPrefix(const std::string& prefix);
    void removePrefix();

    template <typename T>
    void write(const std::string& varname, T input);

    template <typename T>
    void read(const std::string& varname, T& output);

    void write(const std::string& varname, bool input);

    void read(const std::string& varname, bool& output);
};


#if defined(__CLING__)
#include "../Utils/serialize.cpp"
#endif

#endif