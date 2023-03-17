
#include "Helpers.h"
#include "TCanvas.h"
#include "TH2.h"
#include "../../Drawing/Drawing.h"
#include "../../Utils/Helpers/Helpers.h"

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

void DrawHist(TH1* hist,const std::string& name)
{
    auto c1 = new TCanvas();
    hist->SetStats(0);
    hist->Sumw2();
    hist->Draw("colz");
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

std::vector<TH1D*> generateProjectionsZ(TH2D* measured,const std::string& outfilename)
{
    std::vector<TH1D*> measured_z;
    for(int j=0;j<jtptbins_n;j++)
    {
        std::string postfix=generatePostfix(j);
        std::string obj_name= "measured_z"+ postfix;
        auto mea_z= measured->ProjectionX(obj_name.data(),1+j,1+j,"eo");
        DrawHist(mea_z,ReplaceExtension(outfilename.data(),(obj_name+".png").data()));
        mea_z->Write();
        measured_z.push_back(mea_z);
    }
    return measured_z;
}