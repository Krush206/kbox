#pragma once

#include "pointerlist.h"
#include "datetime.h"
#include "latlong.h"

extern double MoonTimes_synmonth; 

extern void MoonTimes_get_moon_state_jd (double jd, double *phase, 
  double *age, double *distance);

extern void MoonTimes_get_lunar_ephemeris (double mjd, 
  double *ra, double *dec);

extern double MoonTimes_getSinAltitude (double longitude, 
  double latitude, double mjd);

void MoonTimes_get_moon_state (const DateTime *date, double *phase, 
   double *age, double *distance);

const char *MoonTimes_get_phase_name (double phase);

void MoonTimes_get_moon_rises (const LatLong *latlong, DateTime *start, 
       DateTime *end, int interval, DateTime *events[], int max_events, 
       int *nevents);

void MoonTimes_get_moon_sets (const LatLong *latlong, DateTime *start, 
       DateTime *end, int interval, DateTime *events[], int max_events, 
       int *nevents);

PointerList *MoonTimes_get_list_for_year (int year, const char *tz, BOOL utc);

