// timesort.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 1月 14 19:44:11 2019 (+0800)
// Last-Updated: 六 1月 19 17:08:45 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 30
// URL: http://wuhongyi.cn 

#include "timesort.hh"

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

timesort::timesort(TString filepath,int runnumber)
{
  snevt = 0;
  CurrentEntry = 0;
  benchmark = new TBenchmark;
  
  filefile = new TFile(TString::Format("%s/run%04d.root",filepath.Data(),runnumber).Data(),"READ");//"RECREATE" "READ"
  if(!filefile->IsOpen())
    {
      std::cout<<"Can't open raw root file"<<std::endl;
      exit(1);
    }
  tt = (TTree*)filefile->Get("t");
  tt->SetBranchAddress("ch", &ch, &b_ch);
  tt->SetBranchAddress("qs", &qs, &b_qs);
  tt->SetBranchAddress("ql", &ql, &b_ql);
  tt->SetBranchAddress("format", &format, &b_format);
  tt->SetBranchAddress("ts", &ts, &b_ts);
  tt->SetBranchAddress("ft", &ft, &b_ft);
  TotalEntry = tt->GetEntries();

  file = new TFile(TString::Format("%s/run%04d_sort.root",filepath.Data(),runnumber).Data(),"RECREATE");
  t = new TTree("t","PKU CAEN DT5730 Sort Time Data");
  t->Branch("ch", &ch, "ch/S");
  t->Branch("qs", &qs, "qs/S");
  t->Branch("ql", &ql, "ql/S");
  t->Branch("format", &format, "format/i");
  t->Branch("ts", &ts, "ts/L");
  t->Branch("ft", &ft, "ft/S");
  t->Branch("nevt", &snevt, "nevt/I");

  // tt->SetCacheSize(100*1024*1024);
  // t->SetCacheSize(100*1024*1024);
}

timesort::~timesort()
{
  filefile->cd();
  filefile->Close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void timesort::Process()
{
  benchmark->Start("timesort");//计时开始

  while(true)
    {
      if(sortdata.size() <= READENTRYUNDER && CurrentEntry < TotalEntry)
	{
	  for (Long64_t entry = CurrentEntry; entry < TotalEntry; ++entry)
	    {
	      if(entry % 10000 == 0) std::cout<<"Read: "<<entry<< " / "<<TotalEntry<<std::endl;
	      filefile->cd();
	      tt->GetEvent(entry);
	      mapvalue.ch = ch;
	      mapvalue.qs = qs;
	      mapvalue.ql = ql;
	      mapvalue.format = format;
	      mapvalue.ts = ts;
	      mapvalue.ft = ft;
	      flagkey = (ts<<2) + ch;
	      sortdata.insert(std::make_pair(flagkey,mapvalue));
	      
	      CurrentEntry = entry+1;
	      if(sortdata.size() >= MAXENTRYINMAP) break;
	    }
	}

      
      if(CurrentEntry >= TotalEntry)
	{
	  while(sortdata.size() > 0)
	    {
	      itkey = sortdata.begin();
	      ch = itkey->second.ch;
	      qs = itkey->second.qs;
	      ql = itkey->second.ql;
	      format = itkey->second.format;
	      ts = itkey->second.ts;
	      ft = itkey->second.ft;
	      sortdata.erase(itkey);
	      file->cd();
	      t->Fill();
	      snevt++;
	      // std::cout<<">0  "<<sortdata.size()<<std::endl;
	    }
	}
      else
	{
	  while(sortdata.size() > READENTRYUNDER)
	    {
	      itkey = sortdata.begin();
	      ch = itkey->second.ch;
	      qs = itkey->second.qs;
	      ql = itkey->second.ql;
	      format = itkey->second.format;
	      ts = itkey->second.ts;
	      ft = itkey->second.ft;
	      sortdata.erase(itkey);
	      file->cd();
	      t->Fill();
	      snevt++;
	      // std::cout<<">50000  "<<sortdata.size()<<std::endl;
	    }
	}


     if(CurrentEntry >= TotalEntry && sortdata.size()==0) break;
    }

  file->cd();
  t->Write();
  file->Close();
  
  benchmark->Show("timesort");//计时结束并输出时间
}

// void timesort::SortByTime()
// {
//   std::map<Long64_t, ULong64_t> rawdata;
//   for (Long64_t entry = 0; entry < TotalEntry; ++entry)
//     {
//       if(entry % 10000 == 0) std::cout<<"Read: "<<entry<< " / "<<TotalEntry<<std::endl;
//       filefile->cd();
//       tt->GetEvent(entry);
//       rawdata.insert(std::make_pair(entry,ts));
//     }

//   sortdata = std::vector<PAIR>(rawdata.begin(), rawdata.end());
//   sort(sortdata.begin(), sortdata.end(), CmpByValue());
  
//   // std::cout<< TotalEntry<<"  "<<rawdata.size()<<"  "<<sortdata.size()<<std::endl;

//   // for (int i = 0; i < sortdata.size(); ++i)
//   //   {
//   //     std::cout<<sortdata[i].first << "  " << sortdata[i].second<<std::endl;
//   //   }
// }


// void timesort::SaveByTime()
// {
//   for (Long64_t entry = 0; entry < TotalEntry; ++entry)
//     {
//       if(entry % 10000 == 0) std::cout<<"Save: "<<entry<< " / "<<TotalEntry<<std::endl;
//       // std::cout<<"Save: "<<entry<< " / "<<TotalEntry<<std::endl;
//       filefile->cd();
//       // std::cout<<sortdata[entry].first<<std::endl;
//       tt->GetEvent(sortdata[entry].first);
//       file->cd();
//       t->Fill();
//       snevt++;
//     }

//   file->cd();
//   t->Write();
//   file->Close();
// }


// 
// timesort.cc ends here
