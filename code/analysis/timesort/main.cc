// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 1月 14 19:44:51 2019 (+0800)
// Last-Updated: 六 1月 19 10:51:55 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#include "timesort.hh"

#include "TString.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  TString filepath ="../../data";//不要以 / 结尾

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
