// DigitizerConfigure.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: Thu Apr 28 22:16:47 2016 (+0800)
// Last-Updated: 五 11月 30 15:12:27 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 42
// URL: http://wuhongyi.cn 

#ifndef _DIGITIZERCONFIGURE_H_
#define _DIGITIZERCONFIGURE_H_

// #include "TCanvas.h"
// #include "TGraph.h"

#include "DigitizerGlobals.hh"
#include "DigitizerAdmin.hh"

#include "CAENDigitizerType.h"
#include "CAENDigitizer.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Only use in V1724 now!!! need update for other board!!!
// return  0=success; -1=error
int DigitizerInit(CAEN_DGTZ_DPP_PSD_Params_t *PKU_DGTZ_DPPParams,DigitizerParams_t *PKU_DGTZ_Params,int *PKU_DGTZ_handle,CAEN_DGTZ_BoardInfo_t PKU_DGTZ_BoardInfo);

void RunManagerInit(DigitizerRun_t *RunManager);

void CheckKeyboard(DigitizerRun_t *PKU_DGTZ_RunManager,int *PKU_DGTZ_handle,CAEN_DGTZ_DPP_PSD_Params_t *PKU_DGTZ_DPPParams,DigitizerParams_t *PKU_DGTZ_Params);

// Calculate throughput and trigger rate (every second),print something about running information.
void PrintRunningStatus(DigitizerRun_t *PKU_DGTZ_RunManager);

void PlotROOTGraph(DigitizerRun_t *PKU_DGTZ_RunManager,int b,int ch,int size,uint16_t *WaveData,uint8_t  *DTrace1,uint8_t  *DTrace2);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Quit the Program
void QuitProgram(int *handle);
void QuitProgram(int *handle,char *buffer);

// Program the registers of the digitizer with the relevant parameters
// return  0=success; -1=error 
int ProgramDigitizer_DT5730(int handle, DigitizerParams_t Params, CAEN_DGTZ_DPP_PSD_Params_t DPPParams);

// return  0=success; -1=error
int ReadDPPParameters_PSD(DigitizerParams_t *Params, CAEN_DGTZ_DPP_PSD_Params_t *DPPParams);

// Read DGTZ Registers 
// return  0=success; -1=error
int Read_DGTZ_Register_725_730_DPP_PSD_Revision05(int handle,int MaxNChInBoard);

// Read DGTZ Info
// return  0=success; -1=error
int Read_DGTZ_Info(int handle);

#endif /* _DIGITIZERCONFIGURE_H_ */
// 
// DigitizerConfigure.hh ends here
