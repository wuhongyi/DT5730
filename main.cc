// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 16 16:27:28 2018 (+0800)
// Last-Updated: 五 11月 16 16:31:58 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

#include "Board.hh"



#include "RVersion.h"//版本判断
#include "TApplication.h"
#include "TBenchmark.h"
#include "TBrowser.h"
#include "TCutG.h"
#include "TDatime.h"
#include "TError.h"
#include "TList.h"
#include "TObject.h"
#include "TRint.h"
#include "TROOT.h"
#include "TSlider.h"
#include "TStopwatch.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTimer.h"
#include "TTimeStamp.h"
#include "THttpServer.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  // Create an interactive ROOT application
  TRint *theApp = new TRint("Rint", &argc, argv);

  Board *board = new Board;

  
  // and enter the event loop...
  theApp->Run();
  delete theApp;
  return 0;
}


// 
// main.cc ends here
