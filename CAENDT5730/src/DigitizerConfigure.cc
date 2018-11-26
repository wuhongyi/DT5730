// DigitizerConfigure.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: Thu Apr 28 22:16:56 2016 (+0800)
// Last-Updated: 一 11月 26 20:57:37 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 199
// URL: http://wuhongyi.cn 

#include "DigitizerGlobals.hh"
#include "DigitizerConfigure.hh"
#include "DigitizerUser.hh"
//#include "DigitizerAdmin.hh"
// #include "TAxis.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int DigitizerInit(CAEN_DGTZ_DPP_PSD_Params_t *PKU_DGTZ_DPPParams,DigitizerParams_t *PKU_DGTZ_Params,int *PKU_DGTZ_handle,CAEN_DGTZ_BoardInfo_t PKU_DGTZ_BoardInfo)
{
  int b;
  int ret;
  std::string tempstring1,tempstring2;

  // Set Parameters 
  memset(PKU_DGTZ_Params, 0, MAXNB * sizeof(DigitizerParams_t));
  memset(PKU_DGTZ_DPPParams, 0, MAXNB * sizeof(CAEN_DGTZ_DPP_PSD_Params_t));

  for (b = 0; b < MAXNB; b++) 
    {
      // if(ReadGlobalsParameters(&PKU_DGTZ_Params[b]) < 0) return -1;
      //=====LinkType=====
      tempstring1 = ReadValue<std::string>("LinkType",PKU_DGTZ_GlobalParametersFileName);
      tempstring2 = ReadValue<std::string>("VMEBaseAddress",PKU_DGTZ_GlobalParametersFileName);
      std::cout<<"LinkType:"<<" "<<tempstring1<<" "<<"VMEBaseAddress:"<<" "<<tempstring2<<std::endl;
      if(tempstring1 == "CAEN_DGTZ_PCI_OpticalLink")
      	{
      	  PKU_DGTZ_Params[b].LinkType = CAEN_DGTZ_PCI_OpticalLink;
      	  if(tempstring2=="0")
      	    {
      	      PKU_DGTZ_Params[b].VMEBaseAddress = 0;
      	      PKU_DGTZ_Params[b].LinkNum = 0;
      	      PKU_DGTZ_Params[b].ConetNode = b;
      	    }
      	  else 
      	    {
      	      if(tempstring2=="0x32100000")
      		{
      		  PKU_DGTZ_Params[b].VMEBaseAddress = 0x32100000;
      		  PKU_DGTZ_Params[b].LinkNum = 0;
      		  PKU_DGTZ_Params[b].ConetNode = 0;
      		}
      	      else {printf("VMEBaseAddress error \n");return -1;} 
      	    }
      	}
      else 
      	{
      	  if(tempstring1 == "CAEN_DGTZ_USB")
      	    {
      	      PKU_DGTZ_Params[b].LinkType = CAEN_DGTZ_USB;
      	      if(tempstring2=="0")
      		{
      		  PKU_DGTZ_Params[b].VMEBaseAddress = 0;
      		  PKU_DGTZ_Params[b].LinkNum = b;
      		  PKU_DGTZ_Params[b].ConetNode =0;
      		}
      	      else 
      		{
      		  if(tempstring2=="0x32100000")
      		    {
      		      PKU_DGTZ_Params[b].VMEBaseAddress = 0x32100000;
      		      PKU_DGTZ_Params[b].LinkNum = 0;
      		      PKU_DGTZ_Params[b].ConetNode =0;
      		    }
      		  else {printf("VMEBaseAddress error \n");return -1;} 
      		}
      	    }
      	  else 
      	    {
      	      if(tempstring1 == "CAEN_DGTZ_PCIE_OpticalLink")
      		{
      		  PKU_DGTZ_Params[b].LinkType = CAEN_DGTZ_PCIE_OpticalLink;
      		  if(tempstring2=="0x32100000")
      		    {
      		      PKU_DGTZ_Params[b].VMEBaseAddress = 0x32100000;
      		      PKU_DGTZ_Params[b].LinkNum = 0;
      		      PKU_DGTZ_Params[b].ConetNode = 0;
      		    }
      		  else {printf("VMEBaseAddress error \n");return -1;} 
      		}
      	      else 
      		{
      		  printf("LinkType error \n");return -1;
      		}
      	    }
      	}

      //=====IOlev=====
      tempstring1 = ReadValue<std::string>("IOlev",PKU_DGTZ_GlobalParametersFileName);
      std::cout<<"IOlev:"<<" "<<tempstring1<<std::endl;
      if(tempstring1 == "CAEN_DGTZ_IOLevel_TTL")
      	{
      	  PKU_DGTZ_Params[b].IOlev = CAEN_DGTZ_IOLevel_TTL;
      	}
      else
      	{
      	  if(tempstring1 == "CAEN_DGTZ_IOLevel_NIM")
      	    {
      	      PKU_DGTZ_Params[b].IOlev = CAEN_DGTZ_IOLevel_NIM;
      	    }
      	  else
      	    {
      	      printf("IOlev error \n");return -1;
      	    }
      	}

      //=====DPPAcqMode=====
      // CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope / CAEN_DGTZ_DPP_ACQ_MODE_List / CAEN_DGTZ_DPP_ACQ_MODE_Mixed  
      tempstring1 = ReadValue<std::string>("DPPAcqMode",PKU_DGTZ_GlobalParametersFileName);
      std::cout<<"DPPAcqMode:"<<" "<<tempstring1<<std::endl;
      if(tempstring1 == "CAEN_DGTZ_DPP_ACQ_MODE_Mixed")
      	{
      	  PKU_DGTZ_Params[b].DPPAcqMode = CAEN_DGTZ_DPP_ACQ_MODE_Mixed;
      	}
      else
      	{
      	  if(tempstring1 == "CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope")
      	    {
      	      PKU_DGTZ_Params[b].DPPAcqMode = CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope;
      	    }
      	  else
      	    {
      	      if(tempstring1 == "CAEN_DGTZ_DPP_ACQ_MODE_List")
      		{
      		  PKU_DGTZ_Params[b].DPPAcqMode = CAEN_DGTZ_DPP_ACQ_MODE_List;
      		}
      	      else
      		{
      		  printf("DPPAcqMode error \n");return -1;
      		}
      	    }
      	}

      //=====AcqMode=====
      // CAEN_DGTZ_SW_CONTROLLED / CAEN_DGTZ_S_IN_CONTROLLED / CAEN_DGTZ_FIRST_TRG_CONTROLLED
      tempstring1 = ReadValue<std::string>("AcqMode",PKU_DGTZ_GlobalParametersFileName);
      std::cout<<"AcqMode:"<<" "<<tempstring1<<std::endl;
      if(tempstring1 == "CAEN_DGTZ_SW_CONTROLLED")
      	{
      	  PKU_DGTZ_Params[b].AcqMode = CAEN_DGTZ_SW_CONTROLLED;
      	}
      else
      	{
      	  if(tempstring1 == "CAEN_DGTZ_S_IN_CONTROLLED")
      	    {
      	      PKU_DGTZ_Params[b].AcqMode = CAEN_DGTZ_S_IN_CONTROLLED;
      	    }
      	  else
      	    {
      	      if(tempstring1 == "CAEN_DGTZ_FIRST_TRG_CONTROLLED")
      		{
      		  PKU_DGTZ_Params[b].AcqMode = CAEN_DGTZ_FIRST_TRG_CONTROLLED;
      		}
      	      else
      		{
      		  printf("DPPAcqMode error \n");return -1;
      		}
      	    }
      	}    
      
      //=====SaveParam=====
      // CAEN_DGTZ_DPP_SAVE_PARAM_EnergyOnly   Only energy (DPP-PHA) or charge (DPP-PSD/DPP-CI v2) is returned 
      // CAEN_DGTZ_DPP_SAVE_PARAM_TimeOnly   Only time is returned 
      // CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime  Both energy/charge and time are returned 
      // CAEN_DGTZ_DPP_SAVE_PARAM_ChargeAndTime  deprecated On DPP-PSD and DPP-CI use CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime 
      // CAEN_DGTZ_DPP_SAVE_PARAM_None    No histogram data is returned 
      tempstring1 = ReadValue<std::string>("SaveParam",PKU_DGTZ_GlobalParametersFileName);
      std::cout<<"SaveParam:"<<" "<<tempstring1<<std::endl;
      if(tempstring1 == "CAEN_DGTZ_DPP_SAVE_PARAM_EnergyOnly")
      	{
      	  PKU_DGTZ_Params[b].SaveParam = CAEN_DGTZ_DPP_SAVE_PARAM_EnergyOnly;
      	}
      else
      	{
      	  if(tempstring1 == "CAEN_DGTZ_DPP_SAVE_PARAM_TimeOnly")
      	    {
      	      PKU_DGTZ_Params[b].SaveParam = CAEN_DGTZ_DPP_SAVE_PARAM_TimeOnly;
      	    }
      	  else
      	    {
      	      if(tempstring1 == "CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime")
      		{
      		  PKU_DGTZ_Params[b].SaveParam = CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime;
      		}
      	      else
      		{
      		  if(tempstring1 == "CAEN_DGTZ_DPP_SAVE_PARAM_ChargeAndTime")
      		    {
      		      PKU_DGTZ_Params[b].SaveParam = CAEN_DGTZ_DPP_SAVE_PARAM_ChargeAndTime;
      		    }
      		  else
      		    {
      		      if(tempstring1 == "CAEN_DGTZ_DPP_SAVE_PARAM_None")
      			{
      			  PKU_DGTZ_Params[b].SaveParam = CAEN_DGTZ_DPP_SAVE_PARAM_None;
      			}
      		      else
      			{
      			  printf("SaveParam error \n");return -1;
      			}
      		    }
      		}
      	    }
      	}


      //=====EventAggr=====
      std::cout<<"EventAggr:"<<" "<<ReadValue<int>("EventAggr", PKU_DGTZ_GlobalParametersFileName)<<std::endl;
      PKU_DGTZ_Params[b].EventAggr = ReadValue<int>("EventAggr", PKU_DGTZ_GlobalParametersFileName);
      
      //=====ExtTrgMode=====
      // CAEN_DGTZ_TRGMODE_DISABLED: do nothing
      // CAEN_DGTZ_TRGMODE_EXTOUT_ONLY: generate the Trigger Output signal
      // CAEN_DGTZ_TRGMODE_ACQ_ONLY = generate acquisition trigger
      // CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT = generate both Trigger Output and acquisition trigger
      tempstring1 = ReadValue<std::string>("ExtTrgMode",PKU_DGTZ_GlobalParametersFileName);
      std::cout<<"ExtTrgMode:"<<" "<<tempstring1<<std::endl;
      if(tempstring1 == "CAEN_DGTZ_TRGMODE_DISABLED")
      	{
      	  PKU_DGTZ_Params[b].ExtTrgMode = CAEN_DGTZ_TRGMODE_DISABLED;
      	}
      else
      	{
      	  if(tempstring1 == "CAEN_DGTZ_TRGMODE_EXTOUT_ONLY")
      	    {
      	      PKU_DGTZ_Params[b].ExtTrgMode = CAEN_DGTZ_TRGMODE_EXTOUT_ONLY;
      	    }
      	  else
      	    {
      	      if(tempstring1 == "CAEN_DGTZ_TRGMODE_ACQ_ONLY")
      		{
      		  PKU_DGTZ_Params[b].ExtTrgMode = CAEN_DGTZ_TRGMODE_ACQ_ONLY;
      		}
      	      else
      		{
      		  if(tempstring1 == "CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT")
      		    {
      		      PKU_DGTZ_Params[b].ExtTrgMode = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
      		    }
      		  else
      		    {
      		      printf("ExtTrgMode error \n");return -1;
      		    }
      		}
      	    }
      	}

      //=====SWTrgMode=====
      // CAEN_DGTZ_TRGMODE_DISABLED: do nothing
      // CAEN_DGTZ_TRGMODE_EXTOUT_ONLY: generate the Trigger Output signal
      // CAEN_DGTZ_TRGMODE_ACQ_ONLY = generate acquisition trigger
      // CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT = generate both Trigger Output and acquisition trigger
      tempstring1 = ReadValue<std::string>("SWTrgMode",PKU_DGTZ_GlobalParametersFileName);
      std::cout<<"SWTrgMode:"<<" "<<tempstring1<<std::endl;
      if(tempstring1 == "CAEN_DGTZ_TRGMODE_DISABLED")
      	{
      	  PKU_DGTZ_Params[b].SWTrgMode = CAEN_DGTZ_TRGMODE_DISABLED;
      	}
      else
      	{
      	  if(tempstring1 == "CAEN_DGTZ_TRGMODE_EXTOUT_ONLY")
      	    {
      	      PKU_DGTZ_Params[b].SWTrgMode = CAEN_DGTZ_TRGMODE_EXTOUT_ONLY;
      	    }
      	  else
      	    {
      	      if(tempstring1 == "CAEN_DGTZ_TRGMODE_ACQ_ONLY")
      		{
      		  PKU_DGTZ_Params[b].SWTrgMode = CAEN_DGTZ_TRGMODE_ACQ_ONLY;
      		}
      	      else
      		{
      		  if(tempstring1 == "CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT")
      		    {
      		      PKU_DGTZ_Params[b].SWTrgMode = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
      		    }
      		  else
      		    {
      		      printf("SWTrgMode error \n");return -1;
      		    }
      		}
      	    }
      	}

      //=====SelfTrgMode=====
      // CAEN_DGTZ_TRGMODE_DISABLED: do nothing
      // CAEN_DGTZ_TRGMODE_EXTOUT_ONLY: generate the Trigger Output signal
      // CAEN_DGTZ_TRGMODE_ACQ_ONLY = generate acquisition trigger
      // CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT = generate both Trigger Output and acquisition trigger
      tempstring1 = ReadValue<std::string>("SelfTrgMode",PKU_DGTZ_GlobalParametersFileName);
      std::cout<<"SelfTrgMode:"<<" "<<tempstring1<<std::endl;
      if(tempstring1 == "CAEN_DGTZ_TRGMODE_DISABLED")
      	{
      	  PKU_DGTZ_Params[b].SelfTrgMode = CAEN_DGTZ_TRGMODE_DISABLED;
      	}
      else
      	{
      	  if(tempstring1 == "CAEN_DGTZ_TRGMODE_EXTOUT_ONLY")
      	    {
      	      PKU_DGTZ_Params[b].SelfTrgMode = CAEN_DGTZ_TRGMODE_EXTOUT_ONLY;
      	    }
      	  else
      	    {
      	      if(tempstring1 == "CAEN_DGTZ_TRGMODE_ACQ_ONLY")
      		{
      		  PKU_DGTZ_Params[b].SelfTrgMode = CAEN_DGTZ_TRGMODE_ACQ_ONLY;
      		}
      	      else
      		{
      		  if(tempstring1 == "CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT")
      		    {
      		      PKU_DGTZ_Params[b].SelfTrgMode = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
      		    }
      		  else
      		    {
      		      printf("SelfTrgMode error \n");return -1;
      		    }
      		}
      	    }
      	}

      //=====RunSyncMode=====
      // CAEN_DGTZ_RUN_SYNC_Disabled / CAEN_DGTZ_RUN_SYNC_TrgOutTrgInDaisyChain / CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain / CAEN_DGTZ_RUN_SYNC_SinFanout / CAEN_DGTZ_RUN_SYNC_GpioGpioDaisyChain
      tempstring1 = ReadValue<std::string>("RunSyncMode",PKU_DGTZ_GlobalParametersFileName);
      std::cout<<"RunSyncMode:"<<" "<<tempstring1<<std::endl;
      if(tempstring1 == "CAEN_DGTZ_RUN_SYNC_Disabled")
      	{
      	  PKU_DGTZ_Params[b].RunSyncMode = CAEN_DGTZ_RUN_SYNC_Disabled;
      	}
      else
      	{
      	  if(tempstring1 == "CAEN_DGTZ_RUN_SYNC_TrgOutTrgInDaisyChain")
      	    {
      	      PKU_DGTZ_Params[b].RunSyncMode = CAEN_DGTZ_RUN_SYNC_TrgOutTrgInDaisyChain;
      	    }
      	  else
      	    {
      	      if(tempstring1 == "CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain")
      		{
      		  PKU_DGTZ_Params[b].RunSyncMode = CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain;
      		}
      	      else
      		{
      		  if(tempstring1 == "CAEN_DGTZ_RUN_SYNC_SinFanout")
      		    {
      		      PKU_DGTZ_Params[b].RunSyncMode = CAEN_DGTZ_RUN_SYNC_SinFanout;
      		    }
      		  else
      		    {
      		      if(tempstring1 == "CAEN_DGTZ_RUN_SYNC_GpioGpioDaisyChain")
      			{
      			  PKU_DGTZ_Params[b].RunSyncMode = CAEN_DGTZ_RUN_SYNC_GpioGpioDaisyChain;
      			}
      		      else
      			{
      			  printf("RunSyncMode error \n");return -1;
      			}
      		    }
      		}
      	    }
      	}
 
      //=====AnalogMonOutput=====
      // CAEN_DGTZ_AM_TRIGGER_MAJORITY / CAEN_DGTZ_AM_TEST / CAEN_DGTZ_AM_ANALOG_INSPECTION / CAEN_DGTZ_AM_BUFFER_OCCUPANCY / CAEN_DGTZ_AM_VOLTAGE_LEVEL
      tempstring1 = ReadValue<std::string>("AnalogMonOutput",PKU_DGTZ_GlobalParametersFileName);
      std::cout<<"AnalogMonOutput:"<<" "<<tempstring1<<std::endl;
      if(tempstring1 == "CAEN_DGTZ_AM_TRIGGER_MAJORITY")
      	{
      	  PKU_DGTZ_Params[b].AnalogMonOutput = CAEN_DGTZ_AM_TRIGGER_MAJORITY;
      	}
      else
      	{
      	  if(tempstring1 == "CAEN_DGTZ_AM_TEST")
      	    {
      	      PKU_DGTZ_Params[b].AnalogMonOutput = CAEN_DGTZ_AM_TEST;
      	    }
      	  else
      	    {
      	      if(tempstring1 == "CAEN_DGTZ_AM_ANALOG_INSPECTION")
      		{
      		  PKU_DGTZ_Params[b].AnalogMonOutput = CAEN_DGTZ_AM_ANALOG_INSPECTION;
      		}
      	      else
      		{
      		  if(tempstring1 == "CAEN_DGTZ_AM_BUFFER_OCCUPANCY")
      		    {
      		      PKU_DGTZ_Params[b].AnalogMonOutput = CAEN_DGTZ_AM_BUFFER_OCCUPANCY;
      		    }
      		  else
      		    {
      		      if(tempstring1 == "CAEN_DGTZ_AM_VOLTAGE_LEVEL")
      			{
      			  PKU_DGTZ_Params[b].AnalogMonOutput = CAEN_DGTZ_AM_VOLTAGE_LEVEL;
      			}
      		      else
      			{
      			  printf("AnalogMonOutput error \n");return -1;
      			}
      		    }
      		}
      	    }
      	}
    }

  //=====Read Board Parameters=====
  if(ReadDPPParameters_PSD(PKU_DGTZ_Params,PKU_DGTZ_DPPParams)<0) 
    {
      printf("ReadDPPParameters_PSD(DigitizerParams_t *Params, CAEN_DGTZ_DPP_PSD_Params_t *DPPParams) error \n");return -1;
    }

  // Open the digitizer and read board information   
  for(b=0; b<MAXNB; b++)
    {
      ret = CAEN_DGTZ_OpenDigitizer(PKU_DGTZ_Params[b].LinkType, PKU_DGTZ_Params[b].LinkNum, PKU_DGTZ_Params[b].ConetNode, PKU_DGTZ_Params[b].VMEBaseAddress, &PKU_DGTZ_handle[b]);//
      if (ret) 
	{
	  printf("Can't open digitizer %d\n",b);
	  QuitProgram(PKU_DGTZ_handle);
	  return ret;
	}

      // Once we have the handler to the digitizer, we use it to call the other functions
      ret = CAEN_DGTZ_GetInfo(PKU_DGTZ_handle[b], &PKU_DGTZ_BoardInfo);
      if (ret) 
	{
	  printf("Can't read board info\n");
	  QuitProgram(PKU_DGTZ_handle);
	  return ret;
	}
      printf("\nConnected to CAEN Digitizer Model %s, recognized as board %d\n", PKU_DGTZ_BoardInfo.ModelName, b);
      printf("ROC FPGA Release is %s\n", PKU_DGTZ_BoardInfo.ROC_FirmwareRel);
      printf("AMC FPGA Release is %s\n", PKU_DGTZ_BoardInfo.AMC_FirmwareRel);


    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // Program the digitizer (see function ProgramDigitizer_DT5730) 
  for (b = 0; b < MAXNB; b++) 
    {
      if (ProgramDigitizer_DT5730(PKU_DGTZ_handle[b], PKU_DGTZ_Params[b], PKU_DGTZ_DPPParams[b])) 
	{
	  printf("Failed to program the digitizer\n");
	  QuitProgram(PKU_DGTZ_handle);
	  return ret;
	}
    }

  // for(b=0; b<MAXNB; b++) 
  //   {
  //     ret |= CAEN_DGTZ_SetAnalogMonOutput(PKU_DGTZ_handle[b], PKU_DGTZ_Params[b].AnalogMonOutput);
  //   }

  // GOTO
  for(b=0; b<MAXNB; b++) 
    {
      // Get board info
      std::cout<<std::endl;
      std::cout<<"== Start Board:"<<b<<" Info"<<std::endl;
      ret = Read_DGTZ_Info(PKU_DGTZ_handle[b]);
      if(ret) 
      	{
      	  printf("Read Board Info ERROR!\n");
      	  return -1;
      	}
      std::cout<<"== End Board:"<<b<<" Info"<<std::endl;

      // Read  registers
      printf("\n********* Register Readout for Board %d **************\n", b);
      Read_DGTZ_Register_725_730_DPP_PSD_Revision05(PKU_DGTZ_handle[b],8);
      if(ret) 
      	{
      	  printf("Read registers ERROR!\n");
      	  return -1;
      	}
    }

  return 0;
}

void RunManagerInit(DigitizerRun_t *RunManager)
{

  RunManager->RunNumber = -1;
  RunManager->FileNo = -1;
  
  RunManager->Quit = false;
  RunManager->AcqRun = false;
  RunManager->Nb = 0;

  RunManager->WriteFlag = false;
  
  memset(RunManager->PrevTime, 0, MAXNB*MaxNChannels*sizeof(uint64_t));
  memset(RunManager->ECnt, 0, MAXNB*MaxNChannels*sizeof(int));
  memset(RunManager->TrgCnt, 0, MAXNB*MaxNChannels*sizeof(int));
  memset(RunManager->PurCnt, 0, MAXNB*MaxNChannels*sizeof(int));

  std::string PathToRawData = ReadValue<std::string>("PathToRawData",PKU_DGTZ_GlobalParametersFileName);
  sprintf(RunManager->PathToRawData,"%s",PathToRawData.c_str());
  // std::cout<<RunManager->PathToRawData<<std::endl;

  RunManager->PlotFlag = false;
  RunManager->DoPlotBoard = 0;
  RunManager->DoPlotChannel = 0;
  RunManager->PlotChooseN = ReadValue<int>("PlotChooseN",PKU_DGTZ_GlobalParametersFileName);
}


void CheckKeyboard(DigitizerRun_t *PKU_DGTZ_RunManager,int *PKU_DGTZ_handle,CAEN_DGTZ_DPP_PSD_Params_t *PKU_DGTZ_DPPParams,DigitizerParams_t *PKU_DGTZ_Params)
{
  // printf("Type a command: \n");
  int b;
  // Check keyboard
  if(kbhit())
    {
      PKU_DGTZ_RunManager->Key = getch();
      std::cout<<PKU_DGTZ_RunManager->Key<<std::endl;
      switch(PKU_DGTZ_RunManager->Key)
	{
	case 'q' :
	  {
	    if(PKU_DGTZ_RunManager->AcqRun) 
	      {
		printf("Please enter [s] to stop and enter [q] to quit.\n");
		break;
	      }
	    PKU_DGTZ_RunManager->Quit = true;
	    break;
	  }
	      
	case 's' :
	  {
	    if(PKU_DGTZ_RunManager->AcqRun)
	      {//running,do stop
		for (b = 0; b < MAXNB; b++) 
		  {
		    CAEN_DGTZ_SWStopAcquisition(PKU_DGTZ_handle[b]); // Stop Acquisition
		    printf("Acquisition Stopped for Board %d\n", b);
		  }
		PKU_DGTZ_RunManager->AcqRun = false;

		CloseFile();
		strcpy(PKU_DGTZ_RunManager->StopRunningTime,GetTimeStringYmdHMS());
		WriteRunLog("\ntime: ");
		WriteRunLog(PKU_DGTZ_RunManager->StopRunningTime);
		WriteRunLog("\n==========RunStop==========\n");
		CloseRunLog();
	      }
	    else
	      {//stop,do run
		for (b = 0; b < MAXNB; b++) 
		  {
		    OpenRunLog();
		    strcpy(PKU_DGTZ_RunManager->StartRunningTime,GetTimeStringYmdHMS());
		    WriteRunLog("\n==========RunStart==========\ntime: ");
		    WriteRunLog(PKU_DGTZ_RunManager->StartRunningTime);

		    // NB: the acquisition for each board starts when the following line is executed
		    // so in general the acquisition does NOT starts syncronously for different boards
		    CAEN_DGTZ_SWStartAcquisition(PKU_DGTZ_handle[b]);
		    printf("Acquisition Started for Board %d\n", b);
		  }
		PKU_DGTZ_RunManager->AcqRun = true;
	      }
	    break;
	  }

	case 't' :
	  {
	    for (b = 0; b < MAXNB; b++)
	      CAEN_DGTZ_SendSWtrigger(PKU_DGTZ_handle[b]); // Send a software trigger to each board
	    break;
	  }

	case 'w' :
	  {
	    if(!PKU_DGTZ_RunManager->AcqRun)
	      {
		printf("Please enter [s] start and enter [w] to write data.\n");
		break;
	      }
	    if(PKU_DGTZ_RunManager->WriteFlag)
	      {//writting, do stop
		PKU_DGTZ_RunManager->WriteFlag = false;
		strcpy(PKU_DGTZ_RunManager->StopWritingTime,GetTimeStringYmdHMS());

		CloseFile();
	      }
	    else
	      {//not write, do start
		PKU_DGTZ_RunManager->WriteFlag = true;
		PKU_DGTZ_RunManager->RunNumber = ReadAndUpdateRunNumber("../Log/RunNumber");
		PKU_DGTZ_RunManager->FileNo = -1;//important

		strcpy(PKU_DGTZ_RunManager->StartWritingTime,GetTimeStringYmdHMS());
		SaveLogFile(PKU_DGTZ_RunManager->StartWritingTime,PKU_DGTZ_GlobalParametersFileName);
		SaveLogFile(PKU_DGTZ_RunManager->StartWritingTime,PKU_DGTZ_BoardParametersFileName);
		char templog[32];
		sprintf(templog,"\nFileNumber: %04d",PKU_DGTZ_RunManager->RunNumber);
		WriteRunLog(templog);

		UpdateFileName(PKU_DGTZ_RunManager);
		OpenFile(PKU_DGTZ_RunManager);
	      }	
	    break;
	  }

	case 'R' :
	  {
	    if(PKU_DGTZ_RunManager->WriteFlag)
	      {
		printf("Please enter [w] stop write data and enter [R] restart.\n");
		break;
	      }
	    for (b = 0; b < MAXNB; b++) 
	      {
		CAEN_DGTZ_SWStopAcquisition(PKU_DGTZ_handle[b]); 
		printf("Restarted\n");
		CAEN_DGTZ_ClearData(PKU_DGTZ_handle[b]);
		memset(PKU_DGTZ_DPPParams, 0, MAXNB * sizeof(CAEN_DGTZ_DPP_PSD_Params_t));
		if(ReadDPPParameters_PSD(PKU_DGTZ_Params,PKU_DGTZ_DPPParams)<0) 
		  {
		    printf("ReadDPPParameters_PSD(DigitizerParams_t *Params, CAEN_DGTZ_DPP_PSD_Params_t *DPPParams) Error,Can't Restart Acquisition \n");
		    exit(-1);
		  }
		CAEN_DGTZ_SWStartAcquisition(PKU_DGTZ_handle[b]);
	      }
	    PKU_DGTZ_RunManager->PlotChooseN = ReadValue<int>("PlotChooseN",PKU_DGTZ_GlobalParametersFileName);
	    WriteRunLog("\nRestart");
	    break;
	  }

	case 'p' :
	  {
	    if(PKU_DGTZ_RunManager->PlotFlag)
	      {//ploting,do stop
		PKU_DGTZ_RunManager->PlotFlag = false;
		if(PKU_DGTZ_Graph != NULL)
		  {
		    delete PKU_DGTZ_Graph;
		    PKU_DGTZ_Graph = NULL;
		  }
		if(PKU_DGTZ_Canvas != NULL)
		  {
		    delete PKU_DGTZ_Canvas;
		    PKU_DGTZ_Canvas = NULL;
		  }
	      }
	    else
	      {// not plot ,do start
		PKU_DGTZ_RunManager->PlotFlag = true;
		PKU_DGTZ_RunManager->DoPlotBoard = 0;
		PKU_DGTZ_RunManager->DoPlotChannel = 0;
		PKU_DGTZ_RunManager->PlotEveryN = 0;
		PKU_DGTZ_RunManager->PlotColor = true;
		
		if(PKU_DGTZ_Canvas == NULL)
		  PKU_DGTZ_Canvas = new TCanvas("PKU_DGTZ_Canvas","PKU Digitizer Online",CANVASX,CANVASY);
		if(PKU_DGTZ_Graph == NULL)
		  {
		    PKU_DGTZ_Graph = new TGraph();
		    // PKU_DGTZ_Graph->GetXaxis()->SetTitle("time[10us/1000]");
		    // PKU_DGTZ_Graph->GetYaxis()->SetTitle("channel");
		  }
	      }
	    break;
	  }

	case '0' :
	  {
	    PKU_DGTZ_RunManager->PlotEveryN = 0;
	    break;
	  }
	  
	case '4' :
	  {
	    if(PKU_DGTZ_RunManager->DoPlotBoard > 0)
	      {
		PKU_DGTZ_RunManager->DoPlotBoard--;
		PKU_DGTZ_RunManager->PlotEveryN = 0;
	      }
	    break;
	  }
	case '6' :
	  {
	    if(PKU_DGTZ_RunManager->DoPlotBoard < MAXNB-1)
	      {
		PKU_DGTZ_RunManager->DoPlotBoard++;
		PKU_DGTZ_RunManager->PlotEveryN = 0;
	      }
	    break;
	  }
	case '2' :
	  {
	    if(PKU_DGTZ_RunManager->DoPlotChannel > 0)
	      {
		PKU_DGTZ_RunManager->DoPlotChannel--;
		PKU_DGTZ_RunManager->PlotEveryN = 0;
	      }
	    break;
	  }
	case '8' :
	  {
	    if(PKU_DGTZ_RunManager->DoPlotChannel < MaxNChannels-1)
	      {
		PKU_DGTZ_RunManager->DoPlotChannel++;
		PKU_DGTZ_RunManager->PlotEveryN = 0;
	      }
	    break;
	  }

	default: break;
	}
    }
}

void PrintRunningStatus(DigitizerRun_t *PKU_DGTZ_RunManager)
{
  PKU_DGTZ_RunManager->CurrentTime = get_time();
  PKU_DGTZ_RunManager->ElapsedTime = PKU_DGTZ_RunManager->CurrentTime - PKU_DGTZ_RunManager->PrevRateTime; /* milliseconds */

  if (PKU_DGTZ_RunManager->ElapsedTime > 1000)
    {
      // GOTO
      UpdateWhenFileFillUp(PKU_DGTZ_RunManager);
      
      int b,i;
      system(CLEARSCR);
      PrintInterface();

      printf("Readout Rate=%.2f MB\n", (float)PKU_DGTZ_RunManager->Nb/((float)PKU_DGTZ_RunManager->ElapsedTime*1048.576f));

      // GOTO ,need to beautify in screen
      for(b=0; b<MAXNB; b++)
	{
	  printf("\nBoard %d:\n",b);
	  for(i=0; i<MaxNChannels; i++)
	    {
	      if (PKU_DGTZ_RunManager->TrgCnt[b][i]>0)
		printf("\tCh %d:\tTrgRate=%.3f kHz\tPileUpRate=%.2f%%\n", i, (float)PKU_DGTZ_RunManager->TrgCnt[b][i]/(float)PKU_DGTZ_RunManager->ElapsedTime, (float)PKU_DGTZ_RunManager->PurCnt[b][i]*100/(float)PKU_DGTZ_RunManager->TrgCnt[b][i]);
	      else
		printf("\tCh %d:\tNo Data\n", i);
	      
	      PKU_DGTZ_RunManager->TrgCnt[b][i]=0;
	      PKU_DGTZ_RunManager->PurCnt[b][i]=0;
	    }
	}

      PKU_DGTZ_RunManager->Nb = 0;
      PKU_DGTZ_RunManager->PrevRateTime = PKU_DGTZ_RunManager->CurrentTime;

      printf("Status:\n");
      if(PKU_DGTZ_RunManager->AcqRun) printf("Start acquisition,");
      if(PKU_DGTZ_RunManager->WriteFlag) printf(" Writing file Number: %d\n",PKU_DGTZ_RunManager->RunNumber);
      else printf(" Not Write ......\n");
      if(PKU_DGTZ_RunManager->PlotFlag)
	printf("Monitor: B-%d Ch-%d\n",PKU_DGTZ_RunManager->DoPlotBoard,PKU_DGTZ_RunManager->DoPlotChannel);
      // GOTO
    }
}

void PlotROOTGraph(DigitizerRun_t *PKU_DGTZ_RunManager,int b,int ch,int size,uint16_t *WaveData)
{
  char picturename[32];
  int i;
  sprintf(picturename, "Board %d - Channel %d;time[10us/1000];channel",b,ch);
  // if(PKU_DGTZ_Graph == NULL) std::cout<<"^^^^^^^^^"<<std::endl;
  PKU_DGTZ_Graph->SetTitle(picturename);
  for(i=0; i<size; i++)
    {
      PKU_DGTZ_Graph->SetPoint(i,i,WaveData[i]);
    }
  if(PKU_DGTZ_RunManager->PlotColor)
    {
      PKU_DGTZ_Graph->SetLineColor(2);
      PKU_DGTZ_RunManager->PlotColor = false;
    }
  else
    {
      PKU_DGTZ_Graph->SetLineColor(1);
      PKU_DGTZ_RunManager->PlotColor = true;
    }
  PKU_DGTZ_Canvas->cd();
  PKU_DGTZ_Graph->Draw("AL");
  // PKU_DGTZ_Canvas->Modified();
  PKU_DGTZ_Canvas->Update();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Quit the Program
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void QuitProgram(int *PKU_DGTZ_handle)
{
  // stop the acquisition, close the device and free the buffers
  int b;
  for (b =0 ; b < MAXNB; b++) 
    {
      CAEN_DGTZ_SWStopAcquisition(PKU_DGTZ_handle[b]);
      CAEN_DGTZ_CloseDigitizer(PKU_DGTZ_handle[b]);
    }
}
void QuitProgram(int *PKU_DGTZ_handle,char *PKU_DGTZ_buffer)
{
  // stop the acquisition, close the device and free the buffers
  int b;
  for (b =0 ; b < MAXNB; b++) 
    {
      CAEN_DGTZ_SWStopAcquisition(PKU_DGTZ_handle[b]);
      CAEN_DGTZ_CloseDigitizer(PKU_DGTZ_handle[b]);
    }
  CAEN_DGTZ_FreeReadoutBuffer(&PKU_DGTZ_buffer);
}

int ProgramDigitizer_DT5730(int handle, DigitizerParams_t Params, CAEN_DGTZ_DPP_PSD_Params_t DPPParams)
{
  // uint32_t ReadRegister;
  
  // This function uses the CAENDigitizer API functions to perform the digitizer's initial configuration
  int i, ret = 0;

  // Reset the digitizer
  ret |= CAEN_DGTZ_Reset(handle);
  if (ret) 
    {
      printf("ERROR: can't reset the digitizer.\n");
      return -1;
    }

  // set the configuration register for basic function
  // ret |= CAEN_DGTZ_WriteRegister(handle, 0x8000, 0x01000114);  // Channel Control Reg (indiv trg, seq readout) ??
  // ret |= CAEN_DGTZ_WriteRegister(handle,0x811C, 0xC003C); // serve per mandare SW trg individuali e per abilitare il ts reset
  ret |= CAEN_DGTZ_WriteRegister(handle,0x8004, 0x00020000); // Extras Recording
  // for (int ch = 0; ch < 8; ++ch)
  // ret |= CAEN_DGTZ_WriteRegister(handle,(0x1084 | (ch<<8)), 0x207); 


  
  // Set the DPP acquisition mode
  //   This setting affects the modes Mixed and List (see CAEN_DGTZ_DPP_AcqMode_t definition for details)
  ret |= CAEN_DGTZ_SetDPPAcquisitionMode(handle, Params.DPPAcqMode, Params.SaveParam);

  // Set the digitizer acquisition mode (CAEN_DGTZ_SW_CONTROLLED or CAEN_DGTZ_S_IN_CONTROLLED)
  ret |= CAEN_DGTZ_SetAcquisitionMode(handle, Params.AcqMode);

  // Set the number of samples for each waveform
  ret |= CAEN_DGTZ_SetRecordLength(handle, Params.RecordLength);

  // Set the I/O level (CAEN_DGTZ_IOLevel_NIM or CAEN_DGTZ_IOLevel_TTL)
  ret |= CAEN_DGTZ_SetIOLevel(handle, Params.IOlev);

  /* Set the digitizer's behaviour when an external trigger arrives:
     see CAENDigitizer user manual, chapter "Trigger configuration" for details */
  ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, Params.ExtTrgMode);

  // Set the digitizer's behaviour when a software trigger arrives
  ret |= CAEN_DGTZ_SetSWTriggerMode(handle, Params.SWTrgMode);

  // Set the enabled channels
  ret |= CAEN_DGTZ_SetChannelEnableMask(handle, Params.ChannelMask);

  // Set the digitizer's behaviour when a self trigger arrives
  // ret |= CAEN_DGTZ_SetChannelSelfTrigger(handle, Params.SelfTrgMode, Params.ChannelMask);
  // 开启该功能时，所有路被动触发

  // Set how many events to accumulate in the board memory before being available for readout
  ret |= CAEN_DGTZ_SetDPPEventAggregation(handle, Params.EventAggr, 0);




  /* Set the mode used to syncronize the acquisition between different boards.
     In this example the sync is disabled */
  ret |= CAEN_DGTZ_SetRunSynchronizationMode(handle, Params.RunSyncMode);

  // Set the delay time between the arrival of the start signal at the input of the board (either SIN or TRGIN) and the actual start of run
  ret |= CAEN_DGTZ_WriteRegister(handle, 0x8170, Params.Run_Start_Stop_Delay);

  // Set the DPP specific parameters for the channels in the given channelMask
  ret |= CAEN_DGTZ_SetDPPParameters(handle, Params.ChannelMask, &DPPParams);

  for(i=0; i<MaxNChannels; i++) 
    {
      if (Params.ChannelMask & (1<<i)) 
	{
	  // Set a DC offset to the input signal to adapt it to digitizer's dynamic range
	  ret |= CAEN_DGTZ_SetChannelDCOffset(handle, i,Params.ChannelDCOffset[i] );

	  // Set the Pre-Trigger size (in samples)
	  ret |= CAEN_DGTZ_SetDPPPreTriggerSize(handle, i, Params.DPPPreTriggerSize[i]);

	  // Set the polarity for the given channel (CAEN_DGTZ_PulsePolarityPositive or CAEN_DGTZ_PulsePolarityNegative)
	  ret |= CAEN_DGTZ_SetChannelPulsePolarity(handle, i, Params.PulsePolarity[i]);
	}
    }

  // GOTO
  // 这里涉及到采样频率问题。全采样或者降低一半频率来记录两种信息。
  // ret |= CAEN_DGTZ_SetDPP_VirtualProbe(handle, ANALOG_TRACE_1, CAEN_DGTZ_DPP_VIRTUALPROBE_Input);
  // ret |= CAEN_DGTZ_SetDPP_VirtualProbe(handle, ANALOG_TRACE_2, CAEN_DGTZ_DPP_VIRTUALPROBE_TrapezoidReduced);
  // ret |= CAEN_DGTZ_SetDPP_VirtualProbe(handle, DIGITAL_TRACE_1, CAEN_DGTZ_DPP_DIGITALPROBE_Peaking);

  // typedef enum
  // {
  //     CAEN_DGTZ_DPP_PSD_VIRTUALPROBE_Baseline     = 0L,
  //     CAEN_DGTZ_DPP_PSD_VIRTUALPROBE_Threshold    = 1L,
  // } CAEN_DGTZ_DPP_PSD_VirtualProbe_t;

  // typedef enum
  // {
  //     /************************************************************
  //     *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
  //     *  The following values are valid for the following DPP-PSD *
  //     *  Firmwares:                                               *
  //     *       x720 Boards: AMC_REL <= 131.5                       *
  //     *       x751 Boards: AMC_REL <= 132.5                       *
  //     *  For newer firmwares, use the values marked with 'R6' in  *
  //     *  the name.                                                *
  //     *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
  //     ************************************************************/
    
  //     /* x720 Digital Probes Types */
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_Armed           = 0L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_Trigger         = 1L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_ChargeReady     = 2L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_PileUp          = 3L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_BlOutSafeBand   = 4L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_BlTimeout       = 5L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_CoincidenceMet  = 6L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_Tvaw            = 7L,

  //     /* x751 Digital Probes Types */
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_OverThr         = 8L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_GateShort       = 9L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_None            = 10L,

  //     /************************************************************
  //     *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
  //     *  The following values are valid for the following DPP-PSD *
  //     *  Firmwares:                                               *
  //     *       x720 Boards: AMC_REL >= 131.6                       *
  //     *       x751 Boards: AMC_REL >= 132.6                       *
  //     *  For older firmwares, use the values above.               *
  //     *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
  //     ************************************************************/
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_ExtTrg       = 11L, /* x720 only */
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_OverThr      = 12L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_TrigOut      = 13L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_CoincWin     = 14L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_PileUp       = 15L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_Coincidence  = 16L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_GateLong     = 17L, 
  // } CAEN_DGTZ_DPP_PSD_DigitalProbe1_t;


  // typedef enum
  // {
  //     /************************************************************
  //     *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
  //     *  The following values are valid for the following DPP-PSD *
  //     *  Firmwares:                                               *
  //     *       x720 Boards: AMC_REL <= 131.5                       *
  //     *       x751 Boards: AMC_REL <= 132.5                       *
  //     *  For newer firmwares, use the values marked with 'R6' in  *
  //     *  the name.                                                *
  //     *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
  //     ************************************************************/

  //     /* x720 Digital Probes Types */
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_Armed           = 0L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_Trigger         = 1L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_ChargeReady     = 2L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_PileUp          = 3L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_BlOutSafeBand   = 4L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_BlTimeout       = 5L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_CoincidenceMet  = 6L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_Tvaw            = 7L,

  //     /* x751 Digital Probes Types */
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_GateShort       = 8L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_GateLong        = 9L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_None            = 10L,

  //     /************************************************************
  //     *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
  //     *  The following values are valid for the following DPP-PSD *
  //     *  Firmwares:                                               *
  //     *       x720 Boards: AMC_REL >= 131.6                       *
  //     *       x751 Boards: AMC_REL >= 132.6                       *
  //     *  For older firmwares, use the values above.               *
  //     *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
  //     ************************************************************/
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_R6_GateShort    = 11L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_R6_OverThr      = 12L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_R6_TrgVal       = 13L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_R6_TrgHO        = 14L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_R6_PileUp       = 15L,
  //     CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_R6_Coincidence  = 16L,
  // } CAEN_DGTZ_DPP_PSD_DigitalProbe2_t;


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

  ret |= CAEN_DGTZ_SetDPP_VirtualProbe(handle, ANALOG_TRACE_1, CAEN_DGTZ_DPP_VIRTUALPROBE_Input);
  ret |= CAEN_DGTZ_SetDPP_VirtualProbe(handle, ANALOG_TRACE_2, CAEN_DGTZ_DPP_VIRTUALPROBE_Baseline);
  ret |= CAEN_DGTZ_SetDPP_VirtualProbe(handle, DIGITAL_TRACE_1, CAEN_DGTZ_DPP_DIGITALPROBE_Gate);
  ret |= CAEN_DGTZ_SetDPP_VirtualProbe(handle, DIGITAL_TRACE_2, CAEN_DGTZ_DPP_DIGITALPROBE_GateShort);
  
  
  if (ret) 
    {
      printf("Warning: errors found during the programming of the digitizer.\nSome settings may not be executed\n");
      return ret;
    }

  return 0;
}



int ReadDPPParameters_PSD(DigitizerParams_t *Params, CAEN_DGTZ_DPP_PSD_Params_t *DPPParams)
{
  int b,ch,i;
  FILE *file;
  int RecLen;
  int ChaMas;
  int TriggerHoldOff;
  char chamas[16];//
  int Board_Read;
  char All[4], Pol[4];
  char str[255], BoardTag[MAXNB][255];
  float Params_Data[MaxNChannels][PARAMS_NUM];
  int ChNo;
  long file_pos1, file_pos2;
  uint32_t RunStartStopDelay;
  file = fopen(PKU_DGTZ_BoardParametersFileName, "r");

  for(b=0; b<MAXNB; b++) 
    {
      sprintf(BoardTag[b], "*** Board: %02d ***\n", b);
      Board_Read=0;
      ChaMas=0;

      // read file
      while(fgets(str,255,file)!=NULL) 
	{
	  // for Board tag
	  if(strcmp(str,BoardTag[b])!=0) continue;
	  printf("\n%s", str);
	  Board_Read=1;

	  if(strcmp(fgets(str,255,file),"[RunStartStopDelay]\n")!=0) 
	    {
	      printf("Read [RunStartStopDelay] ERROR! <DPP_Parameters.txt>\n"); 
	      return -1;
	    }
	  fscanf(file,"%u\n",&RunStartStopDelay);
	  Params[b].Run_Start_Stop_Delay = RunStartStopDelay;
	  printf("[RunStartStopDelay]\n%u\n",RunStartStopDelay);

	  if(strcmp(fgets(str,255,file),"[RecordLength]\n")!=0) 
	    {
	      printf("Read [RecordLength] ERROR! <DPP_Parameters.txt>\n"); 
	      return -1;
	    }
	  fscanf(file,"%d\n",&RecLen);
	  Params[b].RecordLength = RecLen;
	  printf("[RecordLength]\n%d\n",RecLen);

	  if(strcmp(fgets(str,255,file),"[ChannelMask]\n")!=0) 
	    {
	      printf("Read [ChannelMask] ERROR! <DPP_Parameters.txt>\n"); 
	      return -1;
	    }
	  ChaMas=0;
	  fscanf(file,"\n%s\n",&chamas);//
	  // printf ("%s\n",chamas);
	  for(i=0;chamas[i]!='\0';i++)
	    {
	      if(chamas[i]=='1'||chamas[i]=='0')
		{
		  ChaMas<<=1;
		  ChaMas+=chamas[i]-'0';
		}
	      else 
		{
		  printf ("[ChannelMask] input error \n");
		}
	    }  
	  Params[b].ChannelMask=ChaMas;
	  // printf("[ChannelMask]\n%d\n",ChaMas);
	  printf ("[ChannelMask]\n%s\n",chamas);

	  // for [TriggerHoldOff] tag
	  if(strcmp(fgets(str,255,file),"[TriggerHoldOff]\n")!=0) 
	    {
	      printf("Read [TriggerHoldOff] ERROR! <DPP_Parameters.txt>\n"); 
	      return -1;
	    }
	  fscanf(file,"%d\n",&TriggerHoldOff);
	  DPPParams[b].trgho = TriggerHoldOff;
	  printf("[TriggerHoldOff]\n%d\n",TriggerHoldOff);
	  
	  // for [COMMON] tag
	  fgets(str,255,file); // Parameter names
	  if(strcmp(fgets(str,255,file),"[COMMON]\n")!=0) 
	    {
	      printf("Read [COMMEN] ERROR! <DPP_Parameters.txt>\n"); 
	      return -1;
	    }
	  printf("%s", str);
	  fscanf(file,"%s %s",All,Pol);
	  if(strcmp(All,"ALL")!=0) 
	    {
	      printf("Read [ALL] ERROR! <DPP_Parameters.txt>\n"); 
	      return -1;
	    }
	  for(i=0; i<PARAMS_NUM; i++) 
	    fscanf(file,"%f",&Params_Data[0][i]);
	  printf("%s %s %f %f %f ...\n", All, Pol, Params_Data[0][0], Params_Data[0][1], Params_Data[0][2]);
	  
	  for(ch=0; ch<MaxNChannels; ch++) 
	    {
	      // Pulse Polarity (CAEN_DGTZ_PulsePolarityPositive or CAEN_DGTZ_PulsePolarityNegative)
	      if(strcmp(Pol,"POS")==0)
		Params[b].PulsePolarity[ch] = CAEN_DGTZ_PulsePolarityPositive;
	      else if(strcmp(Pol,"NEG")==0)
		Params[b].PulsePolarity[ch] = CAEN_DGTZ_PulsePolarityNegative;
	      else
		{
		  printf("Read [POL] ERROR! <DPP_Parameters.txt>\n"); 
		  return -1;
		}
	      // set all channels
	      for(i=0; i<PARAMS_NUM; i++) 
		{
		  if(ch!=0)
		    {
		      Params_Data[ch][i] = Params_Data[0][i];
		    }
		}
	    }

	  // for [INDIVIDUAL] tag
	  fgets(str,255,file); // for "\n"
	  if(strcmp(fgets(str,255,file),"[INDIVIDUAL]\n")!=0) 
	    {
	      printf("Read [INDIVIDUAL] ERROR! <DPP_Parameters.txt> \n"); 
	      return -1;
	    }
	  printf("%s", str);
	  for(ch=0; ch<MaxNChannels; ch++) 
	    { 
	      // individual loop
	      file_pos1=ftell(file);
	      fgets(str,255,file); // for individual parameters
	      if(strlen(str)<50) break;
	      file_pos2=ftell(file);
	      fseek(file,file_pos1-file_pos2,SEEK_CUR);
	      fscanf(file,"%d %s",&ChNo,Pol);
	      if(ChNo<0 || ChNo>=MaxNChannels) {
		fseek(file,file_pos2,SEEK_SET);
		continue;
	      }
	      // Pulse Polarity (CAEN_DGTZ_PulsePolarityPositive or CAEN_DGTZ_PulsePolarityNegative)
	      if(strcmp(Pol,"POS")==0)
		Params[b].PulsePolarity[ChNo] = CAEN_DGTZ_PulsePolarityPositive;
	      else if(strcmp(Pol,"NEG")==0)
		Params[b].PulsePolarity[ChNo] = CAEN_DGTZ_PulsePolarityNegative;
	      else
		{
		  printf("Read [POL] ERROR! <DPP_Parameters.txt>\n"); 
		  return -1;
		}
	      for(i=0; i<PARAMS_NUM; i++)
		fscanf(file,"%f",&Params_Data[ChNo][i]);
	      fgets(str,255,file); // for "\n"
	      printf("%d %s %f %f %f ...\n", ChNo, Pol, Params_Data[ChNo][0], Params_Data[ChNo][1], Params_Data[ChNo][2]);
	    } // individual loop
	} // board tag loop

      if(Board_Read==0) 
	{
	  printf("Read [Board Tag] ERROR! <DPP_Parameters.txt>\n"); 
	  return -1;
	}


      //GOTO
      // Set corresponding DPP parameters
      for(ch=0; ch<MaxNChannels; ch++) 
	{
	  // DC offset for the input signal to adapt it to digitizer's dynamic range
	  Params[b].ChannelDCOffset[ch] = 0x7FFF+Params_Data[ch][0]/100*0xFFFF;   // 0x7FFF+(XXXX)*0xFFFF
	  printf ("%f\n",Params_Data[ch][0]);
	  /* Params[b].ChannelDCOffset[ch] = Params_Data[ch][0]; */
	  printf ("%f\n\n",Params[b].ChannelDCOffset[ch]);

	  // Pre-Trigger size (in samples)
	  Params[b].DPPPreTriggerSize[ch] = Params_Data[ch][1];
	  
	  // DPP_PSD Parameters
	  DPPParams[b].thr[ch] = Params_Data[ch][2];
	  DPPParams[b].selft[ch] = Params_Data[ch][3]; 
	  DPPParams[b].csens[ch] = Params_Data[ch][4]; 
	  DPPParams[b].sgate[ch] = Params_Data[ch][5]; 
	  DPPParams[b].lgate[ch] = Params_Data[ch][6]; 
	  DPPParams[b].pgate[ch] = Params_Data[ch][7]; 
	  DPPParams[b].tvaw[ch] = Params_Data[ch][8];
	  DPPParams[b].nsbl[ch] = Params_Data[ch][9]; 
	  DPPParams[b].discr[ch] = Params_Data[ch][10];
	  DPPParams[b].cfdf[ch] = Params_Data[ch][11]; 
	  DPPParams[b].cfdd[ch] = Params_Data[ch][12];
	  if(int(Params_Data[ch][13]) == 1)
	    DPPParams[b].trgc[ch] = CAEN_DGTZ_DPP_TriggerConfig_Threshold;
	  else
	    DPPParams[b].trgc[ch] = CAEN_DGTZ_DPP_TriggerConfig_Peak; 
	  
	}

      // Pile-Up rejection Mode
      //    CAEN_DGTZ_DPP_PSD_PUR_DetectOnly -> Only Detect Pile-Up
      //    CAEN_DGTZ_DPP_PSD_PUR_Enabled -> Reject Pile-Up
      // DPPParams[b].purh = CAEN_DGTZ_DPP_PSD_PUR_DetectOnly;
      // dpppsdParams.purh = CAEN_DGTZ_DPP_PSD_PUR_DetectOnly; //NOTE: Ignored on 5730
      // dpppsdParams.purgap = 100;  // Purity Gap   // Pile-up Rejection Gap  //NOTE: Ignored on 5730
      // dpppsdParams.blthr = 3;     // Baseline Threshold  //This parameter is deprecated
      // dpppsdParams.bltmo = 100;   // Baseline Timeout    //This parameter is deprecated
      // dpppsdParams.trgho = 0;     // Trigger HoldOff

      
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


      

      printf("Read parameters for board %02d successfully!\n\n", b);
      rewind(file);
    }
  fclose(file);
  return 0;
}

// Read DGTZ Registers 
// return  0=success; -1=error
int Read_DGTZ_Register_725_730_DPP_PSD_Revision05(int handle,int MaxNChInBoard)
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

  // Channel Status Register
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x102C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel Status Register (0x1%d2C) (channel %d): %s\n",ch,ch,bstr);
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
  // ret |= CAEN_DGTZ_ReadRegister(handle, 0x8118, &Register_Data);
  // Decimal2Binary(Register_Data,bstr);
  // printf("LVDS I/O Data (0x8118): %s\n",bstr); 
  

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

  // Voltage Level Mode Configuration
  // ret |= CAEN_DGTZ_ReadRegister(handle, 0x8138, &Register_Data);
  // Decimal2Binary(Register_Data,bstr);
  // printf("Voltage Level Mode Configuration (0x8138): %s\n",bstr);  

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
  // ret |= CAEN_DGTZ_ReadRegister(handle, 0x81A0, &Register_Data);
  // Decimal2Binary(Register_Data,bstr);
  // printf("Front Panel LVDS I/O New Features (0x81A0): %s\n",bstr);

  // Extended Veto Delay
  // ret |= CAEN_DGTZ_ReadRegister(handle, 0x81C4, &Register_Data);
  // Decimal2Binary(Register_Data,bstr);
  // printf("Extended Veto Delay (0x81C4): %s\n",bstr);
  
  // Readout Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF00, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Readout Control (0xEF00): %s\n",bstr);

  // Readout Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF04, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Readout Status (0xEF04): %s\n",bstr);

  // Board ID
  // ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF08, &Register_Data);
  // Decimal2Binary(Register_Data,bstr);
  // printf("Board ID (0xEF08): %s\n",bstr);

  // MCST Base Address and Control
  // ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF0C, &Register_Data);
  // Decimal2Binary(Register_Data,bstr);
  // printf("MCST Base Address and Control (0xEF0C): %s\n",bstr);

  // Relocation Address
  // ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF10, &Register_Data);
  // Decimal2Binary(Register_Data,bstr);
  // printf("Relocation Address (0xEF10): %s\n",bstr); 

  // Interrupt Status/ID
  // ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF14, &Register_Data);
  // Decimal2Binary(Register_Data,bstr);
  // printf("Interrupt Status/ID (0xEF14): %s\n",bstr); 
  
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

  // Configuration ROM C Code
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF01C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM C Code (0xF01C): %s\n",bstr); 
  
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
      printf("Error:  Read_DGTZ_Register_725_730_DPP_PSD_Revision05 !!!\n");
    }

  if(ret) {
    return -1;
  }
  return 0;
}






