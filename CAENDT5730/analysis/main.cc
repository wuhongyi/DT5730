// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: Fri Apr 29 21:54:43 2016 (+0800)
// Last-Updated: 一 6月 13 21:23:08 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 19
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

  char filepath[128] ="../data";

  
  int runnum_min,runnum_max;
  std::cout << "input runnum_min and runnum_max:\n" ;
  std::cin >> runnum_min >> runnum_max;

  FILE * rawfile;
  bool flag;//true表示该runnum文件不存在
  char filename[256];

  TFile *f = NULL;
  TTree *t = NULL;
  int b;
  int ch;
  int size;
  unsigned short energy;//uint16_t
  unsigned long time;//uint64_t
  int sample[10000];
  short WaveData[10000];//int16_t

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  for(int i=0;i<1500;i++) sample[i]=i;
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
	      t->Branch("b", &b, "b/I");
	      t->Branch("ch", &ch, "ch/I");
	      t->Branch("size", &size, "size/I");
	      t->Branch("e", &energy, "e/s");
	      t->Branch("t", &time, "t/l");
	      t->Branch("wave", WaveData, "wave[size]/S");
	      t->Branch("sample",&sample,"sample[size]/I");
	    }
	  std::cout<<"number:"<<i<<"  file:"<<ii<<std::endl;
	  while(!feof(rawfile))
	    {
	      fread(&b,4,1,rawfile);
	      fread(&ch,4,1,rawfile);
	      fread(&size,4,1,rawfile);
	      fread(&energy,2,1,rawfile);
	      fread(&time,8,1,rawfile);
	      fread(WaveData,2,size,rawfile);

	      
	      if(feof(rawfile)) break;
	      
	      // std::cout<<b<<" "<<ch<<" "<<size<<" "<<energy<<" "<<time<<std::endl;
	      // for (int i = 0; i < size; ++i)
	      // 	{
	      // 	  std::cout<<WaveData[i]<<" ";
	      // 	}
	      // std::cout<<std::endl;

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
