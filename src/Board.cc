// Board.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 11月 12 18:07:01 2018 (+0800)
// Last-Updated: 日 11月 25 21:01:57 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 33
// URL: http://wuhongyi.cn 

#include "Board.hh"

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/stat.h>//stat(const char *file_name,struct stat *buf)
#include <sys/time.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Board::Board()
{
  dig = NULL;
  readoutBuffer = NULL;
  EventPtr = NULL;
  
  dig = (Digitizer*) calloc(1,sizeof(Digitizer));
  dig->boardInfo = (CAEN_DGTZ_BoardInfo_t*) calloc(1,sizeof(CAEN_DGTZ_BoardInfo_t));

  dig->linkType = CAEN_DGTZ_USB;
  dig->linkNum = 0;
  dig->conetNode = 0;
  dig->baseAddress = 0;


  CAEN_DGTZ_SWStopAcquisition(0);
  CAEN_DGTZ_CloseDigitizer(0);

  OpenDigitizer();
  GetInfo();
  PrintInfo();

  int MajorNumber;
  sscanf(dig->boardInfo->AMC_FirmwareRel, "%d", &MajorNumber);
  if (MajorNumber >= 128)
    {
      printf("This digitizer has a DPP firmware\n");
      CheckLicense();
    }

  Reset();
  // printf("Error: Unable to reset digitizer.\nPlease reset digitizer manually then restart the program\n");


  // CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope  CAEN_DGTZ_DPP_ACQ_MODE_List  CAEN_DGTZ_DPP_ACQ_MODE_Mixed
  // This setting affects the modes Mixed and List (see CAEN_DGTZ_DPP_AcqMode_t definition for details)
  // CAEN_DGTZ_DPP_SAVE_PARAM_EnergyOnly        Only charge (DPP-PSD/DPP-CI v2) is returned
  // CAEN_DGTZ_DPP_SAVE_PARAM_TimeOnly        Only time is returned
  // CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime    Both charge and time are returned
  // CAEN_DGTZ_DPP_SAVE_PARAM_None            No histogram data is returned 
  SetDPPAcquisitionMode(CAEN_DGTZ_DPP_ACQ_MODE_Mixed,CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime);

  // CAEN_DGTZ_SW_CONTROLLED  CAEN_DGTZ_S_IN_CONTROLLED  CAEN_DGTZ_FIRST_TRG_CONTROLLED  CAEN_DGTZ_LVDS_CONTROLLED
  SetAcquisitionMode(CAEN_DGTZ_SW_CONTROLLED);


  // CAEN_DGTZ_IOLevel_NIM  CAEN_DGTZ_IOLevel_TTL
  SetIOLevel(CAEN_DGTZ_IOLevel_TTL);

  // CAEN_DGTZ_TRGMODE_DISABLED: do nothing
  // CAEN_DGTZ_TRGMODE_EXTOUT_ONLY: generate the Trigger Output signal
  // CAEN_DGTZ_TRGMODE_ACQ_ONLY = generate acquisition trigger
  // CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT = generate both Trigger Output and acquisition trigger
  SetExtTriggerInputMode(CAEN_DGTZ_TRGMODE_ACQ_ONLY);


  SetDPPEventAggregation(0,0);//1,0
  
  SetRunSynchronizationMode(CAEN_DGTZ_RUN_SYNC_Disabled);


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  for (int ch = 0; ch < 8; ++ch)
    {
      dpppsdParams.thr[ch] = 0;// Trigger Threshold
      dpppsdParams.nsbl[ch] = 2;    // used to specifiy the number of samples for the baseline averaging
      dpppsdParams.lgate[ch] = 32;    // Long Gate Width (N*4ns)
      dpppsdParams.sgate[ch] = 24;    // Short Gate Width (N*4ns)
      dpppsdParams.pgate[ch] = 8;     // Pre Gate Width (N*4ns)
      dpppsdParams.selft[ch] = 0;  // Self Trigger Mode: 0 -> Disabled 1 -> Enabled
      // Trigger configuration:
      // CAEN_DGTZ_DPP_TriggerConfig_Peak       -> trigger on peak. NOTE: Only for FW <= 13X.5
      // CAEN_DGTZ_DPP_TriggerConfig_Threshold  -> trigger on threshold 
      dpppsdParams.trgc[ch] = CAEN_DGTZ_DPP_TriggerConfig_Threshold;
      dpppsdParams.tvaw[ch] = 50;// Trigger Validation Acquisition Window
      dpppsdParams.csens[ch] = 0;// Charge sensibility: 0->40fc/LSB; 1->160fc/LSB; 2->640fc/LSB; 3->2,5pc/LSB
    }
  // Pile-Up rejection Mode
  //    CAEN_DGTZ_DPP_PSD_PUR_DetectOnly -> Only Detect Pile-Up
  //    CAEN_DGTZ_DPP_PSD_PUR_Enabled -> Reject Pile-Up
  // DPPParams[b].purh = CAEN_DGTZ_DPP_PSD_PUR_DetectOnly;
  dpppsdParams.purh = CAEN_DGTZ_DPP_PSD_PUR_DetectOnly; //NOTE: Ignored on 5730
  dpppsdParams.purgap = 100;  // Purity Gap   // Pile-up Rejection Gap  //NOTE: Ignored on 5730
  dpppsdParams.blthr = 3;     // Baseline Threshold  //This parameter is deprecated
  dpppsdParams.bltmo = 100;   // Baseline Timeout    //This parameter is deprecated
  dpppsdParams.trgho = 0;     // Trigger HoldOff

  
  // for(ch=0; ch<MaxNChannels; ch++)
  //   {
  //     DPPParams[b].thr[ch] = 50;        // Trigger Threshold
  //     /* The following parameter is used to specifiy the number of samples for the baseline averaging:
  // 	 0 -> absolute Bl
  // 	 1 -> 16samp
  // 	 2 -> 64samp
  // 	 3 -> 256samp
  // 	 4 -> 1024samp */
  //     DPPParams[b].nsbl[ch] = 1;
  //     DPPParams[b].lgate[ch] = 100;    // Long Gate Width (N*2ns for x730  and N*4ns for x725)
  //     DPPParams[b].sgate[ch] = 24;    // Short Gate Width (N*2ns for x730  and N*4ns for x725)
  //     DPPParams[b].pgate[ch] = 8;     // Pre Gate Width (N*2ns for x730  and N*4ns for x725) 
  //     /* Self Trigger Mode:
  // 	 0 -> Disabled
  // 	 1 -> Enabled */
  //     DPPParams[b].selft[ch] = 1;
  //     // Trigger configuration:
  //     //    CAEN_DGTZ_DPP_TriggerMode_Normal ->  Each channel can self-trigger independently from the other channels
  //     //    CAEN_DGTZ_DPP_TriggerMode_Coincidence -> A validation signal must occur inside the shaped trigger coincidence window
  //     DPPParams[b].trgc[ch] = CAEN_DGTZ_DPP_TriggerMode_Normal;

  //     /*Discrimination mode for the event selection 
  // 	CAEN_DGTZ_DPP_DISCR_MODE_LED -> Leading Edge Distrimination
  // 	CAEN_DGTZ_DPP_DISCR_MODE_CFD -> Constant Fraction Distrimination*/
  //     DPPParams[b].discr[ch] = CAEN_DGTZ_DPP_DISCR_MODE_LED;

  //     /*CFD delay (N*2ns for x730  and N*4ns for x725)  */
  //     DPPParams[b].cfdd[ch] = 4;  

  //     /*CFD fraction: 0->25%; 1->50%; 2->75%; 3->100% */
  //     DPPParams[b].cfdf[ch] = 0;

  //     /* Trigger Validation Acquisition Window */
  //     DPPParams[b].tvaw[ch] = 50;

  //     /* Charge sensibility: 
  // 	 Options for Input Range 2Vpp: 0->5fC/LSB; 1->20fC/LSB; 2->80fC/LSB; 3->320fC/LSB; 4->1.28pC/LSB; 5->5.12pC/LSB 
  // 	 Options for Input Range 0.5Vpp: 0->1.25fC/LSB; 1->5fC/LSB; 2->20fC/LSB; 3->80fC/LSB; 4->320fC/LSB; 5->1.28pC/LSB */
  //     DPPParams[b].csens[ch] = 0;
  //   }
  // /* Pile-Up rejection Mode
  //    CAEN_DGTZ_DPP_PSD_PUR_DetectOnly -> Only Detect Pile-Up
  //    CAEN_DGTZ_DPP_PSD_PUR_Enabled -> Reject Pile-Up */
  // DPPParams[b].purh = CAEN_DGTZ_DPP_PSD_PUR_DetectOnly;
  // DPPParams[b].purgap = 100;  // Purity Gap in LSB units (1LSB = 0.12 mV for 2Vpp Input Range, 1LSB = 0.03 mV for 0.5 Vpp Input Range )
  // DPPParams[b].blthr = 3;     // Baseline Threshold
  // DPPParams[b].trgho = 8;     // Trigger HoldOff


  SetChannelEnableMask(0xFF);
  SetDPPParameters(0xFF,&dpppsdParams);
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  for (uint32_t i = 0; i < 8; ++i)
    {
      //for x725 and x730 Recordlength is common to paired channels (you can set different RL for different channel pairs)
      if(i%2==0)
	SetRecordLength(1024, i);

      // Set a DC offset to the input signal to adapt it to digitizer's dynamic range
      SetChannelDCOffset(i,0x8000);

      SetDPPPreTriggerSize(i,256);

      // CAEN_DGTZ_PulsePolarityPositive  CAEN_DGTZ_PulsePolarityNegative
      SetChannelPulsePolarity(i,CAEN_DGTZ_PulsePolarityNegative);
    }

  SetVirtualProbe();
  
  if(AllocateMemory())
    {
      // error
      printf("Allocate Memory error\n");
    }
  else
    {
      printf("Allocate Memory OK ...\n");
    }



  PrintRegisters();
}

