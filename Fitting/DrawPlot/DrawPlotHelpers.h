#ifndef DRAWPLOTHELPERS
#define DRAWPLOTHELPERS

#include "../../Drawing/TextDrawer.h"
#include "../../Utils/Serialization/Serialization.h"

void drawCut(const ParameterGroup* cuts, TextDrawer* tdrawer);

void drawParams(const ParameterGroup* signal, TextDrawer* tdrawer);

#endif