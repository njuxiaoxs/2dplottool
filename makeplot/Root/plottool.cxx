#include "makeplot/plottool.h"

#include "EventLoop/Algorithm.h"
#include "EventLoop/Job.h"
#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"
#include "EventLoop/OutputStream.h"


//#include <boost/filesystem.hpp>

#include "TFile.h"
#include "string.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TGaxis.h"


#include "atlasstyle/AtlasUtils.h"
#include "atlasstyle/AtlasStyle.h"
#include "atlasstyle/AtlasLabels.h"

#include "Rtypes.h"

#include <vector>
#include <algorithm>

#include <iostream>
#include <sstream>

//ClassImp(plotobj)

using namespace std;

plotobj::plotobj():
m_config(nullptr)
{
    m_histhandlers.clear() ;
    //  m_signalsample.clear() ;
    //  m_signalsample.push_back("ZHvv125");
    //  m_signalsample.push_back("WH125");
    //  m_signalsample.push_back("ZHll125");
    //
    //  m_bkgsample.clear();
    //  m_bkgsample.push_back("ZeeB");
    //  m_bkgsample.push_back("ZeeC");
    //  m_bkgsample.push_back("ZeeL");
    //  m_bkgsample.push_back("ZmumuB");
    //  m_bkgsample.push_back("ZmumuC");
    //  m_bkgsample.push_back("ZmumuL");
    //  m_bkgsample.push_back("ZnunuB");
    //  m_bkgsample.push_back("ZnunuC");
    //  m_bkgsample.push_back("ZnunuL");
    //  m_bkgsample.push_back("WenuB");
    //  m_bkgsample.push_back("WenuC");
    //  m_bkgsample.push_back("WenuL");
    //  m_bkgsample.push_back("WmunuB");
    //  m_bkgsample.push_back("WmunuC");
    //  m_bkgsample.push_back("WmunuL");
    //  m_bkgsample.push_back("WtaunuB");
    //  m_bkgsample.push_back("WtaunuC");
    //  m_bkgsample.push_back("WtaunuL");
    //  m_bkgsample.push_back("ttbar");
    //  m_bkgsample.push_back("singletop_t");
    //  m_bkgsample.push_back("singletop_s");
    //  m_bkgsample.push_back("singletop_Wt");
    
    //  m_flavor.clear() ;
    //  m_flavor.push_back("") ;
    
}

EL::StatusCode plotobj::initialize()
{
    
    std::string configPath = "data/makeplot/plottool.config";
    
    m_config = ConfigStore::createStore( configPath );
    if (!m_config) {
        Error("makeplot","Couldn't instantiate ConfigStore");
        return 0;
    }
    
    m_config->getif< std::vector<string> >("variable",m_var) ;
    m_config->getif<bool>("isNor",m_isNor) ;
    
    m_config->getif<string>("outDir",m_output) ;
    
    m_config->getif<string>("filePath",m_filePath) ;
    
    m_config->getif<int>("Ncat", m_Ncat) ;
    
//
//    boost::filesystem::path dir(m_output);
//    if(boost::filesystem::create_directory(dir)) {
//        std::cout << "Success" << "\n";
//    }
    
    m_file = new TFile(m_filePath.c_str(),"READ") ;
    
    /* in case want to define two files
     m_config->getif<string>("signalPath",m_filesigPath) ;
     m_config->getif<string>("bkgPath"   ,m_filebkgPath) ;
     
     m_filesig = new TFile(m_filesigPath,"READ") ;
     m_filebkg = new TFile(m_filebkgPath,"READ") ;
     */
    
    m_histhandlers.resize(m_Ncat) ;
    
    initialhist() ;
    
    return EL::StatusCode::SUCCESS ;
}

EL::StatusCode plotobj::initialhist()
{
    
    const int m_cNcat = m_Ncat ;
    
//    HistHandler* histhandler(m_Ncat) ;
    
//    vector< vector<string> > sample(m_Ncat)  ;
     vector<string>  sample  ;
    
    m_config->getif<vector<string> >("njets",   m_njets) ;
    
    for(int i=0 ; i<m_cNcat ; ++i)
    {
        sample.clear() ;
        char s[256] ;
        sprintf(s, "sample_%d", i ) ;
        string ss = s ;
        m_config->getif<vector<string> >(ss , sample) ;
        
        m_histhandlers[i] = new HistHandler() ;
        
        m_histhandlers[i] ->SetSampleName(sample) ;
        m_histhandlers[i]->SetNjets(m_njets) ;
        m_histhandlers[i]->SetVar(m_var) ;
        m_histhandlers[i]->SetHist(m_file) ;
    }
    
//    m_config->getif<vector<string> >("signalsample", signalsample) ;
//    m_config->getif<vector<string> >("bkgsample",    bkgsample) ;
//    
//    string item = "default" ;
//    
//    it = find (signalsample.begin(), signalsample.end(), item) ;
//    
//    cout << "signalsample : " << signalsample.size() << endl ;
//    
//    if( it != signalsample.end() )  std::cout<<"In initialhist, signal sample as default"<< std::endl ;
//    else
//    {
//        m_signalsample.clear()  ;
//        m_signalsample.swap(signalsample) ;
//    }
//    
//    it = find(bkgsample.begin(), bkgsample.end(), "default") ;
//    if( it != bkgsample.end() )  std::cout<<"In initialhist, background sample as default"<< std::endl ;
//    else
//    {
//        m_bkgsample.clear()  ;
//        m_bkgsample.swap(bkgsample) ;
//    }
    
}

