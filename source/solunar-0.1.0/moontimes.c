#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "moontimes.h" 
#include "timeutil.h" 
#include "trigutil.h" 
#include "roundutil.h" 
#include "mathutil.h" 
#include "astrodays.h" 
#include "astro.h" 

/*=======================================================================
DateTime_getSinAltitude
=======================================================================*/
double MoonTimes_getSinAltitude (double longitude, double latitude, double mjd)
{
	double cosLatitude = cosDeg (latitude);
	double sinLatitude = sinDeg (latitude);
	double ra, dec;
	Astro_get_lunar_ra_and_dec (mjd + 2400000.5, &ra, &dec);
	double TAU = 15.0 * (timeutil_lmst (mjd, longitude) - ra);
	double result	= sinLatitude * sinDeg(dec)
		+ cosLatitude * cosDeg(dec) * cosDeg(TAU);
	return result;
}


/*=======================================================================
DateTime_get_phase_name
=======================================================================*/
const char *MoonTimes_get_phase_name (double phase)
  {
	if (phase < 0 + 0.0625) return "new";
	if (phase < 0.125 + 0.0625) return "waxing crescent";
	if (phase < 0.25 + 0.0625) return "1st quarter";
	if (phase < 0.375 + 0.0625) return "waxing gibbous";
	if (phase < 0.5 + 0.0625) return "full";
	if (phase < 0.625 + 0.0625) return "waning gibbous";
	if (phase < 0.75 + 0.0625) return "3rd quarter";
	if (phase < 0.875 + 0.0625) return "waning crescent";
	return "new";
  }

/*=======================================================================
DateTime_get_moon_state
Note that moon phases are inverted for the southern hemisphere, although
the cycle is deemed to start on the same date, and description is
the same 
=======================================================================*/
void MoonTimes_get_moon_state (const DateTime *date, double *phase, 
   double *age, double *distance)
{
  return Astro_get_moon_state (DateTime_get_julian_date (date),
    phase, age, distance);
}


/*=======================================================================
DateTime_get_moon_rises
Determines zero or more moonrises withing the specified start and
end times. Results are written into events[] and *nevents specifies
the number found. The caller must free the contents of events[], if
any 
=======================================================================*/
void MoonTimes_get_moon_rises (const LatLong *latlong, 
       DateTime *start, DateTime *end, int interval, 
       DateTime *events[], int max_events, int *nevents)
  {
  long seconds_difference = DateTime_seconds_difference (start, end); 
  int npoints = seconds_difference / interval;

  double *x = (double *) malloc (npoints * sizeof (double));
  double *y = (double *) malloc (npoints * sizeof (double));

  DateTime *tx = DateTime_clone (start);

  int i;
  for (i = 0; i < npoints; i++)
  {
    double alt = MoonTimes_getSinAltitude 
      (LatLong_get_longitude (latlong), LatLong_get_latitude (latlong), 
       DateTime_get_modified_julian_date (tx));
    x[i] = i * interval;
    y[i] = alt;
    DateTime_add_seconds (tx, interval); 
  }

  // Note that x values are in seconds relative to 00:00 on the day
  //  in question

  double *d_events = (double *) malloc (max_events * sizeof (double));
  mathutil_get_positive_axis_crossings (x, y, npoints, d_events, 
    max_events, nevents);

  for (i = 0; i < *nevents; i++) 
    { 
    long offset_from_start = (long) d_events [i]; 
    events[i] = DateTime_clone (start);
    DateTime_add_seconds (events[i], offset_from_start); 
    }
  free (d_events);

  DateTime_free (tx);
  free (x);
  free (y);
  }


/*=======================================================================
DateTime_get_moon_sets
Determines zero or more moonrises withing the specified start and
end times. Results are written into events[] and *nevents specifies
the number found. The caller must free the contents of events[], if
any 
=======================================================================*/
void MoonTimes_get_moon_sets (const LatLong *latlong, 
       DateTime *start, DateTime *end, int interval, 
       DateTime *events[], int max_events, int *nevents)
  {
  long seconds_difference = DateTime_seconds_difference (start, end); 
  int npoints = seconds_difference / interval;

  double *x = (double *) malloc (npoints * sizeof (double));
  double *y = (double *) malloc (npoints * sizeof (double));

  DateTime *tx = DateTime_clone (start);

  int i;
  for (i = 0; i < npoints; i++)
  {
    double alt = MoonTimes_getSinAltitude 
      (LatLong_get_longitude (latlong), LatLong_get_latitude (latlong), 
       DateTime_get_modified_julian_date (tx));
    x[i] = i * interval;
    y[i] = alt;
    DateTime_add_seconds (tx, interval); 
  }

  // Note that x values are in seconds relative to 00:00 on the day
  //  in question

  double *d_events = (double *) malloc (max_events * sizeof (double));
  mathutil_get_negative_axis_crossings (x, y, npoints, d_events, 
    max_events, nevents);

  for (i = 0; i < *nevents; i++) 
    { 
    long offset_from_start = (long) d_events [i]; 
    events[i] = DateTime_clone (start);
    DateTime_add_seconds (events[i], offset_from_start); 
    }
  free (d_events);

  DateTime_free (tx);
  free (x);
  free (y);
  }


