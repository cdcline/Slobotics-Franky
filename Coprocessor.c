/*Coprocessor.c
Coprocessor Device interface functions
and tasklet parsing calls.
Nathan Z. Gustavson
emac.inc
7/22/03
see coprocessor.h
 */
/*!@defgroup COP Coprocessor Module
*/
/*@{*/
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
#include <signal.h>
#include <packet.h>
#include <coprocessor.h>
#include <counter.h>

typedef struct Cop_DataBlock
{
__u8 *data;
int size;
}Cop_DataBlock;


/*!Interprete packet data and store it in  a Cop_Data structure
This function is the core of the interface between the low level packet parsing functions and the user space programs.
It determines what happens to the data after it has been interpreted. In this case it passes it to a Cop_DataBlock structure element and
sets the appropriate flags within it.
@param data pointer to the Cop_Data structure to store data in. This is passed to CopParsePacket by E12_Packet_Route->CopGetPacket once the packet has
been determined to be complete.
*/
int CopParsePacket(Cop_Data *data);

/*!Pull data from an input block and check if it completes a packet based on parameters discoved by CopPacketMask and CopPacketStart.
If it does, pass the data on through the Cop_Data structure into CopParsePacket
@param data  data pointer to the Cop_Data structure to store data in. Typically received from  E12_Packet_Route.
@param block a block of data received from the driver, this is typically passed from E12_Packet_Route.
*/
int CopGetPacket(Cop_Data *data, Cop_DataBlock *block);

/*!Pull a chunk of data from a block and interprete its parameters to find data length, and masking. This is used by the lower level
packet functions and could have unpredicatable results if by the applications developer.
@param data  data pointer to the Cop_Data structure to store data in. Typically received from  E12_Packet_Route.
@param block a block of data received from the driver, this is typically passed from E12_Packet_Route.
*/
int CopPacketMask(Cop_Data *data, Cop_DataBlock *block);

/*!Pull a chunk of data from a block and interprete its parameters to find the type and masking. This is used by the lower level
packet functions and could have unpredicatable results if by the applications developer.
@param data  data pointer to the Cop_Data structure to store data in. Typically received from  E12_Packet_Route.
@param block a block of data received from the driver, this is typically passed from E12_Packet_Route.
*/
int CopPacketStart(Cop_Data *data, Cop_DataBlock *block);


/*!called when a packet is marked as invalid to reset a Cop_Data structures packet counts
@param data  data pointer to the Cop_Data structure to store data in. Typically received from  E12_Packet_Route.
*/
void CopPackBreak(Cop_Data *data);


static int BlockPull(Cop_DataBlock *block,__u8 *buffer, int count)
{
int tcount = 0;

while((block->size)&&(count))
{
*buffer = *block->data; 
GENDEBUG("pulled %c from block\n",*buffer);
++block->data;
++buffer;

block->size--;
tcount++;
count--;
}


return tcount;
}


int CopPacketStart(Cop_Data *data, Cop_DataBlock *block)
{
GENDEBUG("Parsing for a Header\r\n");

while(1)
   	{
	  
	if(!(BlockPull(block,(__u8*) &data->packet[DELIMITER],1)))
		break;

	  
	  if((DLMIN<=data->packet[DELIMITER])&&(data->packet[DELIMITER]<=DLMAX))
  	    {

	     data->psize = 1;
	     return SUCCESS;
  	    }

	  if(data->packetdata.extended==TRUE)
	    {
	     data->psize = 1;
	     data->packet[DELIMITER] = (char)EXT_TRANS(data->packet[DELIMITER]);   
	     return SUCCESS;

	    }		

	
	}

return FAILURE;
}

int CopPacketMask(Cop_Data *data, Cop_DataBlock *block)
{
GENDEBUG("Parsing for packet mask\n");
/*use pull block because you can distinguish pulled NULL from pulled nothing*/

if(BlockPull(block,(__u8*)&data->packet[MASKNIB],1))
  {
    if((DLMIN<=data->packet[MASKNIB])&&(data->packet[MASKNIB]<=DLMAX))
      {/*new header received, restart.*/
	CopPackBreak(data);
	return FAILURE;
      }

    
    data->psize = 2;
    PcpkParseHeader(&data->packet[0],&data->packetdata);
   
    if(data->packetdata.extended==TRUE)
	{//extended packet protocol
	  data->psize=0;/*reset for extended header*/
	  data->ptogo=0;
	}
    else
    	data->ptogo = 2*data->packetdata.datanum*data->packetdata.datasize;
    

    return SUCCESS;
  }

return FAILURE;
}


