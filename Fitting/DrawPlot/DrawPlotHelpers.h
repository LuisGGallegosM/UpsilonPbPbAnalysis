#ifndef DRAWPLOTHELPERS
#define DRAWPLOTHELPERS

#include "../../Drawing/TextDrawer.h"
#include "../../Utils/Serialization/Serialization.h"

void drawParams(const ParameterGroup* signal, TextDrawer* tdrawer);

void drawCut(const ParameterGroup* cuts, TextDrawer* tdrawer);

#endif