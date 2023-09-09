#pragma once


#include "latlong.h"
#include "datetime.h"
#include "error.h"

#define SUNTIMES_DEFAULT_ZENITH (90.0 + 50.0/60.0)
#define SUNTIMES_CIVIL_TWILIGHT (90 + 50.0/60.0 + 6)
#define SUNTIMES_NAUTICAL_TWILIGHT (90 + 50.0/60.0 + 12)
#define SUNTIMES_ASTRONOMICAL_TWILIGHT (90 + 50.0/60.0 + 18)

DateTime *SunTimes_get_sunrise (const LatLong *workingLatlong, 
  const DateTime *date, double zenith, Error **e);
DateTime *SunTimes_get_sunset (const LatLong *workingLatlong, 
  const DateTime *date, double zenith, Error **e);

void suntimes_getSolarRAandDec (double MJD, double *ra, double *dec);

double suntimes_getSinAltitude (double longitude, double latitude, double mjd);


