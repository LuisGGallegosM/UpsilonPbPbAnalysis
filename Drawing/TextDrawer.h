#ifndef TEXTDRAWER
#define TEXTDRAWER

#include "TLatex.h"

class TextDrawer
{
    float pos_text_x;
    float pos_text_y;
    float pos_y_diff;
    float text_size;
    int text_color;
    int lines;

    public:

    TextDrawer(float pos_x,float pox_y, float fontSize = 12.0f);

    void setYDiff(float y_diff);

    void setColor(int color);

    void drawText(const char *text);
};

#endif