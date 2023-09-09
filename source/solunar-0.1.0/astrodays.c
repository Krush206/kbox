/*=======================================================================
astrodays.c
Methods for getting the dates of days with astronomical significance.
We are using Meeus' method for determining equinoxes, which should be
accurate to within a few minutes for all practicable dates
=======================================================================*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "defs.h"
#include "datetime.h"
#include "astrodays.h"
#include "trigutil.h"
#include "astro.h"
#include "mathutil.h"

/*=======================================================================
AstroDays_periodic24
Meeus' correction method for solstices. Don't ask me how the math
works :)
=======================================================================*/
double AstroDays_periodic24 (double t)
  {
  int i;
  static double A[24] = {485,203,199,182,156,136,77,74,70,58,52,50,45,44,29,18,17,16,14,12,12,12,9,8};
  static double B[24] = {324.96,337.23,342.08,27.85,73.14,171.52,222.54,296.72,243.58,119.81,297.17,21.02, 247.54,325.15,60.93,155.12,288.79,198.04,199.76,95.39,287.11,320.81,227.73,15.45};
  static double C[24] = {1934.136,32964.467,20.186,445267.112,45036.886,22518.443, 65928.934,3034.906,9037.513,33718.147,150.678,2281.226, 29929.562,31555.956,4443.417,67555.328,4562.452,62894.029, 31436.921,14577.848,31931.756,34777.259,1222.114,16859.074};
  double s = 0.0;
  for (i = 0; i < 24; i++)
    s += A[i]*cosDeg (B[i] + C[i]*t);
  return s;
  }


/*=======================================================================
AstroDays_get_vernal_equinox
=======================================================================*/
DateTime *AstroDays_get_vernal_equinox (int year)
  {
  double m = (year - 2000.0) / 1000.0;
  double ve = 2451623.80984 + 365242.37404 * m + 0.05169 * m
       * m - 0.00411 * m * m * m - 0.00057 * m * m * m * m ;
  double t = (ve - 2451545.0) / 36525.0;
  double w = 35999.373*t - 2.47;
  double dL = 1 + 0.0334*cosDeg(w) + 0.0007*cosDeg(2*w);
  double s = AstroDays_periodic24 (t);
  ve = ve + ((0.00001*s) / dL); 	

  DateTime *r = DateTime_new_julian (ve);
  DateTime_set_name (r, "vernal equinox");
  return r;
  }


/*=======================================================================
AstroDays_get_autumnal_equinox
=======================================================================*/
DateTime *AstroDays_get_autumnal_equinox (int year)
  {
  double m = (year - 2000.0) / 1000.0;
  double ae = 2451810.21715 + 365242.01767 * m - 0.11575 * m 
      * m + 0.00337 * m * m * m + 0.00078 * m * m * m * m ;
  double t = (ae - 2451545.0) / 36525.0;
  double w = 35999.373*t - 2.47;
  double dL = 1 + 0.0334*cosDeg(w) + 0.0007*cosDeg(2*w);
  double s = AstroDays_periodic24 (t);
  ae = ae + ((0.00001*s) / dL); 	

  DateTime *r = DateTime_new_julian (ae);
  DateTime_set_name (r, "autumnal equinox");
  return r;
  }


/*=======================================================================
AstroDays_get_winter_solstice
=======================================================================*/
DateTime *AstroDays_get_winter_solstice (int year)
  {
  double m = (year - 2000.0) / 1000.0;
  double ws = 2451900.05952 + 365242.74049 * m - 0.06223 * m 
    * m - 0.00823 * m * m * m + 0.00032 * m * m * m * m ;
  double t = (ws - 2451545.0) / 36525.0;
  double w = 35999.373*t - 2.47;
  double dL = 1 + 0.0334*cosDeg(w) + 0.0007*cosDeg(2*w);
  double s = AstroDays_periodic24 (t);
  ws = ws + ((0.00001*s) / dL); 	

  DateTime *r = DateTime_new_julian (ws);
  DateTime_set_name (r, "winter solstice");
  return r;
  }


/*=======================================================================
AstroDays_get_summer_solstice
=======================================================================*/
DateTime *AstroDays_get_summer_solstice (int year)
  {
  double m = (year - 2000.0) / 1000.0;
  double ss = 2451716.56767 + 365241.62603 * m + 0.00325 * m 
      * m + 0.00888 * m * m * m - 0.00030 * m * m * m * m ;
  double t = (ss - 2451545.0) / 36525.0;
  double w = 35999.373*t - 2.47;
  double dL = 1 + 0.0334*cosDeg(w) + 0.0007*cosDeg(2*w);
  double s = AstroDays_periodic24 (t);
  ss = ss + ((0.00001*s) / dL); 	

  DateTime *r = DateTime_new_julian (ss);
  DateTime_set_name (r, "summer solstice");
  return r;
  }