Board::~Board()
{
  CAEN_DGTZ_SWStopAcquisition(0);
  CAEN_DGTZ_CloseDigitizer(0);
  
  free(dig);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Board::AllocateMemory()
{
  int ret = 0;

  if(readoutBuffer != NULL)
    {
      ret = CAEN_DGTZ_FreeReadoutBuffer(&readoutBuffer);
      readoutBuffer = NULL;
      ret = CAEN_DGTZ_FreeDPPEvents(dig->boardHandle, (void **)dpppsdevents);
      ret = CAEN_DGTZ_FreeDPPWaveforms(dig->boardHandle,dpppsdwaveforms);
    }

 
  ret = CAEN_DGTZ_MallocReadoutBuffer(dig->boardHandle, &readoutBuffer, &bufferSize); // WARNING: This malloc must be done after the digitizer programming
  // std::cout<<"BufferSize: "<<bufferSize<<"  "<<sizeof(readoutBuffer)<<std::endl;
  ret |= CAEN_DGTZ_MallocDPPEvents(dig->boardHandle, (void **)dpppsdevents, &size); 
  // std::cout<<"EventSize: "<<size<<"  "<<sizeof(dpppsdevents)<<std::endl;
  ret |= CAEN_DGTZ_MallocDPPWaveforms(dig->boardHandle, (void **)&dpppsdwaveforms, &size);// Allocate memory for the waveforms 
  // std::cout<<"Waveforms: "<<size<<"  "<<sizeof(dpppsdwaveforms)<<std::endl;
  
  if(ret) printf("Warning: errors malloc space the digitizer. ");
  
  return ret;
}

int Board::SetVirtualProbe()
{
  int ret = 0;
  /* Set the virtual probes

     DPP-PSD for x725 and x730 boards can save:
     2 analog waveforms:
     Analog Trace 1: it can be specified with the ANALOG_TRACE_1 parameter;
     Analog Trace 2: it can be specified with the ANALOG_TRACE_2 parameter
     2 digital waveforms:
     Digital Trace 1:   it can be specified with the DIGITAL_TRACE_1 parameters
     Digital Trace 2:   it can be specified with the DIGITAL_TRACE_2 parameters

     Virtual Probes types for Trace 1:
     CAEN_DGTZ_DPP_VIRTUALPROBE_Input        -> Save the Input signal waveform 
     CAEN_DGTZ_DPP_VIRTUALPROBE_CFD          -> Save the CFD waveform (only if discrimination mode is  CAEN_DGTZ_DPP_DISCR_MODE_CFD)

     Virtual Probes types for Trace 2:
     CAEN_DGTZ_DPP_VIRTUALPROBE_Baseline     -> Save the Baseline waveform (mean on nsbl parameter)
     CAEN_DGTZ_DPP_VIRTUALPROBE_CFD          -> Save the CFD waveform (only if discrimination mode is  CAEN_DGTZ_DPP_DISCR_MODE_CFD)
     CAEN_DGTZ_DPP_VIRTUALPROBE_None			-> Dual trace is disabled

     Digital Probes types for Digital Trace 1(x725, x730):
     CAEN_DGTZ_DPP_DIGITALPROBE_Gate   -> GateLong waveform
     CAEN_DGTZ_DPP_DIGITALPROBE_OverThr
     CAEN_DGTZ_DPP_DIGITALPROBE_TRGOut
     CAEN_DGTZ_DPP_DIGITALPROBE_CoincWin
     CAEN_DGTZ_DPP_DIGITALPROBE_PileUp
     CAEN_DGTZ_DPP_DIGITALPROBE_Coincidence
     CAEN_DGTZ_DPP_DIGITALPROBE_Trigger

     Digital Probes types for Digital Trace 2(x725)/3(x730):
     CAEN_DGTZ_DPP_DIGITALPROBE_GateShort
     CAEN_DGTZ_DPP_DIGITALPROBE_OverThr
     CAEN_DGTZ_DPP_DIGITALPROBE_TRGVal
     CAEN_DGTZ_DPP_DIGITALPROBE_TRGHoldoff
     CAEN_DGTZ_DPP_DIGITALPROBE_PileUp
     CAEN_DGTZ_DPP_DIGITALPROBE_Coincidence
     CAEN_DGTZ_DPP_DIGITALPROBE_Trigger
  */

  ret |= CAEN_DGTZ_SetDPP_VirtualProbe(dig->boardHandle, ANALOG_TRACE_1, CAEN_DGTZ_DPP_VIRTUALPROBE_Input);
  ret |= CAEN_DGTZ_SetDPP_VirtualProbe(dig->boardHandle, ANALOG_TRACE_2, CAEN_DGTZ_DPP_VIRTUALPROBE_Baseline);
  ret |= CAEN_DGTZ_SetDPP_VirtualProbe(dig->boardHandle, DIGITAL_TRACE_1, CAEN_DGTZ_DPP_DIGITALPROBE_Gate);
  ret |= CAEN_DGTZ_SetDPP_VirtualProbe(dig->boardHandle, DIGITAL_TRACE_2, CAEN_DGTZ_DPP_DIGITALPROBE_GateShort);

  if (ret) printf("Warning: errors found during the programming of the digitizer.\nSome settings may not be executed\n");
  return ret;
}


void Board::CheckLicense()
{
  uint32_t countdown1, countdown2;
  CAEN_DGTZ_ReadRegister(dig->boardHandle, LICENSE_REG, &countdown1);
  usleep(100000);
  CAEN_DGTZ_ReadRegister(dig->boardHandle, LICENSE_REG, &countdown2);
  if (countdown1 != countdown2 || countdown2 == 0)
    {
      int minleft = countdown2 * LICENSE_COUNTDOWN_MULT / 60000;
      printf("Warning: firmware is unlicensed. Remaining demo mode time: %d minutes\n", minleft);
    }
}

CAEN_DGTZ_ErrorCode Board::OpenDigitizer()
{
  return CAEN_DGTZ_OpenDigitizer(dig->linkType, dig->linkNum, dig->conetNode, dig->baseAddress, &dig->boardHandle);
}

CAEN_DGTZ_ErrorCode Board::GetInfo()
{
  return CAEN_DGTZ_GetInfo(dig->boardHandle, dig->boardInfo); 
}

void Board::PrintInfo()
{
  printf("Connected to CAEN Digitizer Model %s\n", dig->boardInfo->ModelName);
  printf("ROC FPGA Release is %s\n", dig->boardInfo->ROC_FirmwareRel);
  printf("AMC FPGA Release is %s\n", dig->boardInfo->AMC_FirmwareRel);
  printf("Model is %d\n", dig->boardInfo->Model);
  printf("Channels is %d\n", dig->boardInfo->Channels);
  printf("FormFactor is %d\n", dig->boardInfo->FormFactor);
  printf("FamilyCode is %d\n", dig->boardInfo->FamilyCode);
  printf("SerialNumber is %d\n", dig->boardInfo->SerialNumber);
  printf("PCB_Revision is %d\n", dig->boardInfo->PCB_Revision);
  printf("ADC_NBits is %d\n", dig->boardInfo->ADC_NBits);
  printf("CommHandle is %d\n", dig->boardInfo->CommHandle);
  printf("VMEHandle is %d\n", dig->boardInfo->VMEHandle);
  printf("License is %s\n", dig->boardInfo->License);
}

CAEN_DGTZ_ErrorCode Board::GetDPPFirmwareType(CAEN_DGTZ_DPPFirmware_t* firmware)
{
  return CAEN_DGTZ_GetDPPFirmwareType(dig->boardHandle, firmware);
}

CAEN_DGTZ_ErrorCode Board::ReadTemperature(int32_t ch, uint32_t *temp)
{
  return CAEN_DGTZ_ReadTemperature(dig->boardHandle, ch, temp);
}

CAEN_DGTZ_ErrorCode Board::Calibrate()
{
  return CAEN_DGTZ_Calibrate(dig->boardHandle);
}

CAEN_DGTZ_ErrorCode Board::WriteRegister(uint32_t Address, uint32_t Data)
{
  return CAEN_DGTZ_WriteRegister(dig->boardHandle, Address, Data);
}

CAEN_DGTZ_ErrorCode Board::ReadRegister(uint32_t Address, uint32_t *Data)
{
  return CAEN_DGTZ_ReadRegister(dig->boardHandle, Address, Data);
}

CAEN_DGTZ_ErrorCode Board::Reset()
{
  return CAEN_DGTZ_Reset(dig->boardHandle);
}

CAEN_DGTZ_ErrorCode Board::ClearData()
{
  return CAEN_DGTZ_ClearData(dig->boardHandle);
}

CAEN_DGTZ_ErrorCode Board::SendSWtrigger()
{
  return CAEN_DGTZ_SendSWtrigger(dig->boardHandle);
}

CAEN_DGTZ_ErrorCode Board::SWStartAcquisition()
{
  return CAEN_DGTZ_SWStartAcquisition(dig->boardHandle);
}

CAEN_DGTZ_ErrorCode Board::SWStopAcquisition()
{
  return CAEN_DGTZ_SWStopAcquisition(dig->boardHandle);
}

CAEN_DGTZ_ErrorCode Board::SetAcquisitionMode(CAEN_DGTZ_AcqMode_t mode)
{
  return CAEN_DGTZ_SetAcquisitionMode(dig->boardHandle, mode);
}

CAEN_DGTZ_ErrorCode Board::GetAcquisitionMode(CAEN_DGTZ_AcqMode_t *mode)
{
  return CAEN_DGTZ_GetAcquisitionMode(dig->boardHandle, mode);

}

CAEN_DGTZ_ErrorCode Board::SetRunSynchronizationMode(CAEN_DGTZ_RunSyncMode_t mode)
{
  return CAEN_DGTZ_SetRunSynchronizationMode(dig->boardHandle, mode);
}

CAEN_DGTZ_ErrorCode Board::GetRunSynchronizationMode(CAEN_DGTZ_RunSyncMode_t* mode)
{
  return CAEN_DGTZ_GetRunSynchronizationMode(dig->boardHandle, mode);
}


CAEN_DGTZ_ErrorCode Board::SetSWTriggerMode(CAEN_DGTZ_TriggerMode_t mode)
{
  return CAEN_DGTZ_SetSWTriggerMode(dig->boardHandle, mode);
}

CAEN_DGTZ_ErrorCode Board::GetSWTriggerMode(CAEN_DGTZ_TriggerMode_t *mode)
{
  return CAEN_DGTZ_GetSWTriggerMode(dig->boardHandle, mode);
}

CAEN_DGTZ_ErrorCode Board::SetExtTriggerInputMode(CAEN_DGTZ_TriggerMode_t mode)
{
  return CAEN_DGTZ_SetSWTriggerMode(dig->boardHandle, mode);
}

CAEN_DGTZ_ErrorCode Board::GetExtTriggerInputMode(CAEN_DGTZ_TriggerMode_t *mode)
{
  return CAEN_DGTZ_GetExtTriggerInputMode(dig->boardHandle, mode);
}

CAEN_DGTZ_ErrorCode Board::SetChannelSelfTrigger(CAEN_DGTZ_TriggerMode_t mode, uint32_t channelmask)
{
  return CAEN_DGTZ_SetChannelSelfTrigger(dig->boardHandle, mode, channelmask);
}

CAEN_DGTZ_ErrorCode Board::GetChannelSelfTrigger(uint32_t channel, CAEN_DGTZ_TriggerMode_t *mode)
{
  return CAEN_DGTZ_GetChannelSelfTrigger(dig->boardHandle, channel, mode);
}



CAEN_DGTZ_ErrorCode Board::SetTriggerPolarity(uint32_t channel, CAEN_DGTZ_TriggerPolarity_t Polarity)
{
  return CAEN_DGTZ_SetTriggerPolarity(dig->boardHandle, channel, Polarity);
}

CAEN_DGTZ_ErrorCode Board::GetTriggerPolarity(uint32_t channel, CAEN_DGTZ_TriggerPolarity_t *Polarity)
{
  return CAEN_DGTZ_GetTriggerPolarity(dig->boardHandle, channel, Polarity);
}

CAEN_DGTZ_ErrorCode Board::SetIOLevel(CAEN_DGTZ_IOLevel_t level)
{
  return CAEN_DGTZ_SetIOLevel(dig->boardHandle, level);
}

CAEN_DGTZ_ErrorCode Board::GetIOLevel(CAEN_DGTZ_IOLevel_t *level)
{
  return CAEN_DGTZ_GetIOLevel(dig->boardHandle, level);
}

CAEN_DGTZ_ErrorCode Board::SetRecordLength(uint32_t size, uint32_t channel)
{
  return CAEN_DGTZ_SetRecordLength(dig->boardHandle, size, channel);
}

CAEN_DGTZ_ErrorCode Board::GetRecordLength(uint32_t *size, uint32_t channel)
{
  return CAEN_DGTZ_GetRecordLength(dig->boardHandle, size, channel);
}

CAEN_DGTZ_ErrorCode Board::SetChannelDCOffset(uint32_t channel, uint32_t Tvalue)
{
  return CAEN_DGTZ_SetChannelDCOffset(dig->boardHandle, channel, Tvalue);
}

CAEN_DGTZ_ErrorCode Board::GetChannelDCOffset(uint32_t channel, uint32_t *Tvalue)
{
  return CAEN_DGTZ_GetChannelDCOffset(dig->boardHandle, channel, Tvalue);
}

CAEN_DGTZ_ErrorCode Board::SetChannelPulsePolarity(uint32_t channel, CAEN_DGTZ_PulsePolarity_t pol)
{
  return CAEN_DGTZ_SetChannelPulsePolarity(dig->boardHandle, channel, pol);
}

CAEN_DGTZ_ErrorCode Board::GetChannelPulsePolarity(uint32_t channel, CAEN_DGTZ_PulsePolarity_t* pol)
{
  return CAEN_DGTZ_GetChannelPulsePolarity(dig->boardHandle, channel, pol);
}














CAEN_DGTZ_ErrorCode Board::SetDPPPreTriggerSize(int ch, uint32_t samples)
{
  return CAEN_DGTZ_SetDPPPreTriggerSize(dig->boardHandle, ch, samples);
}

CAEN_DGTZ_ErrorCode Board::GetDPPPreTriggerSize(int ch, uint32_t *samples)
{
  return CAEN_DGTZ_GetDPPPreTriggerSize(dig->boardHandle, ch, samples);
}


CAEN_DGTZ_ErrorCode Board::SetChannelEnableMask(uint32_t mask)
{
  return CAEN_DGTZ_SetChannelEnableMask(dig->boardHandle, mask);
}

CAEN_DGTZ_ErrorCode Board::GetChannelEnableMask(uint32_t *mask)
{
return CAEN_DGTZ_GetChannelEnableMask(dig->boardHandle, mask);
}

CAEN_DGTZ_ErrorCode Board::SetDPPParameters(uint32_t channelMask, void* params)
{
  return CAEN_DGTZ_SetDPPParameters(dig->boardHandle, channelMask, (void*) params);
}

CAEN_DGTZ_ErrorCode Board::SetDPPAcquisitionMode(CAEN_DGTZ_DPP_AcqMode_t mode, CAEN_DGTZ_DPP_SaveParam_t param)
{
  return CAEN_DGTZ_SetDPPAcquisitionMode(dig->boardHandle,mode, param);
}

CAEN_DGTZ_ErrorCode Board::GetDPPAcquisitionMode(CAEN_DGTZ_DPP_AcqMode_t *mode, CAEN_DGTZ_DPP_SaveParam_t *param)
{
  return CAEN_DGTZ_GetDPPAcquisitionMode(dig->boardHandle,mode, param);
}

CAEN_DGTZ_ErrorCode Board::SetDPPEventAggregation(int threshold, int maxsize)
{
  return CAEN_DGTZ_SetDPPEventAggregation(dig->boardHandle, threshold, maxsize);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Board::Read_DGTZ_Register_725_730_DPP_PSD_Revision05(int handle,int MaxNChInBoard)
{
  int ret = 0;
  uint32_t Register_Data;
  char bstr[64];
  bzero(bstr, sizeof(bstr));

  printf("\n\n==========================================================\n\n");

  // Record Length
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1020 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Record Length (0x1%d20) (channel %d): %s\n",ch,ch,bstr);
    }

  // Input Dynamic Range
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1028 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Input Dynamic Range (0x1%d28) (channel %d): %s\n",ch,ch,bstr);
    }

  // Number of Events per Aggregate
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1034 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Number of Events per Aggregate (0x1%d34) (channel %d): %s\n",ch,ch,bstr);
    }

  // Pre Trigger
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1038 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Pre Trigger (0x1%d38) (channel %d): %s\n",ch,ch,bstr);
    }

  // CFD settings
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x103C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("CFD settings (0x1%d3C) (channel %d): %s\n",ch,ch,bstr);
    }

  // Charge Zero Suppression Threshold
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1044 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Charge Zero Suppression Threshold (0x1%d44) (channel %d): %s\n",ch,ch,bstr);
    }

  // Short Gate Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1054 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Short Gate Width (0x1%d54) (channel %d): %s\n",ch,ch,bstr);
    }

  // Long Gate Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1058 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Long Gate Width (0x1%d58) (channel %d): %s\n",ch,ch,bstr);
    }

  // Gate Offset
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x105C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Gate Offset (0x1%d5C) (channel %d): %s\n",ch,ch,bstr);
    }

  // Trigger Threshold
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1060 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Trigger Threshold (0x1%d60) (channel %d): %s\n",ch,ch,bstr);
    }

  // Fixed Baseline
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1064 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Fixed Baseline (0x1%d64) (channel %d): %s\n",ch,ch,bstr);
    }

  // Shaped Trigger Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1070 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Shaped Trigger Width (0x1%d70) (channel %d): %s\n",ch,ch,bstr);
    }  
    
  // Trigger Hold-Off Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1074 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Trigger Hold-Off Width (0x1%d74) (channel %d): %s\n",ch,ch,bstr);
    }  
      
  // Threshold for the PSD cut
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1078 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Threshold for the PSD cut (0x1%d78) (channel %d): %s\n",ch,ch,bstr);
    }    

  // PUR-GAP Threshold
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x107C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("PUR-GAP Threshold (0x1%d7C) (channel %d): %s\n",ch,ch,bstr);
    }    

  // DPP Algorithm Control
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1080 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP Algorithm Control (0x1%d80) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP Algorithm Control 2
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1084 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP Algorithm Control 2 (0x1%d84) (channel %d): %s\n",ch,ch,bstr);
    }      
  
  // Channel n Status
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1088 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel n Status (0x1%d88) (channel %d): %s\n",ch,ch,bstr);
    }    

  // AMC Firmware Revision
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x108C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("AMC Firmware Revision (0x1%d8C) (channel %d): %s\n",ch,ch,bstr);
    }

  // DC Offset
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1098 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DC Offset (0x1%d98) (channel %d): %s\n",ch,ch,bstr);
    }     

  // Channel n ADC Temperature
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x10A8 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel n ADC Temperature (0x1%dA8) (channel %d): %s\n",ch,ch,bstr);
    }    

  // Veto Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x10D4 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Veto Width (0x1%dD4) (channel %d): %s\n",ch,ch,bstr);
    }

  // Baseline Freeze Time
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x10D8 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Baseline Freeze Time (0x1%dD8) (channel %d): %s\n",ch,ch,bstr);
    }

  // Board Configuration
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8000, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board Configuration (0x8000): %s\n",bstr);


  // Aggregate Organization
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x800C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Aggregate Organization (0x800C): %s\n",bstr);

  // Acquisition Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8100, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Acquisition Control (0x8100): %s\n",bstr);  

  // Acquisition Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8104, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Acquisition Status (0x8104): %s\n",bstr);

  // Global Trigger Mask
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x810C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Global Trigger Mask (0x810C): %s\n",bstr);

  // Front Panel TRG-OUT (GPO) Enable Mask
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8110, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Front Panel TRG-OUT (GPO) Enable Mask (0x8110): %s\n",bstr);

  // LVDS I/O Data
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8118, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("LVDS I/O Data (0x8118): %s\n",bstr);  

  // Front Panel I/O Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x811C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Front Panel I/O Control (0x811C): %s\n",bstr);

  // Channel Enable Mask
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8120, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Channel Enable Mask (0x8120): %s\n",bstr);  

  // ROC FPGA Firmware Revision
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8124, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("ROC FPGA Firmware Revision (0x8124): %s\n",bstr);

  // Set Monitor DAC
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8138, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Set Monitor DAC (0x8138): %s\n",bstr);  

  // Board Info
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8140, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board Info (0x8140): %s\n",bstr);

  // Monitor DAC Mode
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8144, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Monitor DAC Mode (0x8144): %s\n",bstr);

  // Event Size
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x814C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Event Size (0x814C): %s\n",bstr);

  // Time Bomb Downcounter
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8158, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Time Bomb Downcounter (0x8158): %s\n",bstr); 

  // Fan Speed Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8168, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Fan Speed Control (0x8168): %s\n",bstr);

  // Run/Start/Stop Delay
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8170, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Run/Start/Stop Delay (0x8170): %s\n",bstr);

  // Board Failure Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8178, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board Failure Status (0x8178): %s\n",bstr);

  // Disable External Trigger
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x817C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Disable External Trigger (0x817C): %s\n",bstr);  

  // Trigger Validation Mask
  for (int ch = 0; ch < MaxNChInBoard/2; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, 0x8180+4*ch, &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Trigger Validation Mask (0x8180+(4n)) (couple index %d): %s\n",ch,bstr);
    }    
  
  // Front Panel LVDS I/O New Features
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x81A0, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Front Panel LVDS I/O New Features (0x81A0): %s\n",bstr);

  // Readout Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF00, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Readout Control (0xEF00): %s\n",bstr);

  // Readout Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF04, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Readout Status (0xEF04): %s\n",bstr);

  // Board ID
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF08, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board ID (0xEF08): %s\n",bstr);

  // MCST Base Address and Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF0C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("MCST Base Address and Control (0xEF0C): %s\n",bstr);

  // Relocation Address
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF10, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Relocation Address (0xEF10): %s\n",bstr); 

  // Interrupt Status/ID
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF14, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Interrupt Status/ID (0xEF14): %s\n",bstr); 
  
  // Interrupt Event Number
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF18, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Interrupt Event Number (0xEF18): %s\n",bstr); 
  
  // Aggregate Number per BLT
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF1C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Aggregate Number per BLT (0xEF1C): %s\n",bstr); 
    
  // Scratch
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF20, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Scratch (0xEF20): %s\n",bstr); 

  // Configuration ROM Checksum
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF000, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum (0xF000): %s\n",bstr); 
  
  // Configuration ROM Checksum Length BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF004, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum Length BYTE 2 (0xF004): %s\n",bstr); 

  // Configuration ROM Checksum Length BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF008, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum Length BYTE 1 (0xF008): %s\n",bstr); 

  // Configuration ROM Checksum Length BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF00C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum Length BYTE 0 (0xF00C): %s\n",bstr); 
  
  // Configuration ROM Constant BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF010, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Constant BYTE 2 (0xF010): %s\n",bstr); 

  // Configuration ROM Constant BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF014, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Constant BYTE 1 (0xF014): %s\n",bstr); 

  // Configuration ROM Constant BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF018, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Constant BYTE 0 (0xF018): %s\n",bstr); 
  
  // Configuration ROM R Code
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF020, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM R Code (0xF020): %s\n",bstr); 

  // Configuration ROM IEEE OUI BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF024, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM IEEE OUI BYTE 2 (0xF024): %s\n",bstr); 

  // Configuration ROM IEEE OUI BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF028, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM IEEE OUI BYTE 1 (0xF028): %s\n",bstr); 

  // Configuration ROM IEEE OUI BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF02C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM IEEE OUI BYTE 0 (0xF02C): %s\n",bstr); 
  
  // Configuration ROM Board Version
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF030, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Version (0xF030): %s\n",bstr); 

  // Configuration ROM Board Form Factor
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF034, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Form Factor (0xF034): %s\n",bstr); 

  // Configuration ROM Board ID BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF038, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board ID BYTE 1 (0xF038): %s\n",bstr); 

  // Configuration ROM Board ID BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF03C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board ID BYTE 0 (0xF03C): %s\n",bstr);   
  
  // Configuration ROM PCB Revision BYTE 3
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF040, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 3 (0xF040): %s\n",bstr); 

  // Configuration ROM PCB Revision BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF044, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 2 (0xF044): %s\n",bstr); 

  // Configuration ROM PCB Revision BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF048, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 1 (0xF048): %s\n",bstr); 

  // Configuration ROM PCB Revision BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF04C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 0 (0xF04C): %s\n",bstr);

  // Configuration ROM FLASH Type
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF050, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM FLASH Type (0xF050): %s\n",bstr);

  // Configuration ROM Board Serial Number BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF080, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Serial Number BYTE 1 (0xF080): %s\n",bstr);

  // Configuration ROM Board Serial Number BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF084, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Serial Number BYTE 0 (0xF084): %s\n",bstr);

  // Configuration ROM VCXO Type
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF088, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM VCXO Type (0xF088): %s\n",bstr);
  

  
  if(ret)
    {
      printf("Error:  Read_DGTZ_Register_725_730_DPP_PSD_Revision03 !!!\n");
    }  
  
}


