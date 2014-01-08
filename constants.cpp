#include "constants.h"


const int locoConstants::maxSpeed = 10;
const int locoConstants::minSpeed = 0;
const int locoConstants::speedToDuty[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

int robotConstants::isoOutArray[] = 
{
   robotConstants::ISO_OUT_0, robotConstants::ISO_OUT_1, 
   robotConstants::ISO_OUT_2, robotConstants::ISO_OUT_3,
   robotConstants::ISO_OUT_4, robotConstants::ISO_OUT_5,
   robotConstants::ISO_OUT_6, robotConstants::ISO_OUT_7
};

int robotConstants::isoInArray[] = 
{
   robotConstants::ISO_IN_0, robotConstants::ISO_IN_1, 
   robotConstants::ISO_IN_2, robotConstants::ISO_IN_3,
   robotConstants::ISO_IN_4, robotConstants::ISO_IN_5,
   robotConstants::ISO_IN_6, robotConstants::ISO_IN_7
};

char robotConstants::devPath[] = "/dev/37e12";
