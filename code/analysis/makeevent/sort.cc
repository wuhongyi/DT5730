// sort.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 1月 15 09:20:33 2019 (+0800)
// Last-Updated: 三 1月 23 18:41:48 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#include "sort.hh"

#include "TBranch.h"
#include "TString.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TBenchmark.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

sort::sort(TString filepath,int runnumber)
{
  benchmark = new TBenchmark;

  windows = 200;
  flag = false;
  nevt = 0;
  
  filefile = new TFile(TString::Format("%s/run%04d_sort.root",filepath.Data(),runnumber).Data(),"READ");//"RECREATE" "READ"
  if(!filefile->IsOpen())
    {
      std::cout<<"Can't open raw root file"<<std::endl;
      exit(1);
    }
  tt = (TTree*)filefile->Get("t");
  tt->SetBranchAddress("ch", &ch, &b_ch);
  tt->SetBranchAddress("qs", &qs, &b_qs);
  tt->SetBranchAddress("ql", &ql, &b_ql);
  tt->SetBranchAddress("ts", &ts, &b_ts);
  tt->SetBranchAddress("ft", &ft, &b_ft);
  TotalEntry = tt->GetEntries();

  file = new TFile(TString::Format("%s/run%04d_W.root",filepath.Data(),runnumber).Data(),"RECREATE");
  t = new TTree("t","PKU CAEN DT5730 Event Data");
  t->Branch("CH", &CH, "CH[8]/S");
  t->Branch("QS", &QS, "QS[8]/s");
  t->Branch("QL", &QL, "QL[8]/s");
  t->Branch("TS", &TS, "TS[8]/L");
  t->Branch("FT", &FT, "FT[8]/S");  

  
}

sort::~sort()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void sort::clearopt()
{
  memset(CH,0,sizeof(CH));
  memset(QS,0,sizeof(QS));
  memset(QL,0,sizeof(QL));
  memset(TS,0,sizeof(TS));
  memset(FT,0,sizeof(FT));
}

void sort::Process()
{
  benchmark->Start("event");//计时开始

  for (Long64_t entry = 0; entry < TotalEntry; ++entry)
    {
      if(entry % 10000 == 0)
	{
	  std::cout<<"\r"<<"Entry: "<<entry<<"  |  Event: "<<nevt+1;
	  std::cout << std::flush;
	}
      filefile->cd();
      tt->GetEvent(entry);

      tsnow = ts*2;
      
      if(flag)
	{
	  tsnow = ts*2;
	  
	  if(TMath::Abs(tsnow-inittime) <= Long64_t(windows))
	    {
	      ProcessEntry();
	    }
	  else
	    {
	      file->cd();	      
	      t->Fill();
	      nevt++;
	      InitEvent();
	    }
	}
      else
	{//event header
	  InitEvent();
	  // std::cout<<ts<<"  "<<ch<<"  "<<sid<<std::endl;
	  flag = true;
	}
    }

  std::cout<<std::endl;

  filefile->cd();
  filefile->Close();

  file->cd();
  t->Fill();
  t->Write();
  file->Close();

  
  benchmark->Show("event");//计时结束并输出时间
}

void sort::InitEvent()
{
  clearopt();

  inittime = ts*2;

  ProcessEntry();
}

void sort::ProcessEntry()
{
  CH[ch] = ch;
  QS[ch] = qs;
  QL[ch] = ql;
  TS[ch] = ts;
  FT[ch] = ft;
}




// 
// sort.cc ends here
