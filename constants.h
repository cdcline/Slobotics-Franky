#ifndef CONST
#define CONST

class gpioConstants
{
   public:
      enum
      {
         /**e12digitalGP device port command (data/mask)*
           *commands                                    *
           *                                            *
           *configure - i mask is input/output bitmask  *
           *1 = output 0 = input                        *
           *                                            *
           *read      - r (mask/data) unused            *
           *returns current state of the pins           * 
           *                                            *
           *write     - w data                          *
           *drive output pins with data                 *
           **********************************************/
         /**
          * There are two ports, 0 and 1. They each have 8 bits.
          * All the bits on a port can be inputs or outputs.
          *
          * If one is configured as a PWM, "writing" to it will 
          * stop the PWM.
          *
          * Thus, 3 states: INPUT, OUTPUT, PWM for the 2 (8 bit) ports  
          */ 
         INPUT = 1,
         OUTPUT = 0,
         PWM = 1,
         /**
          * gpioControl will use this to make sure you don't kill PWM signals?
          */

         FREQUENCY = 1000,
         /**
          * Pin outs are for the DIGITAL I/O Connector (HDR3) on the 37e12 
          *
          * E12_PWMControl() in Coprocessor.cpp uses 0 -> 15 to select which 
          * pin to output to. Setting up just a normal output is done by mask.
          * 
          * IDK what happens when you set a PWM out after you've configured
          * IO to be input or output.
          *
          * PT = Port 0; PP = Port 1
          */
         PP7 = 15, //PIN_1
         PP6 = 14, //PIN_3
         PP5 = 13, //PIN_5
         PP4 = 12, //PIN_7
         PP3 = 11, //PIN_9
         PP2 = 10, //PIN11
         PP1 = 9,  //PIN13
         PP0 = 8,  //PIN15
         PT7 = 7,  //PIN17
         PT6 = 6,  //PIN19
         PT5 = 5,  //PIN21
         PT4 = 4,  //PIN23
         PT3 = 3,  //PIN25
         PT2 = 2,  //PIN27
         PT1 = 1,  //PIN29
         PT0 = 0,  //PIN31

      };
};

class locoConstants
{
   public:
      static const int maxSpeed;
      static const int minSpeed;
      static const int speedToDuty[];
};

class pinConstants
{
   public:
      enum
      {
         PIN_1 = 0x80,
         PIN_3 = 0x40,
         PIN_5 = 0x20,
         PIN_7 = 0x10,
         PIN_9 = 0x08,
         PIN11 = 0x04,
         PIN13 = 0x02,
         PIN15 = 0x01,
         
         PIN17 = 0x80,
         PIN19 = 0x40,
         PIN21 = 0x20,
         PIN23 = 0x10,
         PIN25 = 0x08,
         PIN27 = 0x04,
         PIN29 = 0x02,
         PIN31 = 0x01
      };
};

class robotConstants
{
   public:
      static int isoOutArray[];
      static int isoInArray[];
      static char devPath[];
      enum
      {
         LEFT = -1,
         FORWARD = 0,
         RIGHT = 1,
         
         PORT0 = gpioConstants::OUTPUT,
         PORT1 = gpioConstants::INPUT,
         
         F_L_WHEEL = gpioConstants::PP7, //PIN_1
         F_R_WHEEL = gpioConstants::PP6, //PIN_3

         GPIO_P_1 = pinConstants::PIN_1, 
         GPIO_P_3 = pinConstants::PIN_3, 
         GPIO_P_5 = pinConstants::PIN_5, 
         GPIO_P_7 = pinConstants::PIN_7, 
         GPIO_P_9 = pinConstants::PIN_9, 
         GPIO_P11 = pinConstants::PIN11, 
         GPIO_P13 = pinConstants::PIN13, 
         GPIO_P15 = pinConstants::PIN15, 
         GPIO_P17 = pinConstants::PIN17, 
         GPIO_P19 = pinConstants::PIN19, 
         GPIO_P21 = pinConstants::PIN21, 
         GPIO_P23 = pinConstants::PIN23, 
         GPIO_P25 = pinConstants::PIN25, 
         GPIO_P27 = pinConstants::PIN27, 
         GPIO_P29 = pinConstants::PIN29, 
         GPIO_P31 = pinConstants::PIN31, 

         ISO_OUT_0 = pinConstants::PIN17,
         ISO_OUT_1 = pinConstants::PIN19,
         ISO_OUT_2 = pinConstants::PIN21,
         ISO_OUT_3 = pinConstants::PIN23,
         ISO_OUT_4 = pinConstants::PIN25,
         ISO_OUT_5 = pinConstants::PIN27,
         ISO_OUT_6 = pinConstants::PIN29,
         ISO_OUT_7 = pinConstants::PIN31,
         
         ISO_IN_0 = pinConstants::PIN_1,
         ISO_IN_1 = pinConstants::PIN_3,
         ISO_IN_2 = pinConstants::PIN_5,
         ISO_IN_3 = pinConstants::PIN_7,
         ISO_IN_4 = pinConstants::PIN_9,
         ISO_IN_5 = pinConstants::PIN11,
         ISO_IN_6 = pinConstants::PIN13,
         ISO_IN_7 = pinConstants::PIN15
      };
};

#endif
