
#include"Latexer.h"

void Latexer::beginFrame()
{
    latexOut << "\n\\begin{frame}\n";
}
void Latexer::endFrame()
{
    latexOut << "\n\\end{frame}\n";
}

void Latexer::addLine(const std::string& line)
{
    latexOut << "\n\t" << line;
}

Latexer::Latexer(const std::string& filename, const std::string& headerfilename):
latexOut(filename.data())
{
    std::ifstream latexHeader(headerfilename);
    latexOut << latexHeader.rdbuf();
}

void Latexer::beginDoc()
{
    latexOut << "\n\\begin{document}";
}

void Latexer::endDoc()
{
    latexOut << "\n\\end{document}";
}

void Latexer::genTable(const std::vector<std::vector<std::string>>& table)
{
    std::string str("\\begin{tabular} {");
    for (auto v : table[0]) str.append("|c ");
    str.append("}");
    addLine("\\begin{center}");
    addLine(str);
    for(auto row : table)
    {
        int i=0;
        latexOut << '\n';
        for(int i=0;i<(3);i++) latexOut << '\t';
        for(auto value : row)
        {
            if (i!=0) latexOut << " & ";
            latexOut << value;
            i++;
        }
        latexOut << "\\\\\n";
    }
    addLine("\\end{tabular}");
    addLine("\\end{center}");
}