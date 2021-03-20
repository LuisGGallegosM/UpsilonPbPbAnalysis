
#include "BasePloter.h"


BasePloter::BasePloter(const std::string& name, bool isLog):
    textDrawerLeft(0.22f,0.8f,9.0f), textDrawerRight(0.45,0.8),
    canvas(name.data(),name.data(),4,45,550,520), mainPad("graph_pad","mass fit", 0, 0.45, 0.98, 1.0)
{
    canvas.SetLeftMargin(2.6);
    canvas.cd();

    mainPad.SetTicks(1,1);
    mainPad.SetLeftMargin(0.15);
    if (isLog) mainPad.SetLogy();
    mainPad.Draw();
}

void BasePloter::addLeftLine(const std::string& line)
{
    textDrawerLeft.drawText(line.data());
}

void BasePloter::addRightLine(const std::string& line)
{
    textDrawerRight.drawText(line.data());
}