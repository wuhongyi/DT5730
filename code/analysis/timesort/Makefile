### Makefile --- 
## 
## Description: 
## Author: Hongyi Wu(吴鸿毅)
## Email: wuhongyi@qq.com 
## Created: Fri Apr 29 21:53:58 2016 (+0800)
## Last-Updated: 一 1月 14 20:25:35 2019 (+0800)
##           By: Hongyi Wu(吴鸿毅)
##     Update #: 5
## URL: http://wuhongyi.cn 

OBJ = timesort 
sourcefile = main.cc timesort.cc  timesort.hh


ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs)

CFLAGS = -Wall -g -I$(TARTSYS)/include -I$(ROOTSYS)/include -L$(TARTSYS)/lib

GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cc ,$(sourcefile))

clean:
	rm -f *~ *.o $(OBJ)

######################################################################
### Makefile ends here
