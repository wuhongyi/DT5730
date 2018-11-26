// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: Thu Apr 28 20:56:02 2016 (+0800)
// Last-Updated: 一 11月 26 19:32:01 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 141
// URL: http://wuhongyi.cn 

#include "TRint.h"


#include "DigitizerGlobals.hh"
#include "DigitizerConfigure.hh"
#include "DigitizerUser.hh"
// #include "DigitizerAdmin.hh"

#include <iostream>

#include <cstring>
#include <string>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  // Create an interactive ROOT application
  TRint *theApp = new TRint("Rint", &argc, argv);

  // The following variables will store the digitizer configuration parameters 
  CAEN_DGTZ_DPP_PSD_Params_t PKU_DGTZ_DPPParams[MAXNB];//ok
  CAEN_DGTZ_BoardInfo_t PKU_DGTZ_BoardInfo;//ok
  CAEN_DGTZ_DPP_PSD_Event_t *PKU_DGTZ_Events[MaxNChannels];  // events buffer  ok
  CAEN_DGTZ_DPP_PSD_Waveforms_t *PKU_DGTZ_Waveform=NULL;     // waveforms buffer  ok

  int PKU_DGTZ_handle[MAXNB];//ok
  DigitizerParams_t PKU_DGTZ_Params[MAXNB];//ok
  DigitizerRun_t PKU_DGTZ_RunManager;//ok

  // Buffers to store the data. 
  uint32_t PKU_DGTZ_AllocatedSize;
  char *PKU_DGTZ_buffer = NULL;  // readout buffer   ok

   // TCanvas *PKU_DGTZ_Canvas = NULL;
   // TGraph *PKU_DGTZ_Graph = NULL;
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  int ret;
  int b,ch,ev;
  bool flagplot;

  RunManagerInit(&PKU_DGTZ_RunManager);
  if(DigitizerInit(PKU_DGTZ_DPPParams,PKU_DGTZ_Params,PKU_DGTZ_handle,PKU_DGTZ_BoardInfo) < 0) return -1;
  // WARNING: The mallocs MUST be done after the digitizer programming, because the following functions needs to know the digitizer configuration to allocate the right memory amount 
  // Allocate memory for the readout buffer
  ret = CAEN_DGTZ_MallocReadoutBuffer(PKU_DGTZ_handle[0], &PKU_DGTZ_buffer, &PKU_DGTZ_AllocatedSize);
  // Allocate memory for the events
  ret|= CAEN_DGTZ_MallocDPPEvents(PKU_DGTZ_handle[0], (void **)PKU_DGTZ_Events, &PKU_DGTZ_AllocatedSize);
  // Allocate memory for the waveforms
  ret|= CAEN_DGTZ_MallocDPPWaveforms(PKU_DGTZ_handle[0], (void **)&PKU_DGTZ_Waveform, &PKU_DGTZ_AllocatedSize);
  if (ret) 
    {
      printf("Can't allocate memory buffers\n");
      QuitProgram(PKU_DGTZ_handle,PKU_DGTZ_buffer);
      return ret;
    }

  PKU_DGTZ_RunManager.PrevRateTime = get_time();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // Readout Loop
  PrintInterface();

  while(!PKU_DGTZ_RunManager.Quit) 
    {
      CheckKeyboard(&PKU_DGTZ_RunManager,PKU_DGTZ_handle,PKU_DGTZ_DPPParams,PKU_DGTZ_Params);

      if (!PKU_DGTZ_RunManager.AcqRun) 
  	{
  	  Sleep(10);
  	  continue;
  	}

      PrintRunningStatus(&PKU_DGTZ_RunManager);

      // Read data from the boards
      for (b = 0; b < MAXNB; b++)   
  	{
  	  // Read data from the board
  	  ret = CAEN_DGTZ_ReadData(PKU_DGTZ_handle[b], CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, PKU_DGTZ_buffer, &PKU_DGTZ_RunManager.BufferSize);//
	  // std::cout<<PKU_DGTZ_RunManager.BufferSize<<std::endl;
  	  if (ret) 
  	    {
  	      printf("Readout Error\n");
  	      QuitProgram(PKU_DGTZ_handle,PKU_DGTZ_buffer);
	      PKU_DGTZ_RunManager.Quit = true;
	      continue;
            }
  	  if (PKU_DGTZ_RunManager.BufferSize == 0)
  	    {
  	      // printf("Board %d ,BufferSize==0\n",b);
  	      continue;
  	    }
  	  PKU_DGTZ_RunManager.Nb += PKU_DGTZ_RunManager.BufferSize;

  	  // Analyze data
  	  ret|= CAEN_DGTZ_GetDPPEvents(PKU_DGTZ_handle[b], PKU_DGTZ_buffer, PKU_DGTZ_RunManager.BufferSize, (void**)PKU_DGTZ_Events, PKU_DGTZ_RunManager.NumEvents);//Get Event

  	  if (ret) 
  	    {
  	      printf("Data Error: %d\n", ret);
  	      QuitProgram(PKU_DGTZ_handle,PKU_DGTZ_buffer);
	      PKU_DGTZ_RunManager.Quit = true;
	      continue;
  	    }

  	  for (ch = 0; ch < MaxNChannels; ch++) 
  	    {
  	      if (!(PKU_DGTZ_Params[b].ChannelMask & (1<<ch)))
  	  	continue;

  	      for (ev = 0; ev < PKU_DGTZ_RunManager.NumEvents[ch]; ev++)
  	  	{
  		  PKU_DGTZ_RunManager.TrgCnt[b][ch]++;
  		  PKU_DGTZ_RunManager.PrevTime[b][ch] = PKU_DGTZ_Events[ch][ev].TimeTag;

  		  if (PKU_DGTZ_Events[ch][ev].ChargeLong > 0 && PKU_DGTZ_Events[ch][ev].ChargeShort)// Energy
  		    {
  		      PKU_DGTZ_RunManager.ECnt[b][ch]++;
  		    }
  		  else
  		    {// PileUp
  		      PKU_DGTZ_RunManager.PurCnt[b][ch]++;
  		    }

  		  flagplot = PKU_DGTZ_RunManager.PlotFlag && (PKU_DGTZ_RunManager.DoPlotBoard == b) && (PKU_DGTZ_RunManager.DoPlotChannel == ch);
  		  if((PKU_DGTZ_Params[b].DPPAcqMode != CAEN_DGTZ_DPP_ACQ_MODE_List) && (PKU_DGTZ_RunManager.WriteFlag || flagplot))//CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope  CAEN_DGTZ_DPP_ACQ_MODE_Mixed
  		    {
  		      int size;
  	  	      uint16_t *WaveLine;
  	  	      CAEN_DGTZ_DecodeDPPWaveforms(PKU_DGTZ_handle[b], &PKU_DGTZ_Events[ch][ev], PKU_DGTZ_Waveform);// Get Waveform

  		      //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
		      
		      // Use waveform data here...
  	  	      size = (int)(PKU_DGTZ_Waveform->Ns); // Number of samples
  	  	      WaveLine = PKU_DGTZ_Waveform->Trace1;//Waveform. 

  		      if(PKU_DGTZ_RunManager.WriteFlag)
			{
			  // SaveWaveform(b, ch, size,PKU_DGTZ_Events[ch][ev].ChargeLong, WaveLine, PKU_DGTZ_RunManager.PrevTime[b][ch]);
			  printf("%d %d %d %d %d\n",ch,size,PKU_DGTZ_Events[ch][ev].TimeTag,PKU_DGTZ_Events[ch][ev].ChargeShort,PKU_DGTZ_Events[ch][ev].ChargeLong);
			}
  		      if(flagplot)//plot
  		      	{
  		      	  if(PKU_DGTZ_RunManager.PlotEveryN%PKU_DGTZ_RunManager.PlotChooseN == 0)
  		      	    {
  		      	      PlotROOTGraph(&PKU_DGTZ_RunManager, b, ch, size, WaveLine);// do plot
			      PKU_DGTZ_RunManager.PlotEveryN ++;
  		      	    }
  		      	  else
  		      	    {
  		      	      PKU_DGTZ_RunManager.PlotEveryN ++;
  		      	    }
  		      	}
		      
  		    }

  		}// loop on NumEvents

  	    }// loop on channel
	  
  	}// loop on boards

    }

  // and enter the event loop...
  theApp->Run();
  delete theApp;
  
  return 0;
}



// 
// main.cc ends here
