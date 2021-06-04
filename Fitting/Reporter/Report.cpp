
#include<fstream>
#include"../../Utils/Serialization/Serialization.h"
#include"../../Drawing/Drawing.h"

struct VarAndFilename
{
    std::string varName;
    std::string filename;
    bool isFixed;
};

std::vector<VarAndFilename> getVarNames(const ParameterGroup* fit);
void generateVarTable(std::ofstream& latexOut,const std::vector<VarAndFilename>& varnames,const std::vector<ParameterGroup>& fits,const char** fitfilepaths );
void generatePlots(std::ofstream& latexOut,const std::vector<VarAndFilename>& varnames , int size);
void drawCut(const ParameterGroup* cuts, std::ofstream& latexOut);

void Report(const char* multifitpath,const char* outputpath,int size,const char** fitfilepaths)
{

    std::string outputstr(outputpath);
    outputstr+= "/report/report.tex";
    std::ofstream latexOut(outputstr);

    std::ifstream latexHeader("./HelperScripts/examplefiles/latexHeaders.tex");

    latexOut << latexHeader.rdbuf();

    latexHeader.close();

    ParameterGroup multifit;
    multifit.deserialize(multifitpath);
    const ParameterGroup* cuts = multifit.get("cut");
    const auto cutVars=cuts->getSubgroupNames();

    drawCut(cuts,latexOut);

    std::vector<ParameterGroup> fits;
    fits.reserve(size);
    for(int i=0;i<size;i++)
    {
        fits.emplace_back();
        std::string fitfile= std::string(fitfilepaths[i])+'/'+basename(fitfilepaths[i])+".fit";
        fits[i].deserialize(fitfile);
    }

    const auto varnames= getVarNames(&fits[0]);

    generateVarTable(latexOut,varnames,fits,fitfilepaths);

    generatePlots(latexOut,varnames,size);

    latexOut << "\n\\end{document}";

    latexOut.close();
}

void generateVarTable(std::ofstream& latexOut,const std::vector<VarAndFilename>& varnames,const std::vector<ParameterGroup>& fits,const char** fitfilepaths )
{
    for(int startindex=0;startindex< fits.size();startindex+=4)
    {
        int endindex=startindex+4;
        if (endindex > fits.size()) endindex= fits.size()-startindex;

        latexOut << "\n\\begin{frame}\n\t\\begin{center}\n\t\\begin{tabular} { ";
        for (int i=0;i<varnames.size();i++) { latexOut << "|c "; }
        latexOut << "}\n";

        latexOut << "\t\t- ";
        for(int i=startindex;i < endindex;i++) 
        {
            std::string fitbasename = basename(fitfilepaths[i]);
            latexOut << "& " << fitbasename; 
        }
        latexOut << "\\\\\n";

        for(const auto var : varnames)
        {
            latexOut << "\t\t\\detokenize{" << var.varName << "}";
            for(int i=startindex;i < endindex;i++)
            {
                latexOut << " & " << fits[i].getFloat(var.varName+".value");
                if ((fits[i].exists(var.varName+".fixed")) && (fits[i].getBool(var.varName+".fixed"))) latexOut << '*';
            }
            latexOut << "\\\\\n";
        }
        latexOut << "\\end{tabular}\n\\end{center}\n\\end{frame}\n";
    }
}

void generatePlots(std::ofstream& latexOut,const std::vector<VarAndFilename>& varnames , int size)
{
    for(int i=0;i<size;i++)
    {
        latexOut << "\n\t\\drawFit{"<< i << "}{ linear scale }" ;
        latexOut << "\n\t\\drawFitLog{"<< i << "}{ log scale }" ;
    }

    for(const auto var : varnames)
    {
        if (!var.isFixed)
            latexOut << "\n\t\\draw{\\detokenize{../" << var.filename <<".pdf}}{\\detokenize{" << var.varName << "}}";
    }
}

std::vector<VarAndFilename> getVarNames(const ParameterGroup* fit)
{
    std::vector<VarAndFilename> getters;
    const std::vector<std::string> subgroups= {"signal","bkg"};
    for(auto subgroup : subgroups )
    {
        const auto vars=fit->get(subgroup)->getSubgroupNames();
        for(auto& var : vars)
        {
            if (fit->exists(subgroup+"."+var+".fixed"))
            {
                bool isFixed=fit->getBool(subgroup+"."+var+".fixed");
                getters.push_back( VarAndFilename{subgroup+"."+var,subgroup+"_"+var, isFixed});
            }
        }
    }

    return getters;
}

void drawCut(const ParameterGroup* cuts, std::ofstream& latexOut)
{
    const auto vars= cuts->getSubgroupNames();
    std::string str;
    latexOut << "\n\\begin{frame}\n\tQuality cuts: \n\t\\begin{itemize}\n";
    for (auto var : vars)
    {
        bool lowexists=cuts->exists(var+".low");
        bool highexist=cuts->exists(var+".high");
        std::string pretty=var;
        std::replace(pretty.begin(),pretty.end(),'_','-');
        str.clear();
        if (lowexists)
        {
            if (highexist)
            {
                str= Form("%.2f < %s < %.2f",cuts->getFloat(var+".low"),pretty.data(),cuts->getFloat(var+".high"));  
            }
            else
            {
                str= Form("%s > %.2f",pretty.data(),cuts->getFloat(var+".low"));  
            }
        }
        else
        {
            str= Form("%s < %.2f",pretty.data(),cuts->getFloat(var+".high"));  
        }
        latexOut << "\t\t\\item $" << str << "$ \n";
    }
    latexOut << "\t\\end{itemize}\n";
    latexOut << "\\end{frame}\n";
}
