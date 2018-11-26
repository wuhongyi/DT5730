### Makefile --- 
## 
## Description: 
## Author: Hongyi Wu(吴鸿毅)
## Email: wuhongyi@qq.com 
## Created: Fri Apr 29 21:53:58 2016 (+0800)
## Last-Updated: 三 5月  4 09:39:37 2016 (+0800)
##           By: Hongyi Wu(吴鸿毅)
##     Update #: 4
## URL: http://wuhongyi.cn 

OBJ = raw2root 
sourcefile = main.cc


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