/*=======================================================================
AstroDays_get_perihelion
=======================================================================*/
DateTime *AstroDays_get_perihelion (int year)
  {
  int i;
  double minR = 10; // Got to set something big here
  double d = 0;
  double h = 0;

  // Find the lowest R over the first six days of the year
  for (i = 1; i < 6; i++)
    {
    DateTime * dd = DateTime_new_dmy (i, 1, year, NULL, FALSE); // order?
    double jd = DateTime_get_julian_date (dd) + 0.5;
    double R = Astro_get_earth_sun_distance (jd); 
    if (R < minR) 
      {
      minR = R;
      d = i;
      }
    DateTime_free (dd);
    }

  minR = 10;
  d = d - 1; // day offset from zero

  // Find the lower R within the day found
  for (i = 0; i < 49; i++)
    {
    DateTime * dd = DateTime_new_dmy (d, 1, year, NULL, FALSE);
    double jd = DateTime_get_julian_date (dd) + (double) i / 24.0;
    double R = Astro_get_earth_sun_distance (jd); 
    if (R < minR) 
      {
      minR = R;
      h = i;
      }
    DateTime_free (dd);
    }

  double min = 0; // Let's not bother with the minutes :)
  h = h + min/60.0;
  if (h >= 24.0) 
    {
    h = h - 24;
    d++;
    }

  // So now we have the perihelion to the nearest hour

  DateTime * dd = DateTime_new_dmy (d, 1, year, NULL, FALSE);
  double jd = DateTime_get_julian_date (dd);
  double r0 = Astro_get_earth_sun_distance (jd + h / 24.0);
  double rM = Astro_get_earth_sun_distance (jd + (h - 1) / 24.0);
  double rP = Astro_get_earth_sun_distance (jd + (h + 1) / 24.0);
  DateTime_free (dd);

  double hour_min = mathutil_quadratic_minmax (r0, rM, rP, 1.0);

  double p = jd + h / 24.0 + hour_min / 24.0;

  DateTime *r = DateTime_new_julian (p);
  DateTime_set_name (r, "perihelion");
  return r;
  }


/*=======================================================================
AstroDays_get_aphelion
=======================================================================*/
DateTime *AstroDays_get_aphelion (int year)
  {
  int i;
  double maxR = 0;
  double d = 0;
  double h = 100; // something big

  // Find the largest R over the beginning of july 
  for (i = 3; i < 7; i++)
    {
    DateTime * dd = DateTime_new_dmy (i, 7, year, NULL, FALSE); 
    double jd = DateTime_get_julian_date (dd) + 0.5;
    double R = Astro_get_earth_sun_distance (jd); 
    if (R > maxR) 
      {
      maxR = R;
      d = i;
      }
    DateTime_free (dd);
    }

  maxR = 0;
  d = d - 1; // day offset from zero

  // Find the lower R within the day found
  for (i = 0; i < 49; i++)
    {
    DateTime * dd = DateTime_new_dmy (d, 7, year, NULL, FALSE);
    double jd = DateTime_get_julian_date (dd) + (double) i / 24.0;
    double R = Astro_get_earth_sun_distance (jd); 
    if (R > maxR) 
      {
      maxR = R;
      h = i;
      }
    DateTime_free (dd);
    }

  if (h >= 24.0) 
    {
    h = h - 24;
    d++;
    }

  h = h - 1;
  maxR = 0;
  double min = 0; 

  // Find the lower R within the hour found
  // We need to go to the minute here, because the value of R is changing
  //  so slowly we can't get decent accuracy otherwise
  for (i = 0; i < 121; i++)
    {
    DateTime * dd = DateTime_new_dmy (d, 7, year, NULL, FALSE);
    double jd = DateTime_get_julian_date (dd) + 
        h / 24.0 + (double) i / 60.0 / 24.0;
    double R = Astro_get_earth_sun_distance (jd); 
    if (R > maxR) 
      {
      maxR = R;
      min = i;
      }
    DateTime_free (dd);
    }

  h += (double) min/60.0;
  if (h >= 24.0) 
    {
    h = h - 24;
    d++;
    }


  // So now we have the aphelion to the nearest hour and minute

  DateTime * dd = DateTime_new_dmy (d, 7, year, NULL, FALSE);
  double jd = DateTime_get_julian_date (dd);
  double r0 = Astro_get_earth_sun_distance (jd + h / 24.0);
  double rM = Astro_get_earth_sun_distance (jd + (h - 1.0) / 24.0);
  double rP = Astro_get_earth_sun_distance (jd + (h + 1.0) / 24.0);
  DateTime_free (dd);

  double hour_min = mathutil_quadratic_minmax (r0, rM, rP, 1.0);

  double p = jd + h /24.0 + hour_min / 24.0;

  DateTime *r = DateTime_new_julian (p);
  DateTime_set_name (r, "aphelion");
  return r;
  }


/*=======================================================================
AstroDays_get_list_for_year 
=======================================================================*/
PointerList *AstroDays_get_list_for_year (int year, 
     const char *tz, BOOL utc)
  {
  PointerList *l = NULL;
  DateTime *vernal_equinox = AstroDays_get_vernal_equinox (year);
  l = PointerList_append (l, vernal_equinox);
  DateTime *autumnal_equinox = AstroDays_get_autumnal_equinox (year);
  l = PointerList_append (l, autumnal_equinox);
  DateTime *winter_solstice = AstroDays_get_winter_solstice (year);
  l = PointerList_append (l, winter_solstice);
  DateTime *summer_solstice = AstroDays_get_summer_solstice (year);
  l = PointerList_append (l, summer_solstice);
  DateTime *perihelion = AstroDays_get_perihelion (year);
  l = PointerList_append (l, perihelion);
  DateTime *aphelion = AstroDays_get_aphelion (year);
  l = PointerList_append (l, aphelion);

  return l;
  }

