#include "TextDrawer.h"
#include<map>

const std::map<std::string,std::string> prettyMap=
{
    {"pt","p_{t}^{#mu#mu}"},
    {"eta","|#eta^{#mu#mu}|"},
    {"y","|y^{#mu#mu}|"},
    {"jt_pt", "jet p_{T}"},
    {"jt_eta", "|jet #eta|"},
    {"mass", "m^{#mu#mu}"},
    {"z^2","z^{2}"}
};

TextDrawer::TextDrawer(float pos_x,float pox_y, float fontSize) :
    pos_text_x(pos_x), pos_text_y(pox_y),
    pos_y_diff(0.055), text_size(fontSize),
    text_color(1),lines(0)
{

}

void TextDrawer::setYDiff(float y_diff)
{
    pos_y_diff= y_diff;
}

void TextDrawer::setColor(int color)
{
    text_color= color;
}

void TextDrawer::drawText(const char *text)
{
   TLatex *tex = new TLatex(pos_text_x,pos_text_y-pos_y_diff*lines,text);
   tex->SetTextFont(63);
   tex->SetTextSize(text_size);
   tex->SetTextColor(text_color);
   tex->SetLineWidth(1);
   tex->SetNDC();
   tex->Draw("same");
   ++lines;
}

std::string makePrettier(const std::string& name)
{
    if (prettyMap.find(name)!= prettyMap.end()) 
        return prettyMap.at(name);
    return name;
}