/*=======================================================================
MoonTimes_get_new_moons
=======================================================================*/
PointerList *MoonTimes_get_full_moons (PointerList *in, int year, 
    const char *tz, BOOL utc)
  {
  #define NPOINTS_FM 2*365
          // Number of interpolation points -- two a day
          // Increasing does not seem to lead to improved accuracy 
  #define MAX_FULL_MOONS 50
          // shouldn't be this many FM's in a year :)
  int i;
  PointerList *l = in;

  double x[NPOINTS_FM];
  double y[NPOINTS_FM];
  long step = 24 * 3600 / 2;

  DateTime *start = DateTime_new_dmy (1, 1, year, tz, utc);
  DateTime *t = DateTime_clone (start); 
  for (i = 0; i < NPOINTS_FM; i++)
    {
    x[i] = i * step;
    double dummy, phase; 
    MoonTimes_get_moon_state (t, &phase, &dummy, &dummy); 
    double unfullness = phase - 0.5;
    // unfullness will cross the x axis at full and new moons
    y[i] = unfullness; 
    // Although we are going in increments of one notional day, we have to 
    //  add seconds, not days, else we will get discontinuities at 
    //  DST changes
    DateTime_add_seconds (t, step);
    }

  double *d_events = (double *) malloc (MAX_FULL_MOONS * sizeof (double));
  int nevents = 0; 
  mathutil_get_positive_axis_crossings (x, y, NPOINTS_FM, d_events, 
    MAX_FULL_MOONS, &nevents);

  DateTime *ae = AstroDays_get_autumnal_equinox (year);

  int harvest_moon_index = 0;
  long min_ae_distance = -1;
  for (i = 0; i < nevents; i++) 
    { 
    long offset_from_start = (long) d_events [i]; 
    DateTime *event = DateTime_clone (start);
    DateTime_add_seconds (event, offset_from_start); 
    long ae_distance = DateTime_seconds_difference (event, ae);
    if (ae_distance < 0) ae_distance = -ae_distance;
    if (ae_distance < min_ae_distance || min_ae_distance < 0)
      {
      harvest_moon_index = i;
      min_ae_distance = ae_distance;
      }
    DateTime_free (event);
    }

  DateTime_free (ae);

  int hunters_moon_index = -1;
  if (harvest_moon_index < nevents - 1)
    hunters_moon_index = harvest_moon_index  + 1;

  for (i = 0; i < nevents; i++) 
    { 
    long offset_from_start = (long) d_events [i]; 
    DateTime *event = DateTime_clone (start);
    DateTime_add_seconds (event, offset_from_start); 
    if (i == harvest_moon_index)
      DateTime_set_name (event, "harvest moon"); 
    else if (i == hunters_moon_index)
      DateTime_set_name (event, "hunter's moon"); 
    else
      DateTime_set_name (event, "full moon"); 
    l = PointerList_append (l, event);
    }
  free (d_events);

  DateTime_free (t);
  return l;
  }

/*=======================================================================
MoonTimes_get_new_moons
=======================================================================*/
PointerList *MoonTimes_get_new_moons (PointerList *in, int year, 
    const char *tz, BOOL utc)
  {
  #define NPOINTS_NM 32*365
          // Number of interpolation points -- we need a long 
          // more poins because the moon phase function is increasing
          // quite slowly around the new moons 
  #define MAX_NEW_MOONS 50
          // shouldn't be this many FM's in a year :)
  int i;
  PointerList *l = in;

  double x[NPOINTS_NM];
  double y[NPOINTS_NM];
  long step = 24 * 3600 / 32;

  DateTime *start = DateTime_new_dmy (1, 1, year, tz, utc);
  DateTime *t = DateTime_clone (start); 
  for (i = 0; i < NPOINTS_NM; i++)
    {
    x[i] = i * step;
    double dummy, phase; 
    MoonTimes_get_moon_state (t, &phase, &dummy, &dummy); 
    double newness = fabs (phase - 0.5);
    // unfullness will cross the x axis at full and new moons
    y[i] = newness; 
    // Although we are going in increments of one notional day, we have to 
    //  add seconds, not days, else we will get discontinuities at 
    //  DST changes
    DateTime_add_seconds (t, step);
    }

  double *d_events = (double *) malloc (MAX_NEW_MOONS * sizeof (double));
  int nevents = 0; 
  mathutil_get_maxima (x, y, NPOINTS_NM, d_events, 
    MAX_NEW_MOONS, &nevents);

  for (i = 0; i < nevents; i++) 
    { 
    long offset_from_start = (long) d_events [i]; 
    DateTime *event = DateTime_clone (start);
    DateTime_add_seconds (event, offset_from_start); 
    DateTime_set_name (event, "new moon"); 
    l = PointerList_append (l, event);
    }
  free (d_events);

  DateTime_free (t);
  return l;
  }

/*=======================================================================
MoonTimes_get_list_for_year 
We need tz info here because the year runs from midnight to midnight, and
midnight will be different in different zones
=======================================================================*/
PointerList *MoonTimes_get_list_for_year (int year, const char *tz, BOOL utc)
  {
  PointerList *l = NULL;

  l = MoonTimes_get_full_moons (l, year, tz, utc);
  l = MoonTimes_get_new_moons (l, year, tz, utc);

  return l;
  }


