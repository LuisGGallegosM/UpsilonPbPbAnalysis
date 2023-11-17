
#include "Helpers.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TH2.h"
#include "../../Drawing/Drawing.h"
#include "../../Utils/Helpers/Helpers.h"
#include"TPaveText.h"

std::string generatePostfix(int i)
{
   return "_jtpt"+ std::to_string(int(jtptbins_low+jtptbins_width*i))+ std::to_string(int(jtptbins_low+jtptbins_width*(i+1)));
}

std::string generateJtptString(int i)
{
   return std::to_string(int(jtptbins_low+jtptbins_width*i))+ "< jt_{pt} <"+ std::to_string(int(jtptbins_low+jtptbins_width*(i+1)));
}

void DrawHist(TMatrixD* hist,const std::string& name)
{
    TH2D temp(*hist);
    DrawHist(&temp,name);
}

void DrawHistM(TMatrixD* hist,const std::string& name)
{
    TH2D temp(*hist);
    DrawHistM(&temp,name);
}

void DrawHist(TH1* hist,const std::string& name)
{
    auto c1 = new TCanvas();
    hist->SetStats(0);
    hist->Sumw2();
    hist->Draw("colz");
    c1->Draw();
    c1->SaveAs( name.data());
}

void DrawHistM(TH1* hist,const std::string& name)
{
    const int totalbins=zbins_n*jtptbins_n;
    auto c1 = new TCanvas();
    c1->SetGrid();
    hist->SetStats(0);
    hist->Sumw2();
    hist->Draw("colz");
    std::vector<TLine> lines;
    lines.reserve((jtptbins_n-1)*2);
    for(int i=0;i<jtptbins_n-1;i++)
    {
        const float pos=zbins_n*(i+1);
        lines.push_back(TLine(pos,0.0f,pos,totalbins));
        lines.back().SetLineColor(kBlack);
        lines.back().SetLineWidth(2.0f);
        lines.back().Draw();
        lines.push_back(TLine(0.0f,pos,totalbins,pos));
        lines.back().SetLineColor(kBlack);
        lines.back().SetLineWidth(2.0f);
        lines.back().Draw();
    }

    std::vector<TPaveText> texts;
    texts.reserve(jtptbins_n*2);

    hist->GetXaxis()->SetTitle("measured");
    hist->GetXaxis()->SetLabelSize(0);
    hist->GetXaxis()->SetNdivisions(12);

    hist->GetYaxis()->SetTitle("truth");
    hist->GetYaxis()->SetLabelSize(0);
    hist->GetYaxis()->SetNdivisions(12);

    for(int i=0;i<jtptbins_n;i++)
    {
        std::string str;
        const float v1=zbins_n*(i+0.25f);
        const float v2=zbins_n*(i+0.75f);

        texts.push_back(TPaveText(v1,-0.7,v2,-0.1,"NB"));
        str= std::to_string( (i+1)*10 )+" < jt_{pt} < "+std::to_string( (i+2)*10 );
        texts.back().AddText(str.data());
        texts.back().Draw();

        texts.push_back(TPaveText(-1.5,v1,-0.1,v2,"NB"));
        str= std::to_string( (i+1)*10 )+" < jt_{pt} < "+std::to_string( (i+2)*10 );
        texts.back().AddText(str.data());
        texts.back().GetLineWith(str.data())->SetTextAngle(90.0);
        texts.back().Draw();
    }

    c1->Draw();
    c1->SaveAs( name.data());
}

void DrawHist2(TH1* hist1, TH1* hist2,const std::string& name)
{
    auto c1 = new TCanvas();
    hist1->SetStats(0);
    hist1->Sumw2();
    hist1->SetLineColor(1);
    hist1->Draw("colz");
    
    hist2->SetStats(0);
    hist2->Sumw2();
    hist2->SetLineColor(2);
    hist2->Draw("same");
    c1->Draw();
    c1->SaveAs( name.data());
}

void outputTH(TH1* th, const std::string& name, const std::string& dir)
{
    th->SetName(name.data());
    th->SetTitle(name.data());
    std::string namout=name +".png";
    DrawHist(th,dir+'/'+namout);
    th->Write();
}

void outputTH(TH1* th, const std::string& name, const std::string& title, const std::string& dir)
{
    th->SetName(name.data());
    th->SetTitle(title.data());
    std::string namout=name +".png";
    DrawHist(th,dir+'/'+namout);
    th->Write();
}

void outTGraph(const std::vector<float>& x, const std::vector<float>& y, const std::string& name, const std::string& title, const std::string& xlabel, const std::string& ylabel,const std::string& outdir)
{
    TGraph xiGraph(x.size(),x.data(),y.data());
    xiGraph.SetName(name.data());
    xiGraph.SetTitle(title.data());
    const std::string outputfilename= outdir+"/graph";
    writeToCanvas(&xiGraph,xlabel.data(),ylabel.data(),outputfilename.data(),"ACP");
}

std::vector<TH1D*> generateProjectionsZ(TH2D* measured,const std::string& outfilename, const std::string& prefix)
{
    std::vector<TH1D*> measured_z;
    for(int j=0;j<jtptbins_n;j++)
    {
        std::string postfix=generatePostfix(j);
        std::string obj_name= prefix+ postfix;
        auto mea_z= measured->ProjectionX(obj_name.data(),1+j,1+j,"eo");
        DrawHist(mea_z,ReplaceExtension(outfilename.data(),(obj_name+".png").data()));
        mea_z->Write();
        measured_z.push_back(mea_z);
    }
    return measured_z;
}