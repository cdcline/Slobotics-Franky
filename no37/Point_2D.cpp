#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include "Point_2D.h"

Point_2D::Point_2D(double initialX, double initialY)
{
   x = initialX;
   y = initialY;
}

void Point_2D::debug()
{
   printf("(%f, %f)", x, y);
}
