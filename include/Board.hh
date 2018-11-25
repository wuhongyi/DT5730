// Board.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 11月 12 18:06:49 2018 (+0800)
// Last-Updated: 日 11月 25 21:02:08 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 25
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


// typedef struct {
//   int blthr;
//   int bltmo;
//   int trgho;
//   int thr	[MAX_DPP_PSD_CHANNEL_SIZE];
//   int selft	[MAX_DPP_PSD_CHANNEL_SIZE];
//   int csens	[MAX_DPP_PSD_CHANNEL_SIZE];
//   int sgate	[MAX_DPP_PSD_CHANNEL_SIZE];
//   int lgate	[MAX_DPP_PSD_CHANNEL_SIZE];
//   int pgate	[MAX_DPP_PSD_CHANNEL_SIZE];
//   int tvaw	[MAX_DPP_PSD_CHANNEL_SIZE];
//   int nsbl	[MAX_DPP_PSD_CHANNEL_SIZE];
//   int discr	[MAX_DPP_PSD_CHANNEL_SIZE];	//only for FW > 132.32
//   int cfdf	[MAX_DPP_PSD_CHANNEL_SIZE]; //only for FW > 132.32
//   int cfdd	[MAX_DPP_PSD_CHANNEL_SIZE]; //only for FW > 132.32
//   CAEN_DGTZ_DPP_TriggerConfig_t trgc // Ignored for x751
//   [MAX_DPP_PSD_CHANNEL_SIZE];
//   CAEN_DGTZ_DPP_PUR_t purh; 
//   int purgap; // Ignored for x751
// } CAEN_DGTZ_DPP_PSD_Params_t;

// typedef struct 
// {
//   uint32_t Format;
//   uint32_t Format2;
//   uint32_t TimeTag;
//   int16_t ChargeShort;
//   int16_t ChargeLong;
//   int16_t Baseline;
//   int16_t Pur;
//   uint32_t *Waveforms;
//   uint32_t Extras;
// } CAEN_DGTZ_DPP_PSD_Event_t;

// typedef struct
// {
//   uint32_t Ns;
//   uint8_t  dualTrace;
//   uint8_t  anlgProbe;
//   uint8_t  dgtProbe1;
//   uint8_t  dgtProbe2;
//   uint16_t *Trace1;
//   uint16_t *Trace2;
//   uint8_t  *DTrace1;
//   uint8_t  *DTrace2;
//   uint8_t  *DTrace3;
//   uint8_t  *DTrace4;
// } CAEN_DGTZ_DPP_PSD_Waveforms_t;


class Board
{
public:
  Board();
  virtual ~Board();

  int AllocateMemory();
  int SetVirtualProbe();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
  
  void CheckLicense();

  // Writes a 32-bit word in a specific address offset of the digitizer
  CAEN_DGTZ_ErrorCode WriteRegister(uint32_t Address, uint32_t Data);
  // Reads a 32-bit word from a specific address offset of the digitizer
  CAEN_DGTZ_ErrorCode ReadRegister(uint32_t Address, uint32_t *Data);

  // Resets the Digitizer. All internal registers and states are restored to defaults.
  CAEN_DGTZ_ErrorCode Reset();
  // Clears the data stored in the buffers of the Digitizer
  CAEN_DGTZ_ErrorCode ClearData();
  // Sends a Software trigger to the Digitizer
  CAEN_DGTZ_ErrorCode SendSWtrigger();

  // Starts Digitizers acquisition.
  CAEN_DGTZ_ErrorCode SWStartAcquisition();
  // Stops Digitizer acquisition 
  CAEN_DGTZ_ErrorCode SWStopAcquisition();

  // Sets digitizer acquisition mode
  CAEN_DGTZ_ErrorCode SetAcquisitionMode(CAEN_DGTZ_AcqMode_t mode);
  // Gets the acquisition mode of the digitizer 
  CAEN_DGTZ_ErrorCode GetAcquisitionMode(CAEN_DGTZ_AcqMode_t *mode);

  // Sets the run synchronization mode of the digitizer, used to synchronize an acquisition on multiple boards
  CAEN_DGTZ_ErrorCode SetRunSynchronizationMode(CAEN_DGTZ_RunSyncMode_t mode);
  // Gets the run synchronization mode of the digitizer 
  CAEN_DGTZ_ErrorCode GetRunSynchronizationMode(CAEN_DGTZ_RunSyncMode_t* mode);
  
  // Sets one of the available trigger mode
  CAEN_DGTZ_ErrorCode SetSWTriggerMode(CAEN_DGTZ_TriggerMode_t mode);
  // Gets software trigger mode
  CAEN_DGTZ_ErrorCode GetSWTriggerMode(CAEN_DGTZ_TriggerMode_t *mode);
  // Sets external trigger input mode
  CAEN_DGTZ_ErrorCode SetExtTriggerInputMode(CAEN_DGTZ_TriggerMode_t mode);
  // Gets current external trigger input mode
  CAEN_DGTZ_ErrorCode GetExtTriggerInputMode(CAEN_DGTZ_TriggerMode_t *mode);
  // Sets channel self trigger mode according to CAEN_DGTZ_TriggerMode_t
  // NOTE: since x730 board family has even and odd channels paired, the user
  // shouldn't call this function separately for the channels of the same pair,
  // otherwise the second call will overwrite the setting of the first one. The
  // user should instead call at maximum once for every pair with the relevant
  // bits of the channelmask already set to the correct value.
  CAEN_DGTZ_ErrorCode SetChannelSelfTrigger(CAEN_DGTZ_TriggerMode_t mode, uint32_t channelmask);
  // Gets current channel self trigger mode setting.
  CAEN_DGTZ_ErrorCode GetChannelSelfTrigger(uint32_t channel, CAEN_DGTZ_TriggerMode_t *mode);

  
  
