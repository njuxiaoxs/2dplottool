#include "makeplot/HistHandler.h"

#include "TH1F.h"

#include <string>
#include <vector>
#include <algorithm>

#include <iostream>

using namespace std ;

HistHandler::HistHandler()
{
//     m_histhandler_hist.clear() ;
//     m_histhandler_2dhist.clear() ;
//    
//     m_histhandler_hist_component.clear() ;
//     m_histhandler_samplename.clear() ;
//     m_histhandler_histname.clear() ;
//     m_histhandler_njets.clear() ;
//     m_histhandler_var.clear() ;
}


vector<TH1F*> HistHandler::GetHistComponent(vector<string> component_name)
{
}

void HistHandler::MergeHistComponent(vector<TH1F*> hist_component)
{
}

void HistHandler::Rebin(TH1F* hist , int x)  
{
}

void HistHandler::Rebin(TH2F* hist , int x , int y ) 
{
}

//void HistHandler::SetHistName(vector<string> hist_name)
//{
//    m_histhandler_hist_name.clear() ;
//    m_histhandler_hist_name.swap(hist_name) ;
//}

void HistHandler::SetSampleName(vector<string> t_samplename)
{

    m_histhandler_samplename.clear() ;
    m_histhandler_samplename.swap(t_samplename) ;
    
}

void HistHandler::SetNjets(vector<string> njets)
{
    m_histhandler_njets.clear() ;
    m_histhandler_njets.swap(njets);
}

void HistHandler::SetVar(vector<string> var)
{
    m_histhandler_var.clear() ;
    m_histhandler_var.swap(var);
}

string HistHandler::SetHistName(string  sample,  string njet, string var)
{
    string histname = sample +  "_pretag" + njet + "_vpt0_" + var ;
    return histname ;
}


//TH1F* HistHandler::GetHist(TFile* file,string histname)
//{
//    
//    TH1F *h1 =(TH1F*)file->Get(histname.c_str());
//    
//    if(h1==0)
//    {
//        cout << "no hist found !!!"<< endl ;
//    }
//    
//    return h1;
//}

void HistHandler::SetHist(TFile* filename)
{
    
        const int tot_hist = m_histhandler_samplename.size() *  m_histhandler_njets.size() * m_histhandler_var.size()  ;
    
    
    TH1F* allhist ;
    TH1F* hist[tot_hist] ;
    bool isTh1 = true ;
     //  cout << sample.size() << flavor.size() << njet.size() << endl ;
        //  cout << " tot_hist : " << tot_hist << endl ;
    
    
        //  TFile* file = GetFile(filename) ;
//    
        for(unsigned int ivar(0); ivar<m_histhandler_var.size() ; ivar++)
        {
            string var = m_histhandler_var.at(ivar) ;
            
            size_t charpos = var.find("vs",0) ;
            if(charpos != string::npos)
            {
                isTh1 = false ;
//                TH2F *allhist = new TH2F() ;
//                TH2F *hist[tot_hist] ;
            }
            else
            {
                isTh1 = true ;
//                TH1F *allhist = new TH1F() ;
//                TH1F *hist[tot_hist] ;
            }
            
        int i_hist = 0 ;
//            
        for(unsigned int isamp(0); isamp<m_histhandler_samplename.size() ; isamp++)
        {
            string sample_name = m_histhandler_samplename.at(isamp);
    
                for(unsigned int injet(0); injet<m_histhandler_njets.size(); injet++)
                {
                    string njet_name = m_histhandler_njets.at(injet);
    
                    string histname = ( SetHistName(sample_name,  njet_name, var) ) ;
    
                    if(filename->Get(histname.c_str())) hist[i_hist] = (TH1F*)filename->Get(histname.c_str());
    
                    if(i_hist==0) allhist  = hist[i_hist] ;
    
                    else if(i_hist>=1) allhist->Add(hist[i_hist]) ;
                    
                    i_hist = i_hist+1 ;
                }
            }
            
//            if(isTh1) m_histhandler_hist.insert(std::pair<int,TH1F*>(ivar,allhist)) ;
//            if(!isTh1) m_histhandler_2dhist.insert(std::pair<int,TH2F*>(ivar,(TH2F*)allhist)) ;
            
            if(isTh1) m_histhandler_hist.push_back(allhist) ;
            if(!isTh1) m_histhandler_2dhist.push_back((TH2F*)allhist) ;
        }
    
//        return allhist ;
}
