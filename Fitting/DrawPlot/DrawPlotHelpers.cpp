
#include"DrawPlotHelpers.h"

std::string makePrettier(const std::string& name)
{
    if (name=="pt") return "p_{t}";
    return name;
}

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
        str.clear();
        if (cuts->exists(var+".low")) str+= Form("%.2f < ",cuts->getFloat(var+".low"));
        str+=makePrettier(var);
        if (cuts->exists(var+".high")) str+= Form(" < %.2f",cuts->getFloat(var+".high"));
        tdrawer->drawText(str.data());
    }
}
