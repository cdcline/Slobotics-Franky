#include "37e12reqs.h"
#include "pwmControl.h"

void pwmControl::setPWM(int lineNumber, int dutyCycle) {
   int fd;
   Cop_Data *Cop = CopDeviceCreate(0);
   
   OPENDEV(fd,robotConstants::devPath);
   E12_PWMControl(fd, lineNumber, pwmConstants::FREQUENCY, dutyCycle, Cop);

   close(fd);
   CopDeviceDestroy(Cop);
}
