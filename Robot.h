#ifndef ROBOT_H
#define ROBOT_H

#include "37e12reqs.h"
#include "ioControl.h"

class Robot
{
   private:
      int speed;
      int direction;
      bool isoOuts[];
      isodata isoOutData;
      isodata isoInData;
   public:
      Robot();
      ~Robot();
      int getSpeed();
      int getDirection();
      isodata getIsoOuts();
      isodata getIsoIns();
      void goFaster();
      void goSlower();
      void goLeft();
      void goRight();
      void move();
      void updateGPIOIn(int in);
      void turnIsoOutOn(int index);
      void turnIsoOutOff(int index);
      void updateIsoIn();
      void getIsoIn(int in);
};
#endif /*ROBOT_H*/
