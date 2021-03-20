#ifndef BASEPLOTER
#define BASEPLOTER

#include "TCanvas.h"

#include "TextDrawer.h"

class BasePloter
{
    TextDrawer textDrawerLeft;
    TextDrawer textDrawerRight;
    TCanvas canvas;
    TPad mainPad;

    public:
    BasePloter(const std::string& name, bool isLog);
    void addLeftLine(const std::string& line);
    void addRightLine(const std::string& line);
};

#endif