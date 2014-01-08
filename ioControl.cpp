#include "37e12reqs.h"
#include "ioControl.h"
//Optical ISO Outs
void optoIsoControl::setOutput(isodata data) 
{
   int fd;
   OPENDEV(fd,robotConstants::devPath);
   ioctl(fd, ISO_DIG_OUT, &data);
   close(fd);
}

void optoIsoControl::readInput(isodata * data) 
{
   int fd;
   OPENDEV(fd,robotConstants::devPath);
   ioctl(fd, ISO_DIG_IN, data);
   close(fd);
}

//For the 8-bit mask: 1 = output, 0 = input
void gpioControl::configIO(int port, unsigned char mask)
{
   int fd;

   OPENDEV(fd, robotConstants::devPath); 
   if((port > 1) || (port < 0))
   {
      printf("invalid port");
   }
   E12_Digital_Cfg(fd, mask, port);
   close(fd);
}

void gpioControl::writePort(int port, unsigned char data) 
{
   int fd;

   OPENDEV(fd, robotConstants::devPath); 
   if((port > 1) || (port < 0))
   {
      printf("invalid port");
   }

   E12_Digitalout(fd, data, port);
   close(fd);
}

void gpioControl::readPort(int port, unsigned char data) 
{
   int fd = 0, rxnum;
   E12_Digital_Req(fd);
   __u8 buffer[100];
   __u32 typemask = 0;
   Cop_Data *Cop = CopDeviceCreate(0);

   while(!(typemask&DIGRESPONSE))
   {
      rxnum = read(fd, buffer, sizeof(buffer));
      typemask = E12_Packet_Route(Cop, buffer, rxnum);
   }
   
   data = Cop->Digital[port];
   CopDeviceDestroy(Cop);
}


//PWM
void pwmControl::setPWM(int lineNumber, int dutyCycle) 
{
   int fd;
   Cop_Data *Cop = CopDeviceCreate(0);
   
   OPENDEV(fd,robotConstants::devPath);
   E12_PWMControl(fd, lineNumber, gpioConstants::FREQUENCY, dutyCycle, Cop);

   close(fd);
   CopDeviceDestroy(Cop);
}
