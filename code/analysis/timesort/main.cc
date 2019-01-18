// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 1月 14 19:44:51 2019 (+0800)
// Last-Updated: 一 1月 14 20:16:23 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 2
// URL: http://wuhongyi.cn 

#include "timesort.hh"

#include "TString.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  TString filepath ="../../data";

  int runnum_min,runnum_max;
  std::cout << "input runnum_min and runnum_max:\n" ;
  std::cin >> runnum_min >> runnum_max;

  for (int i = runnum_min; i <= runnum_max; ++i)//number
    {
      timesort *ts = new timesort(filepath,i);
      ts->Process();
      delete ts;
    }
  
  return 0;
}

// 
// main.cc ends here
