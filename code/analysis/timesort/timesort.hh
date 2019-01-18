// timesort.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 1月 14 19:41:44 2019 (+0800)
// Last-Updated: 一 1月 14 20:16:23 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#ifndef _TIMESORT_H_
#define _TIMESORT_H_

#include "TString.h"

#include<map>
#include <vector>
#include <algorithm>
#include<iostream>

class TBranch;
class TFile;
class TTree;
class TBenchmark;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define MAXTRACEN 10000 // maximum number of trace points

typedef std::pair<Long64_t, ULong64_t> PAIR;
struct CmpByValue
{
  bool operator()(const PAIR& lhs, const PAIR& rhs)
  {
    return lhs.second < rhs.second;
  }
};

class timesort
{
public:
  timesort(TString filepath,int runnumber);
  virtual ~timesort();

  void Process();

private:
  void SortByTime();
  void SaveByTime();
  
private:
  std::vector<PAIR> sortdata;
  
  //old root
  TFile *filefile;
  TTree *tt;
  // Int_t nevt;
  
  //new root
  TFile *file;
  TTree *t;
  Int_t snevt;
  
  TBenchmark *benchmark;
  Long64_t TotalEntry;

  // Declaration of leaf types
   Short_t         ch;
   Short_t         qs;
   Short_t         ql;
   UInt_t          format;
   ULong64_t       ts;
   Short_t         ft;
   Short_t         size;
   Short_t         wave[MAXTRACEN];   //[size]
   Short_t         sample[MAXTRACEN];   //[size]

   // List of branches
   TBranch        *b_ch;   //!
   TBranch        *b_qs;   //!
   TBranch        *b_ql;   //!
   TBranch        *b_format;   //!
   TBranch        *b_ts;   //!
   TBranch        *b_ft;   //!
   TBranch        *b_size;   //!
   TBranch        *b_wave;   //!
   TBranch        *b_sample;   //!
  

};

#endif /* _TIMESORT_H_ */
// 
// timesort.hh ends here
