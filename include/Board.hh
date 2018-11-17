// Board.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 11月 12 18:06:49 2018 (+0800)
// Last-Updated: 五 11月 16 18:53:40 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 8
// URL: http://wuhongyi.cn 

#ifndef _BOARD_H_
#define _BOARD_H_

#include "CAENDigitizer.h"
#include "CAENDigitizerType.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define LICENSE_REG 0x8158
#define LICENSE_COUNTDOWN_MULT 84

typedef struct {
  CAEN_DGTZ_BoardInfo_t* boardInfo;
  
  CAEN_DGTZ_ConnectionType linkType;
  int linkNum;
  int conetNode;
  int baseAddress;
  int boardHandle;
  
} Digitizer;


class Board
{
public:
  Board();
  virtual ~Board();


  void CheckLicense();

  // Writes a 32-bit word in a specific address offset of the digitizer
  CAEN_DGTZ_ErrorCode WriteRegister(uint32_t Address, uint32_t Data);
  // Reads a 32-bit word from a specific address offset of the digitizer
  CAEN_DGTZ_ErrorCode ReadRegister(uint32_t Address, uint32_t *Data);



  


  
  // Sets the IO Level
  CAEN_DGTZ_ErrorCode SetIOLevel(CAEN_DGTZ_IOLevel_t level);
  // Gets the IO Level
  CAEN_DGTZ_ErrorCode GetIOLevel(CAEN_DGTZ_IOLevel_t *level);
  
  // Sets the trigger polarity of a specified channel
  CAEN_DGTZ_ErrorCode SetTriggerPolarity(uint32_t channel, CAEN_DGTZ_TriggerPolarity_t Polarity);
  // Gets the trigger polarity of a specified channel
  CAEN_DGTZ_ErrorCode GetTriggerPolarity(uint32_t channel, CAEN_DGTZ_TriggerPolarity_t *Polarity);
  

  // Calibrate the board
  CAEN_DGTZ_ErrorCode Calibrate();

  // Read the temperature in celsius for the given channel
  CAEN_DGTZ_ErrorCode ReadTemperature(int32_t ch, uint32_t *temp);
  
  // Get the DPP firmware type
  CAEN_DGTZ_ErrorCode GetDPPFirmwareType(CAEN_DGTZ_DPPFirmware_t* firmware);

private:
  Digitizer* dig;


protected:

  
};

#endif /* _BOARD_H_ */
// 
// Board.hh ends here
