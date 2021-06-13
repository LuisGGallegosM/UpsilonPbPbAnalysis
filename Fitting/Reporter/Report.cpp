
#include<fstream>
#include"../../Utils/Serialization/Serialization.h"
#include"../../Utils/Latexer/Latexer.h"
#include"../../Drawing/Drawing.h"
#include"../Common/Common.h"

struct VarAndFilename
{
    std::string varName;
    std::string filename;
    bool isFixed;
};

std::vector<VarAndFilename> getVarNames(const ParameterGroup* fit);
void generateVarTable(Latexer& latexOut,const std::vector<VarAndFilename>& varnames,const std::vector<ParameterGroup>& fits,const std::vector<std::string>& fitfilepaths );
void generatePlots(Latexer& latexOut,const std::vector<VarAndFilename>& varnames , int size);
void drawCut(const ParameterGroup* cuts, Latexer& latexOut);

void Report(const char* multifitpath,const char* outputpath,int size,const char** fitfilepaths)
{
    std::string outputstr(outputpath);
    outputstr+= "/report/report.tex";

    Latexer latexOut(outputstr,"./HelperScripts/examplefiles/latexHeaders.tex");
    latexOut.beginDoc();

    ParameterGroup multifit;
    multifit.deserialize(multifitpath);
    const ParameterGroup* cuts = multifit.get("cut");
    const auto cutVars=cuts->getSubgroupNames();

    drawCut(cuts,latexOut);

    std::vector<ParameterGroup> fits;
    fits.reserve(size);

    std::vector<std::string> fitpaths;
    fitpaths.reserve(size);
    for(int i=0;i<size;i++) fitpaths.push_back(fitfilepaths[i]);
    std::sort(fitpaths.begin(),fitpaths.end());
    
    for(int i=0;i<size;i++)
    {
        fits.emplace_back();
        std::string fitfile= fitpaths[i]+'/'+basename(fitpaths[i].data())+".fit";
        fits.back().deserialize(fitfile);
    }

    const auto varnames= getVarNames(&fits[0]);

    generateVarTable(latexOut,varnames,fits,fitpaths);

    generatePlots(latexOut,varnames,size);

    latexOut.endDoc();

}

void generateVarTable(Latexer& latexOut,const std::vector<VarAndFilename>& varnames,const std::vector<ParameterGroup>& fits,const std::vector<std::string>& fitfilepaths )
{
    for(int startindex=0;startindex< fits.size();startindex+=4)
    {
        int endindex=startindex+4;
        if (endindex > fits.size()) endindex= fits.size();

        std::vector<std::vector<std::string>> table;

        table.emplace_back();
        table.back().push_back("var");
        for(int i=startindex;i < endindex;i++) 
        {
            table.back().push_back(basename(fitfilepaths[i].data()));
        }

        for(const auto var : varnames)
        {
            table.emplace_back();
            table.back().push_back("\\detokenize{" + var.varName + "}");
            for(int i=startindex;i < endindex;i++)
            {
                std::string value=std::to_string(fits[i].getFloat(var.varName+".value"));
                if ((fits[i].exists(var.varName+".fixed")) && (fits[i].getBool(var.varName+".fixed"))) 
                    value.append("*");
                table.back().push_back(value);
            }
        }
        table.emplace_back();
        table.back().push_back("bkg.type");
        for(int i=startindex;i < endindex;i++)
        {
            table.back().push_back(fits[i].getString("bkg.type"));
        }

        latexOut.beginFrame();
        latexOut.genTable(table);
        latexOut.addLine("*fixed parameter");
        latexOut.endFrame();
    }
}

void generatePlots(Latexer& latexOut,const std::vector<VarAndFilename>& varnames , int size)
{
    for(int i=0;i<size;i++)
    {
        std::string str = std::to_string(i);
        latexOut.addLine("\\drawFit{"+ str + "}{ Fit" + str +" linear scale }") ;
        latexOut.addLine("\\drawFitLog{"+ str + "}{ Fit" + str +" log scale }") ;
    }

    for(const auto var : varnames)
    {
        if (!var.isFixed)
            latexOut.addLine("\\draw{\\detokenize{../" + var.filename +".pdf}}{\\detokenize{" + var.varName + "}}");
    }
}

std::vector<VarAndFilename> getVarNames(const ParameterGroup* fit)
{
    std::vector<VarAndFilename> getters;
    const std::vector<std::string> subgroups= {"signal",bkgName};
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

void drawCut(const ParameterGroup* cuts, Latexer& latexOut)
{
    const auto vars= cuts->getSubgroupNames();
    std::string str;
    latexOut.beginFrame();
    latexOut.addLine("Quality cuts: \n\t\\begin{itemize}");
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
        latexOut.addLine("\t\\item $"+str+"$");
    }
    latexOut.addLine("\t\\end{itemize}");
    latexOut.endFrame();
}