/*called if header received in the middle of a packet*/
void CopPackBreak(Cop_Data *data)
{
GENDEBUG("New Header received, restart packet\n");
data->packet[DELIMITER] = data->packet[MASKNIB];
data->psize = 1;
return;
}





int CopGetPacket(Cop_Data *data, Cop_DataBlock *block)
{
int retval = 0;
GENDEBUG("getting packet\n");

while(data->ptogo>0)//counting bug, fixed on 8/22/02 NZG
  {
    
	if(BlockPull(block,(__u8*)&data->packet[data->psize],1))
	{
	//data->packet[data->psize] = FifoPull(fifo);
	if((DLMIN<=data->packet[data->psize])&&(data->packet[data->psize]<=DLMAX))
	  {
	    data->packet[MASKNIB] = data->packet[data->psize];
	    CopPackBreak(data);
	    return FAILURE;
	  }
	
	data->ptogo--;
	data->psize++;
      }
    else
      break;

  }


GENDEBUG("ptogo is %u at end of get packet\n",data->ptogo);

if(data->ptogo==0)
     retval = CopParsePacket(data);


return retval;
}



/*
CopParsePacket
hardware specific packet parsing function.
interpretes the data from the packets and 
performs actions based upon it.
*/

int CopParsePacket(Cop_Data *data)
{
void *datapntr = data->packetdata.data;
RG_ParseMask *parsemask = &data->parsemask;
unsigned long long cross_stamp;
int channel;

GENDEBUG("Parsing Packet data\n");
PcpkParseData(&data->packet[0],&data->packetdata);

GENDEBUG("data parsed assigning data\n");
switch(data->packetdata.type)
  {
 case EXTNULCMD:
    GENDEBUG("null command no data assigned\n");
    break;
 
  
  case VERSIONINFO:
    GENDEBUG("version info returned\n");
    data->version.version = ((__u8 *)datapntr)[0];
    strcpy(data->version.date,((const char*)datapntr)+1);
  break;
   
    
   /*analog to digital data*/
  case ADBLK0:
    GENDEBUG("Internal AD channel %x returned\n",(unsigned int)data->packetdata.maskcnt);    
    data->Internal_AD[data->packetdata.maskcnt] = *(__u16 *)datapntr;
    parsemask->Internal_AD|=BITMASK(data->packetdata.maskcnt);
    break;
  

  case ADBLK1:
    GENDEBUG("External Analog to digital channel, mask = %x, data =%x\n",data->packetdata.maskcnt,*(__u16 *)datapntr);
    data->External_AD[data->packetdata.maskcnt] = *(__u16 *)datapntr; 
    parsemask->External_AD|=BITMASK(data->packetdata.maskcnt);
    break;
    /*Digital IO data*/

  
  case PRTIN0:
    GENDEBUG("Digital Port Data\n"); 
    if(data->packetdata.maskcnt&0x08)
      data->Digital[0] = *((__u8 *)datapntr);
    if(data->packetdata.maskcnt&0x04)
      data->Digital[1] = ((__u8 *)datapntr)[1];
      break;

  

    /*CAN Message*/
  case CAN:
    GENDEBUG("Can Data\n");   
    if(data->CAN_message);
      data->CAN_message = (E12Can_Message*)datapntr;
    break;
    

    /*Keypad Data*/
  case KEYPAD:
    GENDEBUG("Keypad Data\n");
      data->Key = *(char *)datapntr;
    break;

  case SER422:
    GENDEBUG("422 Data\n");						       
	((__u8 *)datapntr)[data->packetdata.maskcnt] = 0;
	
	if((strlen(data->RS422_string)+data->packetdata.maskcnt)>RS422BUFSIZE)
	  break;

	strcat(data->RS422_string,(const char*)datapntr);
    break;

  case SER232:
    GENDEBUG("232 Data\n");   
    ((__u8 *)datapntr)[data->packetdata.maskcnt] = 0;

    if((strlen(data->RS232_string)+data->packetdata.maskcnt)>RS232BUFSIZE)
      break;
    
    strcat(data->RS232_string,(const char*)datapntr);
    break;
    
  case COUNTER:
        GENDEBUG("Counter data\n");
	if(data->packetdata.maskcnt>7)
	  break;

	data->counter_data[data->packetdata.maskcnt].counter = ((__u16 *)(datapntr))[0];
	parsemask->counter|=BITMASK(data->packetdata.maskcnt);
	break;

  case COUNTERD://configuration data returned, update the shadow with hardware
  channel =  ((__u16 *)(datapntr))[0];
  data->counter_config[channel].trigger =  ((__u16 *)(datapntr))[1];
  data->counter_config[channel].threshold =  ((__u16 *)(datapntr))[2];
  data->counter_config[channel].flags =  ((__u16 *)(datapntr))[3];
  parsemask->counter|=BITMASK(channel);
  break;


case PWMCON://configuration data returned, update the shadow with hardware
  channel =  ((__u16 *)(datapntr))[0];
  data->PWM_config[channel].pwm =  channel;
  data->PWM_config[channel].frequency =  ((__u16 *)(datapntr))[1];
  data->PWM_config[channel].duty =  ((__u16 *)(datapntr))[2];
  data->PWM_config[channel].delay =  ((__u16 *)(datapntr))[3];
  parsemask->pwm|=BITMASK(channel);
  break;




  case ICNOTIFY:
//update count_data variables to reflect timestamp passed by ICNOTIFY
    channel =  ((__u16 *)(datapntr))[0];

    GENDEBUG("counter zero detection, channel %x\n",channel);

    //data->counter_data[channel].countstart = ((__u16 *)(datapntr))[1];

    cross_stamp = ((__u16 *)(datapntr))[2];
    cross_stamp <<=16;
    cross_stamp += ((__u16 *)(datapntr))[3];

    if(channel<COUNTNUM)
        {
        data->counter_data[channel].zerocross++; //this was a zerocrossing
        data->counter_data[channel].laststart = data->counter_data[channel].countstart; //save the last start
        //data->counter_data[channel].countstart = ((__u16 *)(datapntr))[1]; //this is typically zero if it's a zerocross


          if(!(data->counter_config[channel].flags&AUTOSTOP))//if counter didn't stop then countstart has changed
            {
             if(data->counter_config[channel].flags&AUTOLOAD)//autoreload mean counter starts at threshold
                data->counter_data[channel].countstart=data->counter_config[channel].threshold;
             else
                data->counter_data[channel].countstart=0xffff;//wraparound
            }

        }
    else
        {
        channel-=COUNTNUM;//this was a counter start
        data->counter_data[channel].zerocross = 0;
        data->counter_data[channel].countstart = ((__u16 *)(datapntr))[1];//get the start value
        }
    data->counter_data[channel].laststamp =  data->counter_data[channel].timestamp;
    data->counter_data[channel].timestamp = cross_stamp;
    parsemask->zero|=BITMASK(channel);
    break;
	
  }//end of switch(type)

data->psize=0;/*reset and get ready for the next packet*/
data->ptogo=0;

return(data->packetdata.type);
}

