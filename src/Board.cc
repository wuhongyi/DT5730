// Board.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 11月 12 18:07:01 2018 (+0800)
// Last-Updated: 五 11月 16 18:52:57 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
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

  dig = (Digitizer*) calloc(1,sizeof(Digitizer));
  dig->boardInfo = (CAEN_DGTZ_BoardInfo_t*) calloc(1,sizeof(CAEN_DGTZ_BoardInfo_t));

  dig->linkType = CAEN_DGTZ_USB;
  dig->linkNum = 0;
  dig->conetNode = 0;
  dig->baseAddress = 0;
}

Board::~Board()
{
  free(dig);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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



// 
// Board.cc ends here
