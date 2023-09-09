#pragma once

void mathutil_get_maxima (double *x, double *y, int npoints, double *mins,
  int maxmins, int *nmins);

void mathutil_get_minima (double *x, double *y, int npoints, double *mins,
  int maxmins, int *nmins);

void mathutil_get_positive_axis_crossings (double *x, double *y, 
  int npoints, double *mins, int maxmins, int *nmins);

void mathutil_get_negative_axis_crossings (double *x, double *y, 
  int npoints, double *mins, int maxmins, int *nmins);

double mathutil_quadratic_minmax (double r0, double rM, double rP, double dx);
 
 

