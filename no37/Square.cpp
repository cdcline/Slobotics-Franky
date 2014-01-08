#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include "Square.h"

Square::Square(double x_width, double y_width)
{
   /**
    * 2 * 1 
    * *   *
    * 3 * 4
    */ 
   Corners[ZONE_1] = new Point_2D(x_width, y_width); 
   Corners[ZONE_2] = new Point_2D(0.0,     y_width); 
   Corners[ZONE_3] = new Point_2D(0.0,     0.0);
   Corners[ZONE_4] = new Point_2D(x_width,(double)0.0); 
}

void Square::debug()
{
   printf("2 * 1\n*   *\n3 * 4\n");
   printf("Zone 1: ");
   Corners[ZONE_1]->debug();
   printf("\nZone 2: ");
   Corners[ZONE_2]->debug();
   printf("\nZone 3: ");
   Corners[ZONE_3]->debug();
   printf("\nZone 4: ");
   Corners[ZONE_4]->debug();
   printf("\n");
}
