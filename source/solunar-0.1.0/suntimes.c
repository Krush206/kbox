#include <stdio.h>
#include <time.h>
#include <math.h>
#include "suntimes.h"
#include "trigutil.h"
#include "timeutil.h"
#include "roundutil.h"
#include "astro.h"

#define TYPE_SUNRISE 0
#define TYPE_SUNSET 1

static const double DEG_PER_HOUR = 360.0 / 24.0;

static time_t suntimes_getTimeUTC (int year, int month, int day, 
      double longitude, double latitude, double zenith, 
      int type);

static double suntimes_getApproxTimeDays 
     (int dayOfYear, double hoursFromMeridian, int type);

static double suntimes_getLocalMeanTime (double localHour, double
    sunRightAscensionHours, double approxTimeDays);

double suntimes_getSinAltitude (double longitude, double latitude, double mjd)
{
	double cosLatitude = cosDeg (latitude);
	double sinLatitude = sinDeg (latitude);
	double ra, dec;
	Astro_get_solar_ra_and_dec (mjd + 2400000.5, &ra, &dec);
	double TAU = 15.0 * (timeutil_lmst (mjd, longitude) - ra);
	double result = sinLatitude * sinDeg(dec)
		+ cosLatitude * cosDeg(dec) * cosDeg(TAU);
	return (result);
}

time_t suntimes_getSunriseTimeUTC (int year, int month, int day, 
      const double longitude, double latitude, double zenith)
{
  return suntimes_getTimeUTC (year, month, day, longitude, 
    latitude, zenith, TYPE_SUNRISE);
}

time_t suntimes_getSunsetTimeUTC (int year, int month, int day, 
      double longitude, double latitude, double zenith)
{
  return suntimes_getTimeUTC (year, month, day, longitude, latitude, 
    zenith, TYPE_SUNSET);
}

/**
Get time difference between location's longitude 
and the Meridian, in hours. West of Meridian has
a negative time difference
*/
double suntimes_getHoursFromMeridian (double longitude)
  {
  return longitude / DEG_PER_HOUR;
  }


/**
 Calculate the Sun's mean anomaly in degrees, 
at sunrise or sunset, given the longitude
in degrees
*/
double suntimes_getMeanAnomaly (int dayOfYear, double longitude, int type)
  {
  return (0.9856 * suntimes_getApproxTimeDays
     (dayOfYear, suntimes_getHoursFromMeridian (longitude), type)) - 3.289;
  }

double suntimes_getApproxTimeDays 
     (int dayOfYear, double hoursFromMeridian, int type)
  {
  if (type == TYPE_SUNRISE)
    return dayOfYear + ((6.0 - hoursFromMeridian) / 24);
  else
    return dayOfYear + ((18.0 - hoursFromMeridian) / 24);
  }

/**
Calculates the Sun's right ascension in hours, given
the Sun's true longitude in degrees. 
Input and output are angles gte 0 and lt 360. 
*/
double suntimes_getSunRightAscensionHours (double sunTrueLongitude)
  {
	  double a = 0.91764 * tanDeg(sunTrueLongitude);
  double ra = 360.0 / (2.0 * M_PI) * atan(a);
  // get result into 0-360 degree range
  //if (ra >= 360.0) ra = ra - 360.0;
  //if (ra < 0) ra = ra + 360.0;

  double lQuadrant  = floor(sunTrueLongitude/90.0) * 90.0;
  double raQuadrant = floor(ra/90.0) * 90.0;
  ra = ra + (lQuadrant - raQuadrant);
	
  return ra/DEG_PER_HOUR; //convert to hours
  }


/**
Calculates the Sun's true longitude in degrees. The result
is an angle gte 0 and lt 360. Requires the Sun's mean
anomaly, also in degrees
*/
double suntimes_getSunTrueLongitude (double sunMeanAnomaly)
  {
	  double l = sunMeanAnomaly + (1.916 * sinDeg(sunMeanAnomaly)) 
		+ (0.020 * sinDeg(2 * sunMeanAnomaly)) + 282.634;

  // get longitude into 0-360 degree range
  if (l >= 360.0) l = l - 360.0;
  if (l < 0) l = l + 360.0;
  return l;
  }

/**
Gets the cosine of the Sun's local hour angle
*/
double suntimes_getCosLocalHourAngle (double sunTrueLongitude, double latitude, double zenith)
  {
	  double sinDec = 0.39782 * sinDeg(sunTrueLongitude);
  double cosDec = cosDeg(asinDeg(sinDec));
	
  double cosH = (cosDeg(zenith) - (sinDec * sinDeg(latitude))) / (cosDec * cosDeg(latitude));
	
  // Check bounds

  return cosH;
  }

/**
Calculate local mean time of rising or setting. By `local' is meant the exact
time at the location, assuming that there were no time zone. That is, 
the time difference between the location and the Meridian depended entirely
on the longitude. We can't do anything with this time directly; we must
convert it to UTC and then to a locale time. The result is expressed
as a fractional number of hours since midnight
*/
double suntimes_getLocalMeanTime (double localHour, double
    sunRightAscensionHours, double approxTimeDays)
  {
  return localHour + sunRightAscensionHours - (0.06571 * approxTimeDays) - 6.622;
  }


