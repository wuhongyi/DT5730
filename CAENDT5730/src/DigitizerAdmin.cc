// DigitizerAdmin.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 4月 24 16:31:58 2016 (+0800)
// Last-Updated: 六 4月 30 13:07:58 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 48
// URL: http://wuhongyi.cn 

#include "DigitizerAdmin.hh"

#include <ctime>
#include <fcntl.h>
#include <errno.h>//define error

#include <sys/stat.h>//stat(const char *file_name,struct stat *buf)
// #include <sys/types.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

static struct termios g_old_kbd_mode;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
static void cooked(void)
{
  tcsetattr(0, TCSANOW, &g_old_kbd_mode);
}

static void raw(void)
{
  static char init;
  struct termios new_kbd_mode;

  if(init)
    return;
  /* put keyboard (stdin, actually) in raw, unbuffered mode */
  tcgetattr(0, &g_old_kbd_mode);
  memcpy(&new_kbd_mode, &g_old_kbd_mode, sizeof(struct termios));
  new_kbd_mode.c_lflag &= ~(ICANON | ECHO);
  new_kbd_mode.c_cc[VTIME] = 0;
  new_kbd_mode.c_cc[VMIN] = 1;
  tcsetattr(0, TCSANOW, &new_kbd_mode);
  /* when we exit, go back to normal, "cooked" mode */
  atexit(cooked);

  init = 1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Sleep(int t) 
{
  usleep( t*1000 );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int getch(void)
{
  unsigned char temp;

  raw();
  /* stdin = fd 0 */
  if(read(0, &temp, 1) != 1)
    return 0;
  return temp;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int kbhit()
{
  struct timeval timeout;
  fd_set read_handles;
  int status;

  raw();
  /* check stdin (fd 0) for activity */
  FD_ZERO(&read_handles);
  FD_SET(0, &read_handles);
  timeout.tv_sec = timeout.tv_usec = 0;
  status = select(0 + 1, &read_handles, NULL, NULL, &timeout);
  if(status < 0)
    {
      printf("select() failed in kbhit()\n");
      exit(1);
    }
  return (status);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

long get_time()
{
  long time_ms;
  struct timeval t1;
  struct timezone tz;
  gettimeofday(&t1, &tz);
  time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
  return time_ms;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrintInterface() 
{
  printf("\n  [q]   Quit\n");
  printf("  [s]   Start/Stop acquisition\n");
  printf("  [t]   Send a software trigger\n");
  printf("  [w]   Enable/Disable continuous writing to output file\n");
  printf("  [R]   Reload board parameters file and restart\n");
  printf("  [p]   Enable/Disable  plot mode\n");
  printf("  [0]   Plot recently single on plot mode \n");
  printf("  [2/8] Minus/Plus one channel on plot mode\n");
  printf("  [4/6] Minus/Plus one board on plot mode\n");
  printf("--------------------------------------------------------------------------\n");



  // printf("p ) Save once ROOT picture\n");
  // printf("  [T]   Enable/Disable continuous software trigger\n");
  // printf("  [W]   Write one event to output file\n");
  // printf("  [p]   Plot one event\n");
  // printf("  [P]   Enable/Disable continuous plot\n");
  // printf("  [f]   Toggle between FFT and Waveform plot\n");
  // printf("  [h]   Toggle between Histogram and Waveform plot\n");
  // printf(" [0-7]  Enable/Disable one channel on the plot\n");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Decimal2Binary(uint32_t d, char *bstr)
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DoInTerminal(char *terminal)
{
  system(terminal);
}

bool IsFileExists(const char *name)
{
  if((access(name,F_OK))!=-1)  
    {  
      return true;
    }  
  else  
    {  
      return false;
    } 
}

bool CreateDirectory(const char *path)
{
  if(mkdir(path,0777)==0)//the first 0 stand of octal formats
    {
      printf("created the directory %s.\n",path);
      return true;
    }
  else
    {   	  
      printf("cant't creat the directory %s.\n",path);
      printf("errno：%d\n",errno);
      printf("ERR  ：%s\n",strerror(errno));	
      return false;
    }
}

void SaveLogFile(char *time,char *file)
{
  char timestringfile[64];
  char filelog[64];
  if(!IsFileExists("../Log"))
    CreateDirectory("../Log");
  sprintf(timestringfile,"../Log/%s",time);
  if(!IsFileExists(timestringfile))
    CreateDirectory(timestringfile);
  sprintf(filelog,"cp %s  %s",file,timestringfile);
  DoInTerminal(filelog);
}

char* GetTimeStringYmdHMS()
{
  time_t lt = time(NULL);
  tm* current = localtime( &lt );
  char str[100];
  strftime( str , 100 , "%Y%m%d%H%M%S", current);
  return str;
}

int ReadAndUpdateRunNumber(std::string file)
{
  std::ifstream RunNo;
  int RunNumber;
  RunNo.open(file.c_str());
  if(RunNo.fail()) //检查上述操作是否正常
    {
      std::cout<<"Can't open file: "<<file<<std::endl;
      return -1;
    }
  else
    {
      RunNo>>RunNumber;
      std::ofstream WRunNo;
      WRunNo.open(file.c_str());
      WRunNo<<RunNumber+1;
      WRunNo.close();
    }
  RunNo.close();
  return RunNumber;
}

double GetFileSize(char *name)
{
  struct stat statbuff;  
  if(stat(name, &statbuff) < 0){  
    printf("errno：%d\n",errno);
    printf("ERR  ：%s\n",strerror(errno));
    return 0;  
  }else{  
    return statbuff.st_size;//
  }
}



// 
// DigitizerAdmin.cc ends here





