// DigitizerAdmin.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 4月 24 16:31:25 2016 (+0800)
// Last-Updated: 六 4月 30 13:07:58 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 39
// URL: http://wuhongyi.cn 

#ifndef _DIGITIZERADMIN_H_
#define _DIGITIZERADMIN_H_

#include "DigitizerGlobals.hh"

#include <sys/time.h> // struct timeval, select()
#include <termios.h> // tcgetattr(), tcsetattr()
#include <unistd.h> // read()
#include <inttypes.h>
#include <stdint.h>   // C99 compliant compilers: uint64_t
#include <ctype.h>    // toupper()
#define CLEARSCR "clear"

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
//using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// READ PAR
template <class T>
T ReadValue(std::string vname, std::string fname, bool verb=false)
{
  std::ifstream in(fname.c_str(), std::ios::in);
  if(!in.is_open()){
    std::cout<<":-( failed to open the file:"<<fname<<std::endl;
    exit(0);
  }
  T v;
  std::string inname;
  std::string cbf;
  char chd;
  while(true){
    chd=in.peek();    
    if(chd==EOF){//EOF(generally -1) is a micro in <cstdio>
      std::cout<<":-($"<<vname <<"$ not found@"<<fname<<std::endl;
      in.close();
      exit(0);
    }else if(chd=='#' || chd==0x20 || chd==0xA){
      getline(in,cbf);
    }else{
      in>>inname;
      if(inname==vname) { 
	in>>v;
	if(verb) std::cout<<vname<<" = "<<v<<std::endl; 
	in.close();     
	break;
      }
    }     
  }  
  return v;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// SLEEP
void Sleep(int t);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// GETCH
int getch(void);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// KBHIT
int kbhit();

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Get time in milliseconds     return  time in msec 
long get_time();

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Print the interface to screen
void PrintInterface();


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 十进制转二进制
// d 十进制数
// bstr 二进制字符串
// return 出错返回-1，成功返回字符串长度
int Decimal2Binary(uint32_t d, char *bstr);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// execute commands like in a terminal
void DoInTerminal(char *terminal);


//判断文件是否存在
bool IsFileExists(const char *name);

// Create File Directory
// return: true:success 
bool CreateDirectory(const char *path);

void SaveLogFile(char *time,char *file);

char* GetTimeStringYmdHMS();

int ReadAndUpdateRunNumber(std::string file);

// return byte
double GetFileSize(char *name);


#endif /* _DIGITIZERADMIN_H_ */

// 
// DigitizerAdmin.hh ends here
