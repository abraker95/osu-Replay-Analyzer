#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define TRUE  1
#define FALSE 0

typedef struct {
   double cx,cy; /* Center */
   double r;     /* Radius */
} CIRCLE;

int InsideCircle(double,double,CIRCLE);
double CalcArea(CIRCLE *,int,long);

/*
   Calculate the intersecting area of a number of circles
   DEMONSTRATION / TEST SOFTWARE ONLY!
*/

/* Test set 0, no intersection 
#define NCIRCLES 3
CIRCLE circles[NCIRCLES] = {0.0,0.0,1.0,  3.0,0.0,1.0,  6.0,0.0,1.0}; */

/* Test set 1, no intersection (but touching) 
#define NCIRCLES 3
CIRCLE circles[NCIRCLES] = {0.0,0.0,1.0,  2.0,0.0,1.0,  4.0,0.0,1.0};  */

/* Test set 2 all are coincident, area should be pi 
#define NCIRCLES 3
CIRCLE circles[NCIRCLES] = {0.0,0.0,1.0,  0.0,0.0,1.0,  0.0,0.0,1.0}; */

/* Test set 3, two overlap, area = 4(pi/6-sqrt(3)/8) */
#define NCIRCLES 2
CIRCLE circles[NCIRCLES] = {0.0,0.0,1.0,  1.0,0.0,1.0}; 

/* Test set 4, three intersect 
#define NCIRCLES 3
CIRCLE circles[NCIRCLES] = {0.0,0.0,1.0,  1.0,0.0,1.0,  0.0,-1.0,1.0}; */

int main(int argc,char **argv)
{
   int i;
   long hits=10;
   double area;

   for (i=0;i<8;i++) {
      area = CalcArea(circles,NCIRCLES,hits);
      hits *= 10;
      printf("Area: %.10lf (hits = %ld)\n",area,hits);
   }
}

double CalcArea(CIRCLE *c,int nc,long nhits)
{
   int inside;
   long n,total=0,counter=0,i,j;
   double area,x,y;

   if (nc < 1)
      return(0.0);

   for (i=0;i<nhits;i++) {
      x = (2*drand48()-1) * c[0].r + c[0].cx;
      y = (2*drand48()-1) * c[0].r + c[0].cy;
      inside = TRUE;
      for (j=0;j<nc;j++) {
         if (!InsideCircle(x,y,c[j])) {
            inside = FALSE;
            break;
         }
      }
      if (inside)
         counter++;      
   }
   area = counter * 4 * c[0].r * c[0].r / (double)nhits;

   return(area);
}

/*
   Return TRUE if the point (x,y) is inside the circle.
   Note that the edge is considered inside, see change below.
*/
int InsideCircle(double x,double y,CIRCLE c)
{
   double dy,dx;

   dx = c.cx - x;
   dy = c.cy - y;
   if (dx*dx + dy*dy <= c.r*c.r) /* Change to < to not include the edge */
      return(TRUE);
   else
      return(FALSE);
}

