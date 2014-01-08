#ifndef IO_CONTROL_H
#define IO_CONTROL_H
class optoIsoControl
{
   public:
      static void setOutput(isodata data);
      static void readInput(isodata * data);
};
#define DIGRESPONSE BITMASK(PRTIN0)
class gpioControl
{
   public:
      static int port0_conf, port1_conf;
      static void configIO(int port, unsigned char mask);
      static void writePort(int port, unsigned char data);
      static void readPort(int port, unsigned char data);
};

class pwmControl
{
   public:
      static void setPWM(int lineNumber, int dutyCycle);
};
#endif
