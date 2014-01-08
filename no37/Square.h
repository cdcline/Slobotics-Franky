#include "Point_2D.h"

class Square
{
   public:
      Square(double x_width, double y_width);
      void debug();
      Point_2D* Corners[4];
      /**
       * 2 * 1 
       * *   *
       * 3 * 4
       */
      enum {
       ZONE_1,
       ZONE_2,
       ZONE_3,
       ZONE_4
      };
};
