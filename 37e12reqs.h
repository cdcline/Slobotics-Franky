#ifndef COM_REQ
#define COM_REQ

#define OPENDEV(filed,dev){if((filed = open(dev, O_RDWR))<=0){printf("couldn't open %s\n",dev);}}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <asm/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sched.h>
#include <sys/resource.h>
#include <sys/io.h>
extern "C" {
#include <E12user.h>
#include <packet.h>
#include <coprocessor.h>
}

#include "constants.h"
#endif
