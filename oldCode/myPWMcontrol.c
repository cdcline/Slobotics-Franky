/*! @defgroup PWMcontrol PWMcontrol - PWMcontrol.c
  This module is used to build the PWMcontrol application.
  PWMcontrol is used to configure the internal digital ports of the HCS12 on the pcm37e12 as PWMs.
  Not all frequency/duty cycles are possible.
  @see E12_PWMControl
 */

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
#include <E12user.h>
#include <packet.h>
#include <coprocessor.h>



#define ARGCHECK(num1,num2){if((argc!=num1)&&(argc!=num2)){printf("invalid number of arguements\n");help();return -1;}}

#define OPENDEV(filed,dev){if((filed = open(dev, O_RDWR))<=0){printf("couldn't open %s\n",dev);return -1;}}

static int help(void)
{
const char *help_table = "***********************************************PWMcontrol                                  **Nathan Z. Gustavson                         **Emac.inc                                    * *"__DATE__"                                 *   *usage:                                      *   *PWMcontrol device pwm freq duty             **ex:                                         **version /dev/37e12 0 1000 50                **Sets pwm 0 to 1000kHz at 50% duty cycle     **Or                                          **PWMcontrol device 0xff                      **to display current PWM configurations       ***********************************************\n";
printf("%s",help_table);
return 0;
}

static int displayPWMs(Cop_Data *Cop)
{
int pwm;
  printf("pwm\tfreq\tduty\t\n");
for(pwm=0;pwm<PWMNUM;pwm++)
  printf("%u\t%u\t%u\t\n",Cop->PWM_config[pwm].pwm,Cop->PWM_config[pwm].frequency,Cop->PWM_config[pwm].duty);

return 0;
}

int main(int argc, char **argv)
{
int fd;
int pwm,freq,duty;

	while(1) {
		Cop_Data *Cop = CopDeviceCreate(0);

		ARGCHECK(5,3);
		OPENDEV(fd,argv[1]);

		pwm = strtoul(argv[2],NULL,0);

		if(pwm==0xff)
		  {
		    UpdatetHardwarePWMInfo(fd, Cop);//read current PWM configuration from the hardware and copy it into Cop
		    displayPWMs(Cop);//display PWM configuration
		  }
		else
		  {//set PWM configuration
		    freq = strtoul(argv[3],NULL,0);
		    duty = strtoul(argv[4],NULL,0);
		    E12_PWMControl(fd,pwm, freq, duty, Cop);
		  }

		close(fd);
		CopDeviceDestroy(Cop);
	}

return 0;
}


