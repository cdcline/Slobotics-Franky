SDKBASE=../gcc_emac/
#CROSS=$(SDKBASE)gcc-3.3.6-SIB/bin/
CROSS=$(SDKBASE)bin/i486-linux-

CC = $(CROSS)c++
CC2 = $(CROSS)gcc 

DLIBS=-lpthread -lmodbus -lstdc++

OFLAGS = -Wall -O -Wno-deprecated -le12packet
CPLUSFLAGS = -Wall -O -Wno-deprecated
RCFLAGS = -Wall -O -Wno-deprecated -le12packet
CFLAGS = -Wall -O -le12packet

INCLUDES =$(SDKBASE)include
LIBRARIES =$(SDKBASE)lib
COPCODE = -I$(INCLUDES) -L$(LIBRARIES)

all: robotCommand

robotCommand: robotCommand.cpp Robot.o constants.o ioControl.o Coprocessor.o
	$(CC) $(COPCODE) -o $@ $(RCFLAGS) $^

Robot.o: Robot.cpp
	$(CC) $(COPCODE) -c -o $@ $(CPLUSFLAGS) $^

ioControl.o : ioControl.cpp
	$(CC) $(COPCODE) -c -o $@ $(CPLUSFLAGS) $^

pwmControl.o: pwmControl.cpp
	$(CC) $(COPCODE) -c -o $@ $(CPLUSFLAGS) $^

constants.o: constants.cpp
	$(CC) $(COPCODE) -c -o $@ $(CPLUSFLAGS) $^

Coprocessor.o: Coprocessor.c
	$(CC2) $(COPCODE) -c -o $@ $(CFLAGS) $^

myPWMControl: myPWMcontrol.c Coprocessor.o
	$(CC) $(COPCODE) -o $@ $(CFLAGS) $^

clean:
	$(RM) *.o robotCommand test myPWMcontrol
	$(RM) *~

