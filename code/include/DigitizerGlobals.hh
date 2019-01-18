// DigitizerGlobals.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: Thu Apr 28 22:11:05 2016 (+0800)
// Last-Updated: 五 11月 30 14:50:03 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 36
// URL: http://wuhongyi.cn 

#ifndef _DIGITIZERGLOBALS_H_
#define _DIGITIZERGLOBALS_H_

#include <stdint.h>
#include <CAENDigitizerType.h>

#include <cstring>
#include <string>

#include "TCanvas.h"
#include "TGraph.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// you need to be modifield before each experiment
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// module number
#define MAXNB_DT5730  1
#define MAXNB  (MAXNB_DT5730) 

#define CANVASX 800
#define CANVASY 400

#define MAXFILEBYTE 2000000000  //byte

static char PKU_DGTZ_GlobalParametersFileName[32] = "GlobalParameters.txt";
static char PKU_DGTZ_BoardParametersFileName[32] = "BoardParameters.txt";

static FILE *PKU_DGTZ_FilePointer = NULL;
static FILE *PKU_DGTZ_LogFilePointer = NULL;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Do NOT change.
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// maximum channel number
#define MAXNCH_DT5730   8
#define MaxNChannels (MAXNCH_DT5730)

#define MAXNBITS 14

//can enter total of 14 parameters : Offset + PreTrg +12(5) PSD Par
#define PARAMS_NUM 14

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

struct DigitizerParams_t
{
  CAEN_DGTZ_ConnectionType LinkType;//ok
  int LinkNum;//ok
  int ConetNode;//ok
  uint32_t VMEBaseAddress;//ok
  uint32_t RecordLength;//ok
  uint32_t ChannelMask;//ok
  int EventAggr;//ok
  CAEN_DGTZ_DPP_AcqMode_t DPPAcqMode;//ok
  CAEN_DGTZ_AcqMode_t AcqMode;//ok
  CAEN_DGTZ_IOLevel_t IOlev;//ok
  CAEN_DGTZ_PulsePolarity_t PulsePolarity[MaxNChannels];//ok
  uint32_t DPPPreTriggerSize[MaxNChannels];//ok
  float ChannelDCOffset[MaxNChannels];//ok

  CAEN_DGTZ_DPP_SaveParam_t SaveParam;//ok
  CAEN_DGTZ_TriggerMode_t SWTrgMode;//ok
  CAEN_DGTZ_TriggerMode_t ExtTrgMode;//ok
  CAEN_DGTZ_TriggerMode_t SelfTrgMode;//ok
  CAEN_DGTZ_RunSyncMode_t RunSyncMode;//ok
  int VirtualProbe1;
  int VirtualProbe2;
  int DigitalProbe;
  uint32_t Run_Start_Stop_Delay;//ok
  CAEN_DGTZ_AnalogMonitorOutputMode_t AnalogMonOutput;//ok
} ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//run manager
struct DigitizerRun_t
{
  int RunNumber;
  int FileNo;//form 0,1,2,3……
  
  bool Quit;//true:exit  false:run
  bool AcqRun;//true:start  false:stop
  char Key;//
  int Nb;//

  bool WriteFlag;//true:write  false:not write

  bool PlotFlag;
  int DoPlotBoard;
  int DoPlotChannel;
  int PlotEveryN;//  plot one every N waveform
  bool PlotColor;
  int PlotChooseN;//const

  uint64_t CurrentTime;
  uint64_t PrevRateTime;
  uint64_t ElapsedTime;

  uint32_t BufferSize;
  uint32_t NumEvents[MaxNChannels];
  
  // Arrays for data analysis
  uint64_t PrevTime[MAXNB][MaxNChannels];//
  int ECnt[MAXNB][MaxNChannels];//
  int TrgCnt[MAXNB][MaxNChannels];//
  int PurCnt[MAXNB][MaxNChannels];//

  char PathToRawData[128];
  char WriteFileName[256];

  char StartWritingTime[16];
  char StopWritingTime[16];
  char StartRunningTime[16];
  char StopRunningTime[16];
} ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

static TCanvas *PKU_DGTZ_Canvas = NULL;
static TGraph *PKU_DGTZ_Graph = NULL;
static TGraph *PKU_DGTZ_Short = NULL;
static TGraph *PKU_DGTZ_Long = NULL;

#endif /* _DIGITIZERGLOBALS_H_ */

// 
// DigitizerGlobals.hh ends here