static void CopInit(Cop_Data *CopData)
{
int num;
CopData->psize=0;
CopData->ptogo=0;
CopData->Datarxd=TRUE;
CopData->packetdata.extended=FALSE;
CopData->packetdata.mult = 0;
CopData->RS232_string[0] = 0;
CopData->RS422_string[0] = 0;
CopData->CAN_message  = NULL;

for(num=0;num<COUNTNUM;num++)//initialize counter data
  {
  CopData->counter_config[num].counter = num;
  CopData->counter_config[num].trigger = 0;
  CopData->counter_config[num].threshold = 0;
  CopData->counter_config[num].flags =0;
  CopData->counter_data[num].counter =0;
  CopData->counter_data[num].countstart =0;
  CopData->counter_data[num].timestamp =0;
  CopData->counter_data[num].laststamp =0;
  CopData->counter_data[num].zerocross =0;
  }

for(num=0;num<PWMNUM;num++)//initialize PWM data
{
CopData->PWM_config[num].pwm = num;
CopData->PWM_config[num].frequency = 0;
CopData->PWM_config[num].duty = 0;
CopData->PWM_config[num].delay = 0;
}


}



Cop_Data *CopDeviceCreate(int flags)
{
Cop_Data *Cop = (Cop_Data*)malloc(sizeof(Cop_Data)); 
CopInit(Cop);
return(Cop);
}

