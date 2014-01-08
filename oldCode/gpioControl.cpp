#include "37e12reqs.h"

#define DIGRESPONSE BITMASK(PRTIN0)

class gpioControl
{
   public:
      static void output(int pin);
};

void gpioControl::output(int pin) {
   int fd, rxnum;
   __u8 mask;
   Cop_Data *Cop = CopDeviceCreate(0);
   __u32 typemask = 0;
   __u8 buffer[100];
   int port;

   OPENDEV(fd, robotConstants::devPath);

   mask = strtoul(fd, pwmConstants::OUTPUT, port);
   E12_Digital_Cfg(fd, mask, port);
   E12_Digital_Req(fd);
  




/** 
   while(!(typemask&DIGRESPONSE)) 
   {
      rxnum = read(fd, buffer, sizeof(buffer));
      typemask = E12_Packet_Route(Cop, buffer, rxxnum);
   }
*/
