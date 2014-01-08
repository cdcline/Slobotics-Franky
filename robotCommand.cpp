#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include "Robot.h"
#include "37e12reqs.h"

int main(int argc, char **argv)
{
   Robot r; 
	char in = 'c';
	char in2 = 'c';
   char port;
   while(in != 'q') {
      printf("speed: %i, ISO_out: 0x%x, ISO_in: 0x%x\n", r.getSpeed(), r.getIsoOuts(), r.getIsoIns());	
      printf("m: move, o:outputs, i:inputs, q:quit\n");	
      cin >> in;
      switch ( in )
      {
         case 'm':
           printf("w:faster s:slower a:lefter d:righter\n");	
           cin >> in2;
           switch ( in2 )
           {
              case 'w':
                 r.goFaster();
                 break;
              case 's':
                 r.goSlower();
                 break;
               case 'a':
                 r.goLeft();
                 break; 
               case 'd':
                 r.goRight();
                 break;
               default:
                 break;
           }
           break; 
         case 'o':
           printf("What ISO? (0-7)\n");	
           
           /**
            * optoIso
            *
           printf("What ISO? (0-7)\n");	
           cin >> port;
           printf("o:on f:off\n");	
           cin >> in2;
           switch ( in2 )
           {
              case 'o':
                 r.turnIsoOutOn(atoi(&port));
                 break; 
              case 'f':
                 r.turnIsoOutOff(atoi(&port));
                 break; 
              default:
                 break;
           }
           */

         case 'i':
           /**
            * optoIso
            *
           printf("What ISO? (0-7)\n");	
           cin >> port;
           r.getIsoIn(atoi(&port));
            */
         case 'q':
           break;
         default:
           break;
      }
      r.move();
   }
}