EL::StatusCode plotobj::execute()
{
    
//    for(int it = 0 ; it < m_var.size() ; ++it)
//    {
        drawhist() ;
        //    drawroc(it) ;
        //   draweff(it) ;
//    }
    
    return EL::StatusCode::SUCCESS ;
}

EL::StatusCode plotobj::drawhist()
{
    const int nhist = m_histhandlers.at(0)->m_histhandler_hist.size() ;
    const int n2dhist = m_histhandlers.at(0)->m_histhandler_2dhist.size() ;
    
    const int m_cNcat = m_histhandlers.size() ;
    
    TH1F* hist1d[nhist] ;
    TH2F* hist2d[n2dhist] ;
    
    for(int j=0 ; j< nhist  ; j++)
    {
        
        TCanvas *c1 = new TCanvas("c1","c1",900 , 600) ;
        
//        c1->SetGridy() ;
//        c1->SetLogy() ;
        
        gStyle->SetOptStat(0000);
        
        TLegend *legend = new TLegend(0.65,0.65,0.86,0.82);
        
        for(int i=0 ; i<m_cNcat ; i++)
        {
            
            
            if(i==0) m_histhandlers[i]->m_histhandler_hist.at(j)->Draw() ;
            else
            {
                m_histhandlers[i]->m_histhandler_hist.at(j)->SetLineColor(2+i);
                m_histhandlers[i]->m_histhandler_hist.at(j)->Draw("same") ;
                
            }
            
            legend->AddEntry(m_histhandlers[i]->m_histhandler_hist.at(j),m_histhandlers[i]->GetSampleName().at(0).c_str(),"l");
            
        }
        
        
        legend->Draw();
        char output[100] ;
        sprintf(output, "%s.eps",  m_var.at(j).c_str() ) ;
        c1->SaveAs(output) ;
        delete c1 ;
        
    }
    
    for(int j=0; j< n2dhist ; ++j)
    {
        
        for(int i=0 ; i<m_cNcat ; i++)
        {
            
            
            TCanvas *c1 = new TCanvas("c1","c1",900 , 600) ;
            gStyle->SetOptStat(0000);
            m_histhandlers[i]->m_histhandler_2dhist.at(j)->Draw("COLZ") ;
            
            TLine *l1 = new TLine(80,0,80,1000) ;
            l1->SetLineColor(kRed);
            l1->Draw();
            TLine *l2 = new TLine(180,0,180,1000) ;
            l2->SetLineColor(kRed);
            l2->Draw();
            TLine *l3 = new TLine(0,100,1000,100) ;
            l3->SetLineColor(kRed);
            l3->Draw();
            TLine *l4 = new TLine(0,300,1000,300) ;
            l4->SetLineColor(kRed);
            l4->Draw();
            
            
            double error ;
            
            cout <<  m_histhandlers[i]->GetSampleName().at(0).c_str() << " in region A " << m_histhandlers[i]->m_histhandler_2dhist.at(j)->IntegralAndError(8,18,0,10,error) + m_histhandlers[i]->m_histhandler_2dhist.at(j)->IntegralAndError(8,18,30,100,error) << " +- " << error <<endl   ;
            cout <<  m_histhandlers[i]->GetSampleName().at(0).c_str() << " in region B " << m_histhandlers[i]->m_histhandler_2dhist.at(j)->IntegralAndError(8,18,10,30,error) <<" +- " << error << endl   ;
            cout <<  m_histhandlers[i]->GetSampleName().at(0).c_str() << " in region C " << m_histhandlers[i]->m_histhandler_2dhist.at(j)->IntegralAndError(18,100,10,30,error) <<" +- " << error << endl ;
            char output[100] ;
            sprintf(output, "%s_%s.eps", m_histhandlers[i]->GetSampleName().at(0).c_str(), m_var.at(j).c_str() ) ;
            c1->SaveAs(output) ;
            delete c1 ;
        }
        
        
        
    }
    
//    legend->AddEntry(histbkg,"background","l");
//
//    
//    c1->SaveAs( (m_output+"/"+m_var.at(index) + ".eps").c_str() ) ;
//    c1->SaveAs( (m_output+"/"+m_var.at(index) + ".png").c_str() ) ;
//    
//    delete c1 ;
//    
//    
//    return EL::StatusCode::SUCCESS ;
    
}