// Read DGTZ Info
// return  0=success; -1=error
int Read_DGTZ_Info(int handle)
{
  // typedef struct {
  //     char                        ModelName[12];
  //     uint32_t                    Model;
  //     uint32_t                    Channels;
  //     uint32_t                    FormFactor;
  //     uint32_t                    FamilyCode;
  //     char                        ROC_FirmwareRel[20];
  //     char                        AMC_FirmwareRel[40];
  //     uint32_t                    SerialNumber;
  //     uint32_t                    PCB_Revision;
  //     uint32_t                    ADC_NBits;
  //     uint32_t                    SAMCorrectionDataLoaded;        //used only for x743 boards
  //     int                         CommHandle;
  //     char                        License[MAX_LICENSE_LENGTH];
  // } CAEN_DGTZ_BoardInfo_t;

  int ret;// CAEN_DGTZ_ErrorCode ret;
  CAEN_DGTZ_BoardInfo_t BoardInfo;
  ret = CAEN_DGTZ_GetInfo(handle, &BoardInfo);
  if(ret)
    {
      printf("Get CAEN DGTZ Info error.\n");
      return -1;
    }
  printf("ModelName:%s\n",BoardInfo.ModelName);
  printf("Model:%u\n",BoardInfo.Model);
  printf("Channels:%u\n",BoardInfo.Channels);
  printf("FormFactor:%u\n",BoardInfo.FormFactor);
  printf("FamilyCode:%u\n",BoardInfo.FamilyCode);
  printf("ROC_FirmwareRel:%s\n",BoardInfo.ROC_FirmwareRel);
  printf("AMC_FirmwareRel:%s\n",BoardInfo.AMC_FirmwareRel);
  printf("SerialNumber:%u\n",BoardInfo.SerialNumber);
  printf("PCB_Revision:%u\n",BoardInfo.PCB_Revision);
  printf("ADC_NBits:%u\n",BoardInfo.ADC_NBits);
  printf("SAMCorrectionDataLoaded:%u\n",BoardInfo.SAMCorrectionDataLoaded);
  printf("CommHandle:%d\n",BoardInfo.CommHandle);
  printf("License:%s\n",BoardInfo.License);

  return 0;
}


// 
// DigitizerConfigure.cc ends here
