// DigitizerUser.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 4月 24 17:54:16 2016 (+0800)
// Last-Updated: 一 11月 26 15:00:08 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 34
// URL: http://wuhongyi.cn 

#ifndef _DIGITIZERUSER_H_
#define _DIGITIZERUSER_H_

#include "TGraph.h"
#include "TCanvas.h"

#include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include "DigitizerGlobals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// void SaveROOTPicture(char *type,int b, int ch, int size,int16_t *WaveData,TCanvas *c,TGraph *g);

void UpdateFileName(DigitizerRun_t *PKU_DGTZ_RunManager);
void OpenFile(DigitizerRun_t *PKU_DGTZ_RunManager);
void CloseFile();
void OpenRunLog();
void WriteRunLog(char *log);
void CloseRunLog();
void UpdateWhenFileFillUp(DigitizerRun_t *PKU_DGTZ_RunManager);

// This function saves the waveform in a textfile as a sequence of number representing the wave height
// b--Board ch--channel  size -- Record Length 
int SaveWaveform(int b, int ch, int size, int16_t energy,uint16_t *WaveData,uint32_t time);






#endif /* _DIGITIZERUSER_H_ */
// 
// DigitizerUser.hh ends here