EL::StatusCode plotobj::drawroc()
{
//    
//    const int Nbins = m_histsig.at(index)->GetNbinsX() ;
//    
//    double low_bin =  m_histsig.at(index)->GetXaxis()->GetXmin() ;
//    double up_bin =  m_histsig.at(index)->GetXaxis()->GetXmax() ;
//    
//    double xbin[Nbins] ;
//    
//    double y_sig[Nbins] ;
//    double y_bkg[Nbins] ;
//    
//    double y_sb[Nbins] ;
//    
//    double sig_tot = m_histsig.at(index)->Integral()  ;
//    double bkg_tot = m_histbkg.at(index)->Integral()  ;
//    
//    TCanvas *c1 = new TCanvas("c1", "the ROC curve", 200, 10 , 700 , 500) ;
//    
//    c1->SetGrid() ;
//    
//    for(int i=0 ; i<Nbins ; ++i)
//    {
//        xbin[i] = low_bin + (up_bin-low_bin)/Nbins * i ;
//        
//        y_sig[i] = m_histsig.at(index)->Integral(i,Nbins)/sig_tot ;
//        y_bkg[i] = m_histbkg.at(index)->Integral(0,i)/bkg_tot ;
//    }
//    
//    TGraph *gr_sig = new TGraph(Nbins,xbin,y_sig);
//    TGraph *gr_bkg = new TGraph(Nbins,xbin,y_bkg);
//    
//    gr_sig->SetLineColor(kRed);
//    
//    gr_sig->GetYaxis()->SetAxisColor(kRed) ;
//    gr_sig->GetYaxis()->SetTitle("signal efficiency") ;
//    gr_sig->GetYaxis()->SetTitleColor(kRed) ;
//    gr_sig->GetYaxis()->SetLabelColor(kRed) ;
//    
//    //  gr_sig->GetHistogram()->SetMaximum(gr_sig->GetMaximum() * 1.5 ) ;
//    gr_sig->Draw("ACP");
//    gr_bkg->Draw("CP");
//    
//    gr_sig->GetYaxis()->SetRangeUser(gr_sig->GetYaxis()->GetXmin() , gr_sig->GetYaxis()->GetXmax() * 1.5 ) ;
//    gPad->Modified();
//    gPad->Update() ;
//    
//    //  Float_t rightmax = 1.1*gr_bkg->GetMaximum();
//    
//    //  TF1 *f1 = new TF1("f1","y",gPad->GetUymin(),gPad->GetUymax()) ;
//    //  TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
//    //    gPad->GetUxmax(), gPad->GetUymax(),"f1",510,"+");
//    
//    //  TGaxis *axis1 = new TGaxis(gPad->GetUxmin(),gr_sig->GetYaxis()->GetXmin() ,
//    //    gPad->GetUxmin(), gr_sig->GetYaxis()->GetXmax()*1.5 ,0,gr_sig->GetYaxis()->GetXmax()*1.5,510,"+R");
//    //  axis1->SetLineColor(kRed);
//    //  axis1->SetLabelColor(kRed);
//    //  axis1->Draw();
//    //  gGaxis->SetLineColor(kRed) ;
//    
//    TGaxis *axis = new TGaxis(gPad->GetUxmax(),gr_sig->GetYaxis()->GetXmin() ,
//                              gPad->GetUxmax(), gr_sig->GetYaxis()->GetXmax()*1.5 ,0,gr_sig->GetYaxis()->GetXmax()*1.5,510,"+L");
//    //  axis->SetLineColor(kRed);
//    //  axis->SetLabelColor(kRed);
//    axis->SetTitle("background rejection");
//    axis->SetLabelFont(62);
//    axis->Draw();
//    
//    TLegend *legend = new TLegend(0.65,0.65,0.86,0.82);
//    legend->AddEntry(gr_sig,"signal ","l");
//    legend->AddEntry(gr_bkg,"background","l");
//    legend->Draw();
//    
//    c1->SaveAs( (m_output+"/"+m_var.at(index) + "_roc.eps").c_str() ) ;
//    c1->SaveAs( (m_output+"/"+m_var.at(index) + "_roc.png").c_str() ) ;
//    
//    delete gr_sig ;
//    delete gr_bkg ;
//    
//    delete legend ;
//    
//    delete c1 ;
//    
//    TCanvas *c2 = new TCanvas("c2", "the ROC curve", 200, 10 , 700 , 500) ;
//    
//    c2->SetGrid() ;
//    
//    for(int i=0 ; i<Nbins ; ++i)
//    {
//        //    xbin[i] = low_bin + (up_bin-low_bin)/Nbins * i ;
//        y_sb[i] = m_histbkg.at(index)->Integral(i,Nbins)>0 ? m_histsig.at(index)->Integral(i,Nbins)/sqrt(m_histbkg.at(index)->Integral(i,Nbins)) : 0 ;
//    }
//    
//    TGraph *gr_sb = new TGraph(Nbins,xbin,y_sb);
//    
//    gr_sb->Draw("ACP");
//    
//    gr_sb->GetYaxis()->SetRangeUser(gr_sb->GetYaxis()->GetXmin() , gr_sb->GetYaxis()->GetXmax() * 1.5 ) ;
//    gPad->Modified();
//    //  gr_sb->SetMaximum(gr_sig->GetMaximum() * 1.5 ) ;
//    gPad->Update() ;
//    
//    TLegend *legend2 = new TLegend(0.65,0.65,0.86,0.82);
//    legend2->AddEntry(gr_sb,"signal efficiency ","l");
//    legend2->Draw();
//    
//    c2->SaveAs( (m_output+"/"+m_var.at(index) + "_sb.eps").c_str() ) ;
//    c2->SaveAs( (m_output+"/"+m_var.at(index) + "_sb.png").c_str() ) ;
//    
//    delete gr_sb ;
//    
//    delete legend2 ;
//    
//    delete c2 ;
//    
//    return EL::StatusCode::SUCCESS ;
}


