#include "37e12reqs.h"
#include "isoOutControl.h"

void isoOutControl::setIsoOuts(isodata data) {
   int fd;
   OPENDEV(fd,robotConstants::devPath);
   ioctl(fd, ISO_DIG_OUT, &data);
   close(fd);
}
