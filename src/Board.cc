// Board.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 11月 12 18:07:01 2018 (+0800)
// Last-Updated: 六 11月 17 20:25:51 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 27
// URL: http://wuhongyi.cn 

#include "Board.hh"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
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

// 
// Board.cc ends here