// Set a bit in a 32-bit unsigned integer.
unsigned int Board::SetBit_32(unsigned short bit,unsigned int value)//将value第bit位设为1
{
  return(value | (unsigned int)(std::pow(2.0, (double)bit)));
}

// Clear a bit in a 32-bit unsigned integer.
unsigned int Board::ClrBit_32(unsigned short bit,unsigned int value)//将value第bit位设为0
{
  value = SetBit_32(bit, value);
  return(value ^ (unsigned int)(std::pow(2.0, (double)bit)));
}

// Test a bit in a 32-bit unsigned integer.
unsigned int Board::TstBit_32(unsigned short bit,unsigned int value )//获得value第bit位数值
{
  return(((value & (unsigned int)(std::pow(2.0, (double)bit))) >> bit));
}


int Board::Decimal2Binary(uint32_t d, char *bstr)
{
  // if (d < 0) return -1;
  int mod = 0;
  char tmpstr[64];
  bzero(tmpstr, sizeof(tmpstr));
  bzero(bstr, sizeof(bstr));
 
  unsigned int i = 0;
  if(d==0)
    {
      tmpstr[0]='0';
      i++;
    }
  while (d > 0)
    {
      mod = d % 2;
      d /= 2;
      tmpstr[i] = mod + '0';
      i++;
    }
 
  //copy
  unsigned int len = strlen(tmpstr);
  for (i = 0; i < len; i++)
    {
      bstr[i] = tmpstr[len - i - 1];
    }
  return (int)len;
}

long Board::GetTime()
{
  long time_ms;
  struct timeval t1;
  struct timezone tz;
  gettimeofday(&t1, &tz);
  time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
  return time_ms;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void Board::PrintRegisters()
{
  Read_DGTZ_Register_725_730_DPP_PSD_Revision05(dig->boardHandle,8);
}




// 
// Board.cc ends here
