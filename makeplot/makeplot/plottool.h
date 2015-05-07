#ifndef plottool_H
#define plottool_H

#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <string.h>

#include "EventLoop/StatusCode.h"

#include "CxAODTools/ConfigStore.h"

#include "HistHandler.h"

using namespace std ;


class plotobj
{

public:
    

//    TH1F* m_hist ;
//    vector<TH1F*> m_histsig ; // merged signal 
//    vector<TH1F*> m_histbkg ; // merged bkg
//    
//    vector<TH2F*> m_2dhist ;
//
    TFile* m_file ;
//    TFile* m_filesig ; //  signal file 
//    TFile* m_filebkg ; //  bkg file
//
    string  m_filePath ;    //  signal file path
//    string  m_filesigPath ; //  signal file path 
//    string  m_filebkgPath ; //  bkg    file path
//
    string  m_output ; //  bkg    file path
//
//    std::vector<string> m_signalsample ;
//    std::vector<string> m_bkgsample ;
//
//    std::vector<string> m_flavor ;
    
    vector<HistHandler*> m_histhandlers ;

    string m_configPath   ;
    ConfigStore* m_config ;
    
    int m_Ncat ;

    vector<string> m_var ; // the variables to be drawn

    vector<string> m_njets ; // the njet region

    bool m_isNor ; //whether to normilize the sig and  bkg 

    string m_histname ;

    plotobj();

public:

    void Draw(TH1F* hist);

    void SetConfig(std::string configPath){ m_configPath = configPath ;} 

    string GetHistName(string sample, string  flavor, string njet, string  var);

    TFile* GetFile(string filename);
    TH1F*  GetHist(TFile* file, string histname);

    TH1F* Add(TH1F* hist1, TH1F* hist2);

    TH1F* plot(TFile* filename, const vector<string> sample, const vector<string> flavor , const vector<string> njet, string var);

    virtual EL::StatusCode initialhist() ;
    virtual EL::StatusCode initialize() ;
    virtual EL::StatusCode finalize(){} ;
    virtual EL::StatusCode execute() ;

    virtual EL::StatusCode makeplot(){} ;

    virtual EL::StatusCode drawhist() ;
    virtual EL::StatusCode drawroc() ;
    virtual EL::StatusCode draweff() ;

//  ClassDef(plotobj,1);
};
#endif


