#ifndef SERIALIZE
#define SERIALIZE

#include <iostream>
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
    iotype type;
    
    public:
    serializer(const char* filename, serializer::iotype iot= serializer::iotype::read);

    ~serializer();

    template <typename T>
    void write(const char* var, T input);

    template <typename T>
    void read(const char* var, T& output);

    void write(const char* var, bool input);

    void read(const char* var, bool& output);
};


#if defined(__CLING__)
#include "../Utils/serialize.cpp"
#endif

#endif