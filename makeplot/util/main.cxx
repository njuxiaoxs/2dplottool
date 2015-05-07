#include "makeplot/plottool.h"

//#include "/afs/cern.ch/work/c/cwang/plottool/CxAODTool/CxAODTool/ConfigStore.h"
#include "CxAODTools/ConfigStore.h"

//#include "EventLoop/Job.h"
//#include "EventLoop/DirectDriver.h"
//#include "EventLoop/ProofDriver.h"
//#include "EventLoopGrid/PrunDriver.h"

#include "EventLoop/StatusCode.h"


//#include <stdlib.h>
#include <vector>

#include <TFile.h>
#include <TString.h>
#include <TH1F.h>
#include <TCanvas.h>

#include <iostream>

#include "atlasstyle/AtlasUtils.h"
#include "atlasstyle/AtlasStyle.h"
#include "atlasstyle/AtlasLabels.h"

#include "Rtypes.h"

int main(int argc, char* argv[])
{

//  if (argc > 1) submitDir = argv[1];
//  if (argc > 2) submitDir = argv[2];

//  SetAtlasStyle();

  plotobj* algorithm = new plotobj() ; 

  algorithm->initialize() ;

  algorithm->execute() ;

  return 0 ;
}