time_t suntimes_getTimeUTC (const int year, const int month, const int day, 
      const double longitude, const double latitude, const double zenith, const int type)
{
  int dayOfYear = timeutil_getDayOfYear (year, month, day);
  double sunMeanAnomaly = suntimes_getMeanAnomaly (dayOfYear, longitude, type);  
  double sunTrueLong = suntimes_getSunTrueLongitude (sunMeanAnomaly);
  double sunRightAscensionHours = suntimes_getSunRightAscensionHours (sunTrueLong);
  double cosLocalHourAngle = suntimes_getCosLocalHourAngle (sunTrueLong, latitude, zenith);

  double localHourAngle;
  if (type == TYPE_SUNRISE)
    {
    if (cosLocalHourAngle > 1) return 0;
    localHourAngle = 360.0 - acosDeg(cosLocalHourAngle);
    }
  else if (type == TYPE_SUNSET)
    {
    if (cosLocalHourAngle < -1) return 0;
    localHourAngle = acosDeg(cosLocalHourAngle);
    }
  else
      return 0; // should never happen

  double localHour = localHourAngle / DEG_PER_HOUR;

  double localMeanTime = suntimes_getLocalMeanTime (localHour, sunRightAscensionHours, 
    suntimes_getApproxTimeDays(dayOfYear, suntimes_getHoursFromMeridian(longitude), type));

  double temp = localMeanTime - suntimes_getHoursFromMeridian (longitude);
  if (temp < 0) temp += 24;
  if (temp > 24) temp -= 24;

  return timeutil_makeTimeGMT (year, month, day, temp);
}




/*=======================================================================
SunTimes_get_sunrise
=======================================================================*/
DateTime *SunTimes_get_sunrise (const LatLong *latlong, 
    const DateTime *date, double zenith, Error **e)
  {
  int dayOfYear = DateTime_get_day_of_year(date);
  double sunMeanAnomaly = suntimes_getMeanAnomaly (dayOfYear, 
    LatLong_get_longitude (latlong), TYPE_SUNRISE);  
  double sunTrueLong = suntimes_getSunTrueLongitude (sunMeanAnomaly);
  double sunRightAscensionHours = suntimes_getSunRightAscensionHours (sunTrueLong);
  double cosLocalHourAngle = suntimes_getCosLocalHourAngle (sunTrueLong, 
    LatLong_get_latitude (latlong), zenith);

  double localHourAngle;
  if (cosLocalHourAngle > 1 || cosLocalHourAngle < -1)
      {
      *e = Error_new ("No sunrise");
      return NULL;
      } 
  localHourAngle = 360.0 - acosDeg(cosLocalHourAngle);

  double localHour = localHourAngle / DEG_PER_HOUR;

  double localMeanTime = suntimes_getLocalMeanTime 
    (localHour, sunRightAscensionHours, 
    suntimes_getApproxTimeDays(dayOfYear, 
      suntimes_getHoursFromMeridian(LatLong_get_longitude (latlong)),    
      TYPE_SUNRISE));

  double temp = localMeanTime - suntimes_getHoursFromMeridian 
    (LatLong_get_longitude (latlong));
  if (temp < 0) temp += 24;
  if (temp > 24) temp -= 24;
  DateTime *result = DateTime_clone (date); 
  DateTime_set_time_hours_fraction (result, temp);
  return result;
  }


/*=======================================================================
SunTimes_get_sunset
=======================================================================*/
DateTime *SunTimes_get_sunset (const LatLong *latlong, 
    const DateTime *date, double zenith, Error **e)
  {
  int dayOfYear = DateTime_get_day_of_year(date);
  double sunMeanAnomaly = suntimes_getMeanAnomaly (dayOfYear, 
    LatLong_get_longitude (latlong), TYPE_SUNRISE);  
  double sunTrueLong = suntimes_getSunTrueLongitude (sunMeanAnomaly);
  double sunRightAscensionHours = suntimes_getSunRightAscensionHours (sunTrueLong);
  double cosLocalHourAngle = suntimes_getCosLocalHourAngle (sunTrueLong, 
    LatLong_get_latitude (latlong), zenith);

  double localHourAngle;
  if (cosLocalHourAngle > 1 || cosLocalHourAngle < -1)
    {
    *e = Error_new ("No sunset");
    return NULL;
    } 
  localHourAngle = acosDeg(cosLocalHourAngle);

  double localHour = localHourAngle / DEG_PER_HOUR;

  double localMeanTime = suntimes_getLocalMeanTime 
    (localHour, sunRightAscensionHours, 
    suntimes_getApproxTimeDays(dayOfYear, 
      suntimes_getHoursFromMeridian(LatLong_get_longitude (latlong)),    
      TYPE_SUNSET));

  double temp = localMeanTime - suntimes_getHoursFromMeridian 
    (LatLong_get_longitude (latlong));
  if (temp < 0) temp += 24;
  if (temp > 24) temp -= 24;
  DateTime *result = DateTime_clone (date); 
  DateTime_set_time_hours_fraction (result, temp);
  return result;
  }