int CopDeviceDestroy(Cop_Data *Cop)
{
free(Cop);
return 0;
}

static int COPclearmem(Cop_Data *Cop)
{
Cop->RS232_string[0] = 0;
Cop->RS422_string[0] = 0;
return 0;
}

__u32 E12_Packet_Route(Cop_Data *Cop, unsigned char *data, int datasize)
{
Cop_DataBlock datablock;
int type = 0; 
__u32 retval=0;
datablock.size = datasize;
datablock.data = data;

memset(&Cop->parsemask,0,sizeof(RG_ParseMask));/*new parse clear old mask*/


while(datablock.size)
  {
    if(Cop->psize==0)
      CopPacketStart(Cop,&datablock);

    if(Cop->psize==1)
      CopPacketMask(Cop,&datablock);
    
    if(Cop->psize>1)
      type = CopGetPacket(Cop,&datablock);

    retval |= BITMASK(type);//build bitmask of returned types
  }
return retval;
}

/*parse data from fd until one of the flags in bitmask is set.
An alternative to the  E12_Packet_Route function.
*/
__u32 E12_Parse_Until(int fd, Cop_Data *Cop, __u32 bitmask )
{
Cop_DataBlock datablock;
int type = 0;
__u32 typemask=0;
char buffer[1000];
int loopcount=0;

memset(&Cop->parsemask,0,sizeof(RG_ParseMask));/*new parse clear old mask*/

while(!(typemask&bitmask))
{

  datablock.size = read(fd,buffer,sizeof(buffer));//read data
  datablock.data = (__u8*)buffer;

  while(datablock.size)
    {
      if(Cop->psize==0)
        CopPacketStart(Cop,&datablock);

      if(Cop->psize==1)
        CopPacketMask(Cop,&datablock);

      if(Cop->psize>1)
        type = CopGetPacket(Cop,&datablock);

      typemask |= BITMASK(type);//build bitmask of returned types
    }
if(loopcount++==1000)//timeout for non-blocking mode
  return 0;
}
return typemask;
}


int UpdatetHardwareCounterInfo(int fd, Cop_Data *Cop)
{
int counter;
int parseneeded = 0xff;

for(counter=0;counter<COUNTNUM;counter++)
  E12_CountCfg_Req(fd, counter);//request config packets for all the counters

while(parseneeded)
  {
  E12_Parse_Until(fd, Cop, BITMASK(COUNTERD));//parse until a config packet is received.
  parseneeded&=~(Cop->parsemask.counter);//clear bits from parseneeded until all the bits have been cleared.
  }

return 0;
}

int UpdatetHardwarePWMInfo(int fd, Cop_Data *Cop)
{
int pwm;
int parseneeded = 0xffff;

for(pwm=0;pwm<PWMNUM;pwm++)
  E12_PWMCfg_Req(fd, pwm);//request config packets for all the pwms

while(parseneeded)
  {
  E12_Parse_Until(fd, Cop, BITMASK(PWMCON));//parse until a config packet is received.
  parseneeded&=~(Cop->parsemask.pwm);//clear bits from parseneeded until all the bits have been cleared.
  }

return 0;
}


int e12packetout(int fd,__u8 device,__u8 maskcnt,void *data)
{
__u8 *packet = (__u8*)PcpkPackGen(device,maskcnt,data);
write(fd,packet,strlen((const char*)packet));
free(packet);
return 0;
}
	     


int E12_PWMControl(int fd,int pwm, int frequency, int duty,Cop_Data *Cop)
{
PWMconfig *config = &Cop->PWM_config[pwm];
//shadow the variables and provide a config structure to pass to e12packetout
config->pwm = pwm;
config->frequency = frequency;
config->duty  = duty;
config->delay = 0;

if(pwm<COUNTNUM)
  {
    Cop->counter_config[pwm].flags=0;
    Cop->counter_config[pwm].threshold = 0;
    Cop->counter_config[pwm].trigger = 0; //this turns off the counter on that channel, if enabled
  }

e12packetout(fd,PWMCON,0x0F,config);
return 0;
}

int E12_PWMCfg_Req(int fd, int pwm)
{
PWMconfig config;
config.pwm=pwm+PWMNUM;//request for countconfig
e12packetout(fd,PWMCON,0x0f,&config);
return 0;
}


