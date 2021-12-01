# Common build variables.

VER			 = 
OFFSET		 = 
CC           = g++ 
LD           = ld
LDSO	     = $(CC) -shared -nostartfiles
CFLAGS		 = -fPIC -shared $(OFFSET) -O2
LINKFLAGS    = -lpthread -lz
MKDIR        = mkdir -p
RM           = rm -f
CP           = cp -fp
ECHO         = echo
FIND         = find
MV           = mv -f
LN           = ln -s
AR           = ar -r
ARFLAGS      = rv
DEBUG        = 

ARCH = $(shell getconf LONG_BIT)
ifeq (64,$(ARCH))
LINKFLAGS = ../lib/linux/x64/libgm3000.1.0.so
MAIN    = interruptTest64
else    
LINKFLAGS = ../lib/linux/x86/libgm3000.1.0.so
MAIN    = interruptTest32
endif


OBJSMAIN    = main.o 
#MAIN    = interruptTest

# Complie all
all: $(MAIN)
$(MAIN): $(OBJSMAIN)
	$(RM) $(MAIN)
	$(CC) -o $(MAIN) $(OBJSMAIN) $(LINKFLAGS)

clean:
	$(RM) $(MAIN)
	$(RM) *.o $(MAIN)

# Complie the source code 

%.o: %.c
	$(CC) -c -o $@ $? 
%.o: %.cpp
	$(CC) -c -o $@ $? 
