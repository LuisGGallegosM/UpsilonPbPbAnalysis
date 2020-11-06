#ifndef SERIALIZE
#define SERIALIZE

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

class serializer
{
    private:
    bool changed;
    std::string filename_;
    std::map<std::string,std::string> vars;

    public:
    serializer(const char* filename);

    ~serializer();

    template <typename T>
    void write(const char* var, T input);

    template <typename T>
    void read(const char* var, T& output);

    void write(const char* var, bool input);

    void read(const char* var, bool& output);
};

#endif