#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include "Square.h"


int main(int argc, char **argv)
{
   double x_width = 4.0;
   double y_width = 4.0;

   Square* map = new Square(x_width, y_width);
   map->debug(); 
}
