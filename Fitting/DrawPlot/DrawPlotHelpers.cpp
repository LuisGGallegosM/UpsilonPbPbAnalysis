
#include"DrawPlotHelpers.h"

void drawParams(const ParameterGroup* signal, TextDrawer* tdrawer)
{
    for (auto var : signal->getSubgroupNames())
    {
        if (!signal->getBool(var+".fixed"))
        {
            tdrawer->drawText( Form("%s=%.3f #pm %.3f",var.data(),signal->getFloat(var+".value"),signal->getFloat(var+".error")));
        }
    }
}

void drawCut(const ParameterGroup* cuts, TextDrawer* tdrawer)
{
    const auto vars= cuts->getSubgroupNames();
    std::string str;
    for (auto var : vars)
    {
        bool lowexists=cuts->exists(var+".low");
        bool highexist=cuts->exists(var+".high");
        std::string pretty=makePrettier(var);
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
        tdrawer->drawText(str.data());
    }
}
