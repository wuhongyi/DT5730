// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: Fri Apr 29 21:54:43 2016 (+0800)
// Last-Updated: 六 1月 19 10:28:50 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 26
// URL: http://wuhongyi.cn 

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <string>
#include <cstring>

#include "TFile.h"
#include "TTree.h"

int main(int argc, char *argv[])
{
  // CHECK
  if(sizeof(char) != 1) std::cout<<"sizeof(char) != 1 The current compiler is not suitable for running the program！"<<std::endl; 
  if(sizeof(short) != 2) std::cout<<"sizeof(short) != 2 The current compiler is not suitable for running the program！"<<std::endl; 
  if(sizeof(int) != 4) std::cout<<"sizeof(int) != 4 The current compiler is not suitable for running the program！"<<std::endl;
  if(sizeof(long) != 8) std::cout<<"sizeof(long) != 8 The current compiler is not suitable for running the program！"<<std::endl;

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  char filepath[1024] ="../../data";//不要以 / 结尾

  
  int runnum_min,runnum_max;
  std::cout << "input runnum_min and runnum_max:\n" ;
  std::cin >> runnum_min >> runnum_max;

  FILE * rawfile;
  bool flag;//true表示该runnum文件不存在
  char filename[256];

  TFile *f = NULL;
  TTree *t = NULL;

  short ch;
  unsigned int TimeTag;
  short ChargeShort;//int16_t
  short ChargeLong;
  uint32_t Format;
  uint32_t Extras;
  unsigned long ts;//uint64_t
  short ft;
  short size;
  short sample[10000];
  short WaveData[10000];//int16_t

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  for(int i=0;i<10000;i++) sample[i]=i;
  for (int i = runnum_min; i <= runnum_max; ++i)//number
    {
      flag = false;
      int ii = 0;
      
      for (;;)//循环处理 _0 _1 _2 _3 等数据
	{
	  if(flag) break;
	  sprintf(filename,"%s/run%04d_%d",filepath,i,ii);
	  
	  if((rawfile = fopen(filename,"rb")) == NULL)
	    {
	      if(ii == 0) flag = true;
	      break;
	    }	  

	  if(ii == 0)
	    {
	      sprintf(filename,"%s/run%04d.root",filepath,i);
	      f = new TFile(filename,"RECREATE");//"RECREATE" "READ"
	      t = new TTree("t","PKU Digitizer data");
	      t->Branch("ch", &ch, "ch/S");
	      t->Branch("qs", &ChargeShort, "qs/S");
	      t->Branch("ql", &ChargeLong, "ql/S");
	      t->Branch("format", &Format, "format/i");
	      t->Branch("ts", &ts, "ts/l");
	      t->Branch("ft", &ft, "ft/S");
	      t->Branch("size", &size, "size/S");
	      t->Branch("wave", WaveData, "wave[size]/S");
	      t->Branch("sample",&sample,"sample[size]/S");
	    }
	  std::cout<<"number:"<<i<<"  file:"<<ii<<std::endl;
	  while(!feof(rawfile))
	    {
	      fread(&ch,2,1,rawfile);
	      fread(&TimeTag,4,1,rawfile);
	      fread(&ChargeShort,2,1,rawfile);
	      fread(&ChargeLong,2,1,rawfile);
	      fread(&Format,4,1,rawfile);
	      fread(&Extras,4,1,rawfile);	      
	      fread(&size,2,1,rawfile);
	      fread(WaveData,2,size,rawfile);
   
	      
	      if(feof(rawfile)) break;
	      
	      // std::cout<<b<<" "<<ch<<" "<<size<<" "<<energy<<" "<<time<<std::endl;
	      // for (int i = 0; i < size; ++i)
	      // 	{
	      // 	  std::cout<<WaveData[i]<<" ";
	      // 	}
	      // std::cout<<std::endl;

	      ts = (((unsigned long)(Extras&0xffff0000))<<15) + (TimeTag&0x7fffffff);
	      ft = Extras&0x3ff;
	      t->Fill();
	      
	    }
	  
	  fclose(rawfile);
	  ii++;
	}//循环处理 _0 _1 _2 _3 等数据

      if(flag) continue;
      
      if(t)
	t->Write();
      if(t) delete t;
      
      if(f != NULL)
	f->Close();
      f = NULL;

    }//number

  // if(WaveData != NULL)
  //   delete [] WaveData;

  return 0;
}


// 
// main.cc ends here
