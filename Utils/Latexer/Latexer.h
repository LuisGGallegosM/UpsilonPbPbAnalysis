#ifndef LATEXER
#define LATEXER

#include<fstream>
#include<vector>

class Latexer
{
    std::ofstream latexOut;

    public:

    Latexer(const std::string& filename, const std::string& headerfilename);

    void beginFrame();
    void endFrame();
    void beginDoc();
    void endDoc();

    void addLine(const std::string& line);

    void genTable(const std::vector<std::vector<std::string>>& table);


};
#endif