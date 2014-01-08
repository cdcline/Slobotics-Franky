#include "Robot.h"

Robot::Robot()
{
   isoOuts[8] = false;
   speed = 5;
   direction = robotConstants::FORWARD;
   isoOutData = strtoul("0xFF", NULL, 0);
   optoIsoControl::readInput(&isoInData);

   //The GPIO has 2 ports with 8 bits.
   //Initilizes the ports to be INPUT(=0) or OUTPUT(=1)
   if (robotConstants::PORT0)
      gpioControl::configIO(0, 0xFF);
   else
      gpioControl::configIO(0, 0x00);

   if (robotConstants::PORT1)
      gpioControl::configIO(1, 0xFF);
   else
      gpioControl::configIO(1, 0x00);


}

Robot::~Robot()
{

}

int Robot::getSpeed()
{
   return speed;
}

int Robot::getDirection()
{
   return direction;
}

void Robot::goFaster()
{
   if (speed < locoConstants::maxSpeed)
      speed++;
}

void Robot::goSlower()
{
   if (speed > locoConstants::minSpeed)
      speed--;
}

isodata Robot::getIsoOuts()
{
   return isoOutData;
}

isodata Robot::getIsoIns()
{
   return isoInData;
}
/**
 * Binary => Pin_17, Pin_19, Pin_21 ...
 * 
 * 0xFF => 0000, 0000
 * 0x1F => 1110, 0000
 * 0x00 => 1111, 1111
 */
void Robot::turnIsoOutOn(int index)
{
   isoOutData &= (~robotConstants::isoOutArray[index]);
   optoIsoControl::setOutput(isoOutData);
}

void Robot::turnIsoOutOff(int index)
{
   isoOutData |= robotConstants::isoOutArray[index];
   optoIsoControl::setOutput(isoOutData);
}

void Robot::updateGPIOIn(int in)
{
   
}

void Robot::updateIsoIn()
{
   isodata* tmpData = &isoInData;
   optoIsoControl::readInput(tmpData); 
}

void Robot::getIsoIn(int in)
{
   updateIsoIn();
   isodata tmp = robotConstants::isoInArray[in] & isoInData;
   if (tmp == robotConstants::isoInArray[in])
      printf("Pin %i Off\n", in);
   else
      printf("Pin %i On\n", in);
}

void Robot::goRight()
{
   if (direction < robotConstants::RIGHT)
     direction++; 
}

void Robot::goLeft()
{
   if (direction > robotConstants::LEFT)
     direction--; 
}

void Robot::move()
{
   int oppositeSpeed = -(speed - locoConstants::maxSpeed);
   int F_L_duty = locoConstants::speedToDuty[speed];
   int F_R_duty = locoConstants::speedToDuty[oppositeSpeed]; 
   pwmControl::setPWM(robotConstants::F_L_WHEEL, F_L_duty);
   updateIsoIn();
//   printf("Duty: %i, %s\n", locoConstants::speedToDuty[speed], robotConstants::devPath);
}
