ANAME=RecorderClient_broadcast 
C++ = arm-hisiv400-linux-g++
CC = arm-hisiv400-linux-gcc
TMP_PROGS = $(wildcard *.cpp)
PROGS = $(TMP_PROGS)  
OBJS = $(PROGS:.cpp=.o)  
INCDIR=./ 
  
all: $(ANAME)  
  
$(ANAME): $(OBJS)  
	@echo "--------------- .o to ELT "  
	$(C++) -DNDEBUG $(TMP_PROGS) -o $@ -lpthread   
.cpp.o:  
	@echo "--------------- CPP to .o "  
	$(C++) -DNDEBUG $(CFLAGS) -I$(INCDIR) -c  $< -o $@  -lpthread   
  
clean:  
	$(RM) $(ANAME)  
	$(RM) $(OBJS)
