#ifndef Histhandler_H
#define Histhandler_H

#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>

#include <string>
#include <vector>

#include "EventLoop/StatusCode.h"

#include "CxAODTools/ConfigStore.h"

using namespace std ;

class HistHandler
{
    
    public :
    
    enum Type{
        signal,
        background
    } ;
    
//    map<int,TH1F*> m_histhandler_hist ;
//    map<int,TH2F*> m_histhandler_2dhist ;
    vector<TH1F*> m_histhandler_hist ;
    vector<TH2F*> m_histhandler_2dhist ;
    Type m_histhandler_type ;
    
    vector<TH1F*>  m_histhandler_hist_component ;
    vector<string> m_histhandler_samplename ;
    vector<string> m_histhandler_histname ;
    vector<string> m_histhandler_njets ;
    vector<string> m_histhandler_var ;
    vector<string> m_histhandler_region ;
    vector<bool>   m_isTh1;
    
    HistHandler() ;
    
    vector<string> GetHistName(){return m_histhandler_histname ;}  ;
    vector<string> GetNjets(){return m_histhandler_njets ;}  ;
    vector<string> GetVar(){return m_histhandler_var ;}  ;
    vector<bool>   GetisTh1(){return m_isTh1 ;}  ;
    vector<TH1F*> GetHistComponent(vector<string> component_name) ;
    vector<string> GetSampleName() {return m_histhandler_samplename ;} ;
    vector<string> GetRegionName() {return m_histhandler_region ;} ;
    Type GetHistType(){return m_histhandler_type ;}  ;
    
//    template <typename TPname>
    void SetHist(TFile* filename ) ;
//    TH1F* GetHist(TFile* filename,  string histname) ;
    
    string SetHistName(string  sample, string njet,  string var) ;
    void SetNjets(vector<string> njets) ;
    void SetVar(vector<string> var) ;
//    void SetHistName(vector<string> hist_name) ;
    void SetHistType(Type type){ m_histhandler_type = type ;}  ;
    void MergeHistComponent(vector<TH1F*> hist_component) ;
    void SetSampleName(vector<string> t_samplename) ;
    void SetRegionName(vector<string> regionname) ;
    
    void Rebin(TH1F* hist , int x)  ;
    void Rebin(TH2F* hist , int x , int y ) ;
    
};

#endif
