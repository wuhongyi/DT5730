// sort.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 1月 15 09:20:13 2019 (+0800)
// Last-Updated: 三 1月 23 18:41:48 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#ifndef _SORT_H_
#define _SORT_H_

#include "TString.h"

#include<iostream>
class TBranch;
class TFile;
class TTree;
class TBenchmark;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class sort
{
public:
  sort(TString filepath,int runnumber);
  virtual ~sort();

  void Process();
  
private:
  void clearopt();
  void InitEvent();
  void ProcessEntry();

  
private:
  //old root
  TFile *filefile;
  TTree *tt;
  //new root
  TFile *file;
  TTree *t;

  TBenchmark *benchmark;
  Long64_t TotalEntry;
  
  Short_t         ch;
  UShort_t         qs;
  UShort_t         ql;
  ULong64_t       ts;
  Short_t         ft;

  TBranch        *b_ch;   //!
  TBranch        *b_qs;   //!
  TBranch        *b_ql;   //!
  TBranch        *b_ts;   //!
  TBranch        *b_ft;   //!

  Short_t CH[8];
  Short_t QS[8];
  Short_t QL[8];
  Long64_t TS[8];
  Short_t FT[8];

  int nevt;
  bool flag;
  Long64_t inittime;
  Long64_t tsnow;
  int windows;//组合事件门宽 ns
};

#endif /* _SORT_H_ */
// 
// sort.hh ends here