EL::StatusCode plotobj::draweff( )
{
    return EL::StatusCode::SUCCESS ;
}



string plotobj::GetHistName(string  sample, string flavor,  string njet, string var)
{
    string histname = sample + flavor + "_pretag" + njet + "_vpt0_" + var ;
    return histname ;
}

//string plotobj::GetHistName(string  sample, string flavor,  string njet, string var, string sys="Normal")
//{
////  string histname = sample + flavor + "_pretag" + njet + "_vpt0_" + var ;
//  string histname = var + "_" + sys + sample +  "_pretag" + njet + "_vpt0_" + var ;
//  return histname ;
//}

TFile* plotobj::GetFile(string filename)
{
    
    TFile *f1 =  new TFile(filename.c_str());
    if(f1==0) std::cout << "wrong input" << std::endl;
    return f1;
}


TH1F* plotobj::GetHist(TFile* file, string histname)
{
    
    //  TH1F *h1 =(file)?(TH1F*)file->Get(histname):0;
    TH1F *h1 =(TH1F*)file->Get(histname.c_str());
    
    if(h1==0)
    {
        std::cout << "no hist found !!!"<< std::endl ;
    }
    
    return h1;
}

TH1F* plotobj::Add(TH1F* h1, TH1F* h2)
{
    
    h1->Add(h2) ;
    //  TH1F *h ;
    //  .Add(h2);
    return h1;
    
}

TH1F* plotobj::plot(TFile* filename, const vector<string> sample, const vector<string> flavor, const vector<string> njet, string var)
{
//    
//    const int tot_hist = sample.size() * flavor.size() * njet.size()  ;
//    
//    int i_hist = 0 ;
//    
//    //  cout << sample.size() << flavor.size() << njet.size() << endl ;
//    //  cout << " tot_hist : " << tot_hist << endl ;
//    
//    TH1F *hist[tot_hist] ;
//    TH1F *allhist ;
//    
//    //  TFile* file = GetFile(filename) ;
//    
//    for(unsigned int isamp(0); isamp<sample.size() ; isamp++)
//    {
//        string sample_name = sample.at(isamp);
//        
//        for(unsigned int iflavor(0); iflavor<flavor.size(); iflavor++)
//        {
//            string flavor_name = flavor.at(iflavor);
//            
//            for(unsigned int injet(0); injet<njet.size(); injet++)
//            {
//                string njet_name = njet.at(injet);
//                
//                
//                string histname = GetHistName(sample_name, flavor_name, njet_name, var) ;
//                
//                //        std::cout << "histname : " << histname << std::endl ;
//                //        string histname = GetHistName(sample_name, flavor_name, njet_name, var) ;
//                
//                //        std::cout << "i hists : " << i_hist << std::endl ;
//                
//                hist[i_hist] = GetHist(filename, histname) ;
//                
//                if(i_hist==0) allhist  = hist[i_hist] ;
//                
//                else if(i_hist>=1) allhist->Add((TH1F*)hist[i_hist]) ;
//                
//                i_hist = i_hist+1 ;
//            }
//        }
//    }
//    
//    return allhist ;
}



void plotobj::Draw(TH1F* hist)
{
    hist->Draw();
}


