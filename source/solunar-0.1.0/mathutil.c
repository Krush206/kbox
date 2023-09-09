#include <stdio.h>
#include <math.h>
#include "mathutil.h"

/*=======================================================================
mathutil_get_maxima
=======================================================================*/
void mathutil_get_maxima (double *x, double *y, int npoints, double *mins,
  int maxmins, int *nmins)
{
  *nmins = 0;

  int i;
  for (i = 1; i < npoints - 1  && *nmins < maxmins; i++)
  {
    double y0 = y[i - 1];
    double y1 = y[i];
    double y2 = y[i + 1];
    if (y0 < y1 && y2 < y1)
    {
        double x0 = mathutil_quadratic_minmax (y1, y0, y2, 
          x[1] - x[0]);
        double xguess = x[i] + x0; 
        mins[*nmins] = xguess;
        (*nmins)++;
    }
  } 
}


/*=======================================================================
mathutil_get_minima
This function needs work
=======================================================================*/
void mathutil_get_minima (double *x, double *y, int npoints, double *mins,
  int maxmins, int *nmins)
{
  *nmins = 0;

  int i;
  for (i = 1; i < npoints - 1  && *nmins < maxmins; i++)
  {
    double y0 = y[i - 1];
    double y1 = y[i];
    double y2 = y[i + 1];
    if (y0 > y1 && y2 > y1)
    {
      //double root1 = 0.0;
      //double root2 = 0.0;
      //int nRoots = 0;
      double A = (0.5 * (y0 + y2)) - y1;
      double B = (0.5 * (y2 - y0));
      //double C = y1;
      double xExtreme = -B / (2.0 * A);
/*
      double discriminant = (B * B) - 4.0 * A * C;
printf ("(%f %f) (%f %f) (%f %f)\n", -1.0, y0, 0.0, y1, 1.0, y2); 
      if (discriminant <= 0.0)
      { 
        double DX = 0.5 * sqrt(-discriminant) / fabs(A);
        root1 = xExtreme - DX;
        root2 = xExtreme + DX;
        if (fabs(root1) <= +1.0) nRoots++;
        if (fabs(root2) <= +1.0) nRoots++;
        double bisect = (root2 + root1) / 2;
        double xguess = x[i] + (bisect * (x[i] - x[i-1])); 
        mins[*nmins] = xguess;
        (*nmins)++;
      }
*/
        double xguess = x[i-1] + ((1 + xExtreme) * (x[i] - x[i-1])); 
        mins[*nmins] = xguess;
        (*nmins)++;
    }
  } 
}

/*=======================================================================
mathutil_get_positive_axis_crossings
Find upwards zero-crossings using muller's method 
=======================================================================*/
void mathutil_get_positive_axis_crossings (double *x, double *y, 
  int npoints, double *mins, int maxmins, int *nmins)
{
  *nmins = 0;

  int i;
  for (i = 1; i < npoints - 1  && *nmins < maxmins; i++)
  {
    double y0 = y[i - 1];
    double y1 = y[i];
    double y2 = y[i + 1];
    if (y0 < 0 && y1 <= 0 && y2 > 0)
    {
      double root1 = 0.0;
      double root2 = 0.0;
      double A = (0.5 * (y0 + y2)) - y1;
      double B = (0.5 * (y2 - y0));
      double C = y1;
      double xExtreme = -B / (2.0 * A);
      double discriminant = (B * B) - 4.0 * A * C;
      if (discriminant >= 0.0)
      { 
        double DX = 0.5 * sqrt(discriminant) / fabs(A);
        root1 = xExtreme - DX;
        root2 = xExtreme + DX;
        double bisect = 0;
        if (fabs (root2) < 1.0)
          bisect = root2;
        else if (fabs (root1) < 1.0)
          bisect = root1;
        double xguess = x[i] + (bisect * (x[i] - x[i-1])); 
        mins[*nmins] = xguess;
        (*nmins)++;
      }
    }
  } 
}

/*=======================================================================
mathutil_get_negative_axis_crossings
Find downwards zero-crossings using muller's method 
=======================================================================*/
void mathutil_get_negative_axis_crossings (double *x, double *y, 
  int npoints, double *mins, int maxmins, int *nmins)
{
  *nmins = 0;

  int i;
  for (i = 1; i < npoints - 1  && *nmins < maxmins; i++)
  {
    double y0 = y[i - 1];
    double y1 = y[i];
    double y2 = y[i + 1];
    if (y0 > 0 && y1 >= 0 && y2 < 0)
    {
      double root1 = 0.0;
      double root2 = 0.0;
      double A = (0.5 * (y0 + y2)) - y1;
      double B = (0.5 * (y2 - y0));
      double C = y1;
      double xExtreme = -B / (2.0 * A);
      double discriminant = (B * B) - 4.0 * A * C;
      if (discriminant >= 0.0)
      { 
        double DX = 0.5 * sqrt(discriminant) / fabs(A);
        root1 = xExtreme - DX;
        root2 = xExtreme + DX;
        double bisect = 0;
        if (fabs (root2) < 1.0)
          bisect = root2;
        else if (fabs (root1) < 1.0)
          bisect = root1;
        double xguess = x[i] + (bisect * (x[i] - x[i-1])); 
        mins[*nmins] = xguess;
        (*nmins)++;
      }
    }
  } 
}

 
/*=======================================================================
mathutil_quadratic_minmax
Given a point (y0,0), and two surrounding points that are both smaller or
both larger (yM, -dx) and (yP, dx), find the x value of the turning point
assuming that the three points lie on a parabola
=======================================================================*/
double mathutil_quadratic_minmax (double y0, double yM, double yP, double dx)
  {
  double x1=-dx;
  double x2=dx; 
  double a = (yM*x2-yP*x1 - y0*(x2-x1))/(x1*x1*x2-x2*x2*x1);
  double b = (yM-y0-a*x1*x1)/x1;

  return -b/(2*a);
  }