int E12_CountConfig(int fd, int counter, int trigger, int threshold,int flags,Cop_Data *Cop)
{
countconfig *config = &Cop->counter_config[counter];
Cop->PWM_config[counter].frequency = 0;//this turns off the PWM on that channel, if enabled
Cop->PWM_config[counter].duty = 0;
//shadow the variables and provide a config structure to pass to e12packetout
config->trigger = trigger;
config->threshold = threshold;
config->flags = flags;

e12packetout(fd,COUNTERD,0x0F,config);
return 0;
} 

int E12_CountCfg_Req(int fd, int counter)
{
countconfig config;
config.counter=counter+COUNTNUM;//request for countconfig
e12packetout(fd,COUNTERD,0x0f,&config);
return 0;
}


int E12_CounterReadRQ(int fd, int counter)
{
int command =0; //command arguement for further protocol expansions
e12packetout(fd,COUNTER,counter,&command);
return 0;
}

int E12_CounterLoad(int fd, int counter, int data, Cop_Data *Cop)
{
e12packetout(fd,COUNTER,counter+8,&data);
//Cop->counter_data[counter].countstart=data; //update countstart element
return 0;
}


int E12_CounterCalcFreq(int counter, Cop_Data *Cop)
{
unsigned long timediff_ms = (Cop->counter_data[counter].timestamp - Cop->counter_data[counter].laststamp);
unsigned long count_ms = Cop->counter_data[counter].laststart*(unsigned long)1000;
unsigned int freq=0;

if(timediff_ms!=0)
   freq =  count_ms/timediff_ms;
return(freq);
}


int E12_CountCommand(int fd, int counter,int command)
{
__u16 commandstruct[4];
commandstruct[0] = counter;
commandstruct[1] = command;
commandstruct[2] = 0x77;//currently unused
commandstruct[3] = 0x77;//currently unused
e12packetout(fd,ICNOTIFY,0x0f,commandstruct);
return 0;
}		




int E12_Digitalout(int fd, __u8 data,int port)
{
__u8 mask = 8>>port;
__u8 dataa[4];

dataa[port] = data;

if(port>1)
  return -1;

e12packetout(fd,PRTOUT,mask,dataa);
return 0;
}


//requests the status of both digital ports
int E12_Digital_Req(int fd)
{
__u8 placeholder;
e12packetout(fd,DREQ,0x0C,&placeholder);
return 0;
}


int E12_Digital_Cfg(int fd, __u8 mask, int port)
{

__u8 pmask = 8>>port;
__u8 maska[4];

maska[port] = mask;

if(port>1)
  return -1;

e12packetout(fd,PRTCFG0,pmask,maska);
return 0;
}


int E12_Internal_AtoD_Req(int fd, int channel)
{
e12packetout(fd,ADREQ0,channel,NULL);
return 0;
}

int E12_External_AtoD_Req(int fd, int channel)
{
e12packetout(fd,ADREQ1,channel,NULL);
return 0;
}


int E12_AtoD_Req(int fd, int channel)
{
if(channel<INTERNAL_ATOD_NUM)
  E12_Internal_AtoD_Req(fd, channel);
else
  {
  channel-=INTERNAL_ATOD_NUM;
  E12_External_AtoD_Req(fd, channel);
  }
return 0;
}


int E12_DtoA_Write(int fd, int channel,int data)
{
__u8 mask = 8>>channel;
__u16 dataa[4];
dataa[channel] = data;
e12packetout(fd,DAQ,mask,dataa);
return 0;
}

int E12_Serial_Write(int fd, int port, int numchars,char *buffer)
{
int charnum=0;
int chars_tx;

if(!(chars_tx=numchars%15))
  chars_tx = 15;

while(charnum<numchars)
  {

switch(port)
  {
  case RS232PORT:
    e12packetout(fd,RS232DATA,chars_tx,&buffer[charnum]);
    break;

  case RS422PORT:
    e12packetout(fd,RS422DATA,chars_tx,&buffer[charnum]);
    break;
  }

charnum+=chars_tx;

chars_tx = 15;
  }


return charnum;
}


int E12_Serial_Config(int fd, int port, int baud,int mode)
{
Serconfig config;
config.id = port;
config.mode = mode;
config.baud = baud;
e12packetout(fd,SERCFG,0x0F,&config);
return 0;
}

/*@}*/