  // Sets the IO Level
  CAEN_DGTZ_ErrorCode SetIOLevel(CAEN_DGTZ_IOLevel_t level);
  // Gets the IO Level
  CAEN_DGTZ_ErrorCode GetIOLevel(CAEN_DGTZ_IOLevel_t *level);

  // Sets channels that will be  enabled into events
  CAEN_DGTZ_ErrorCode SetChannelEnableMask(uint32_t mask);
  // Gets current mask of enabled channels in events.
  // If a mask bit is set, corresponding channel is currenly enabled for event readout
  CAEN_DGTZ_ErrorCode GetChannelEnableMask(uint32_t *mask);
  
  // Sets the trigger polarity of a specified channel
  CAEN_DGTZ_ErrorCode SetTriggerPolarity(uint32_t channel, CAEN_DGTZ_TriggerPolarity_t Polarity);
  // Gets the trigger polarity of a specified channel
  CAEN_DGTZ_ErrorCode GetTriggerPolarity(uint32_t channel, CAEN_DGTZ_TriggerPolarity_t *Polarity);
  
  // Sets acquisition record length
  CAEN_DGTZ_ErrorCode SetRecordLength(uint32_t size, uint32_t channel);
  // Gets current acquisition record length
  CAEN_DGTZ_ErrorCode GetRecordLength(uint32_t *size, uint32_t channel);

  // Sets the DC offset for a specified channel
  CAEN_DGTZ_ErrorCode SetChannelDCOffset(uint32_t channel, uint32_t Tvalue);
  // Gets the DC offset for a specified channel
  CAEN_DGTZ_ErrorCode GetChannelDCOffset(uint32_t channel, uint32_t *Tvalue);

  // Set the pulse polarity for the specified channel
  CAEN_DGTZ_ErrorCode SetChannelPulsePolarity(uint32_t channel, CAEN_DGTZ_PulsePolarity_t pol);
  // Get the value of the pulse polarity for the specified channel
  CAEN_DGTZ_ErrorCode GetChannelPulsePolarity(uint32_t channel, CAEN_DGTZ_PulsePolarity_t* pol);


  
  // Sets the pre-trigger size, which is the portion of acquisition window visible before a trigger
  // ch=-1 writes the value for all channels.
  CAEN_DGTZ_ErrorCode SetDPPPreTriggerSize(int ch, uint32_t samples);
  // Gets the pre-trigger size
  CAEN_DGTZ_ErrorCode GetDPPPreTriggerSize(int ch, uint32_t *samples);

  // Set DPP configuration parameters
  CAEN_DGTZ_ErrorCode SetDPPParameters(uint32_t channelMask, void* params);

  // Set the DPP acquisition mode.
  CAEN_DGTZ_ErrorCode SetDPPAcquisitionMode(CAEN_DGTZ_DPP_AcqMode_t mode, CAEN_DGTZ_DPP_SaveParam_t param);
  // Get the information about the DPP acquisition mode.
  CAEN_DGTZ_ErrorCode GetDPPAcquisitionMode(CAEN_DGTZ_DPP_AcqMode_t *mode, CAEN_DGTZ_DPP_SaveParam_t *param);

  // Set event aggregation parameters
  CAEN_DGTZ_ErrorCode SetDPPEventAggregation(int threshold, int maxsize);


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  void PrintRegisters();



  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  
  // Calibrate the board
  CAEN_DGTZ_ErrorCode Calibrate();

  // Read the temperature in celsius for the given channel
  CAEN_DGTZ_ErrorCode ReadTemperature(int32_t ch, uint32_t *temp);
  
  // Get the DPP firmware type
  CAEN_DGTZ_ErrorCode GetDPPFirmwareType(CAEN_DGTZ_DPPFirmware_t* firmware);

  // Retrieves the board information of the digitizer
  CAEN_DGTZ_ErrorCode GetInfo();
  void PrintInfo();
  
  CAEN_DGTZ_ErrorCode OpenDigitizer();


  // return  0=success; -1=error
  void Read_DGTZ_Register_725_730_DPP_PSD_Revision05(int handle,int MaxNChInBoard);

  // Set a bit in a 32-bit unsigned integer.
  unsigned int SetBit_32(unsigned short bit,unsigned int value);//将value第bit位设为1

  // Clear a bit in a 32-bit unsigned integer.
  unsigned int ClrBit_32(unsigned short bit,unsigned int value);//将value第bit位设为0

  // Test a bit in a 32-bit unsigned integer.
  unsigned int TstBit_32(unsigned short bit,unsigned int value );//获得value第bit位数值

  // 十进制转二进制
  // d 十进制数
  // bstr 二进制字符串
  // return 出错返回-1，成功返回字符串长度
  int Decimal2Binary(uint32_t d, char *bstr);

  // Get time in milliseconds
  // return  time in msec
  long GetTime();

  
private:
  Digitizer* dig;

  CAEN_DGTZ_DPP_PSD_Params_t dpppsdParams;
  CAEN_DGTZ_DPP_PSD_Event_t *dpppsdevents[MAX_DPP_PSD_CHANNEL_SIZE];
  CAEN_DGTZ_DPP_PSD_Waveforms_t *dpppsdwaveforms;
  
protected:
  char* readoutBuffer;
  uint32_t bufferSize;
  char *EventPtr;
  uint32_t size;


};

#endif /* _BOARD_H_ */
// 
// Board.hh ends here
