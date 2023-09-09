#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "city.h"
#include "cityinfo.h"
#include "latlong.h"
#include "pointerlist.h"
#include "error.h"
#include "datetime.h"
#include "suntimes.h"
#include "moontimes.h"
#include "astrodays.h"
#include "nameddays.h"
#include "astro.h"


/*=======================================================================
print_short_usage
=======================================================================*/
void print_short_usage(const char *argv0)
  {
  printf ("Usage: %s [options]\n", argv0);
  printf ("\"%s --longhelp\" for full details\n", argv0);
  }

/*=======================================================================
print_cities
=======================================================================*/
void print_cities()
  {
  City *city = cities;
  while (city->name)
    {
    printf ("%s\n", city->name);
    city++;
    };
  }

/*=======================================================================
print_long_usage
=======================================================================*/
void print_long_usage(const char *argv0)
  {
  printf ("Usage: %s [options]\n", argv0);
  printf ("  -c, --city [name]              specify city\n");
  printf ("  --cities                       print list of cities\n");
  printf ("  -d, --datetime [date_time]     set date and/or time\n");
  printf ("  -f, --full                     print full data\n");
  printf ("  -h                             print brief help message\n");
  printf ("  -l, --latlong [+DDMM+DDDMM]    latitude/longitude\n");
  printf ("  --list-astro-events            list equinoxes, etc\n");
  printf ("  --list-named-days              list days with names\n");
  printf ("  --list-moon-events             list full moons, etc\n");
  printf ("  --longhelp                     print long help message\n");
  printf ("  -q, --quiet                    no captions or interim results\n");
  printf ("  -u, --utc                      times and dates are UTC\n");
  printf ("  -v, --version                  print version\n");
  }

/*=======================================================================
print_datetime_help
=======================================================================*/
void print_datetime_help()
  {
  printf ("\n");
  printf ("Date/time formats\n");
  printf ("=================\n");
  printf ("\n");
  printf ("Either date or time or both may be specified. If both are given,\n");
  printf ("date must come first. Time is in 24-hour format, with a colon \n");
  printf ("between hours and minutes -- HH:MM. If not time is given, midnight \n");
  printf ("is assumed.\n"); 
  printf ("\n");

  printf ("The following date representations are understood: \n");
  printf ("DD/MM/YYYY\n");
  printf ("DD/MM\n");
  printf ("DD/MM/YYYY\n");
  printf ("month_name DD YYYY\n");
  printf ("month_name DD\n");
  printf ("DD month_name YYYY\n");
  printf ("DD month_name\n");
  printf ("DDD#YYYY            (day of year, 1-366; for use in scripts)\n");
  printf ("\n");
  printf ("Date and time are assumed to be local to the selected city, or the\n");
  printf ("home locale if not city is specified, including daylist savings where\n");
  printf ("applicable. This behaviour is overridden when the --utc switch is in\n");
  printf ("effect.\n");
  printf ("\n");
  }

/*=======================================================================
print_latlong_help
=======================================================================*/
void print_latlong_help()
  {
  printf ("\n");
  printf ("Latitude/longitude format\n");
  printf ("=========================\n");
  printf ("\n");
  printf ("1: +DDMM+DDDMM\n");
  printf ("First number group is latitude degrees and minutes, with '+' being north.\n");
  printf ("Second number group is longitude degrees and minutes, with '+' being east.\n");
  printf ("The two groups must have four and five digits respectively.\n");
  printf ("\n");
  printf ("2: NN.NN,NN.NN\n");
  printf ("First number is a decimal latitude, with negative being south.\n");
  printf ("Second number is a decimal longitude, with negative being west.\n");
  printf ("\n");
  printf ("In most cases, specifying a city rather than an exact location\n");
  printf ("is likely to be more convenient, and accurate enough for most\n");
  printf ("purposes.\n");
  printf ("\n");
  }


/*=======================================================================
print_sunrise_time
=======================================================================*/
void print_sunrise_time (char *text, double zenith, BOOL opt_utc, const char *tz,
    const LatLong *latlong, const DateTime *date)
  {
  char *s;
  Error *e = NULL;
  DateTime *sunrise = SunTimes_get_sunrise (latlong, date, 
      zenith, &e);
  if (e)
    {
    s = strdup (Error_get_message (e));
    Error_free (e);
    }
  else
    {
    if (opt_utc)
      s = DateTime_time_to_string_UTC (sunrise);
    else
      s = DateTime_time_to_string_local (sunrise, tz);
    }
  printf ("%s%s\n", text, s);
  free (s);
  }


/*=======================================================================
print_sunset_time
=======================================================================*/
void print_sunset_time (char *text, double zenith, BOOL opt_utc, const char *tz,
    const LatLong *latlong, const DateTime *date)
  {
  char *s;
  Error *e = NULL;
  DateTime *sunrise = SunTimes_get_sunset (latlong, date, 
      zenith, &e);
  if (e)
    {
    s = strdup (Error_get_message (e));
    Error_free (e);
    }
  else
    {
    if (opt_utc)
      s = DateTime_time_to_string_UTC (sunrise);
    else
      s = DateTime_time_to_string_local (sunrise, tz);
    }
  printf ("%s%s\n", text, s);
  free (s);
  }


/*=======================================================================
initialize_named_days
=======================================================================*/
PointerList *initialize_named_days (const char *tz, BOOL utc, int year)
  {
  return NamedDays_get_list_for_year (year, tz, utc);
  }


/*=======================================================================
initialize_moon_events
=======================================================================*/
PointerList *initialize_moon_events (const char *tz, BOOL utc, int year)
  {
  return MoonTimes_get_list_for_year (year, tz, utc);
  }


/*=======================================================================
initialize_astro_events
=======================================================================*/
PointerList *initialize_astro_events (const char *tz, BOOL utc, int year)
  {
  return AstroDays_get_list_for_year (year, tz, utc);
  }


/*=======================================================================
get_events_today
Amalgamates the various events lists into one big list. The caller should
free the list but _not_ its contents, which are just pointers to 
the events supplied in the original lists
=======================================================================*/
PointerList *get_events_today (PointerList *named_days, 
    PointerList *astro_events, PointerList *moon_events, DateTime *day)
  {
  PointerList *out = NULL;

  PointerList *list = NULL;
  list = PointerList_concatenate (list, named_days);
  list = PointerList_concatenate (list, moon_events);
  list = PointerList_concatenate (list, astro_events);

  int i, l = PointerList_get_length (list);
  for (i = 0; i < l; i++)
    {
    DateTime *event = PointerList_get_pointer (list, i);
    if (DateTime_is_same_day (day, event))
      {
      DateTime *event2 = DateTime_clone (event); 
      out = PointerList_append (out, event2);
      }
    }
  return out;
  }

/*=======================================================================
free_events
Frees a pointerlist of datetime objects
=======================================================================*/
void free_events (PointerList *events)
  {
  int i, l = PointerList_get_length (events);
  for (i = 0; i < l; i++)
    {
    DateTime_free ((DateTime *) PointerList_get_pointer (events, i));
    }
  PointerList_free (events, FALSE);
  }


/*=======================================================================
list_events
=======================================================================*/
void list_events (PointerList *events, const DateTime *start, 
     const char *tz, BOOL utc)
  {
  DateTime *d = DateTime_clone (start);
  int i;
  for (i = 0; i < 365; /* leap year? */ i++)
    {
    int i, l = PointerList_get_length (events);
    for (i = 0; i < l; i++)
      {
      DateTime *event = PointerList_get_pointer (events, i);
      if (DateTime_is_same_day_of_year (d, event))
        {
        int j;
        const char *name = DateTime_get_name (event);
        char *s = DateTime_date_to_string (event);
        printf (s);
        putchar (' ');
        for (j = strlen (s); j < 26; j++) 
          putchar (' ');
        printf (name);
        int dummy, hour, min, sec;
        DateTime_get_ymdhms (event, &dummy, &dummy, &dummy, &hour,
          &min, &sec, tz, utc);
        if (hour == 0 && min == 0 && sec == 0)
          {
          // All day event
          }
        else
          {
          printf (" (%02d:%02d)", hour, min);
          }
        putchar ('\n');
        free (s);
        }
      }
    DateTime_add_days (d, 1, tz, utc); 
    } 
  DateTime_free (d);
  }

/*=======================================================================
main
=======================================================================*/
int main (int argc, char **argv)
  {
  static BOOL opt_version = FALSE;
  static BOOL opt_help = FALSE;
  static BOOL opt_longhelp = FALSE;
  static BOOL opt_cities = FALSE;
  static BOOL opt_quiet = FALSE;
  static BOOL opt_utc = FALSE;
  static BOOL opt_full = FALSE;
  static BOOL opt_list_named_days = FALSE;
  static BOOL opt_list_moon_events = FALSE;
  static BOOL opt_list_astro_events = FALSE;
  BOOL show_sunrise_sunset = TRUE;
  BOOL show_moon_state = TRUE;
  BOOL show_moon_rise_set = TRUE;
  BOOL show_today = TRUE;
  char *datetime = NULL;
  char *tz = NULL;
  DateTime *datetimeObj = NULL;
  char *city = NULL;
  City *cityObj = NULL;
  char *latlong = NULL;
  LatLong *latlongObj = NULL;
  LatLong *workingLatlong = NULL;
  PointerList *named_days = NULL;
  PointerList *moon_events = NULL;
  PointerList *astro_events = NULL;
  static struct option long_options[] = 
    {
    {"city", required_argument, NULL, 'c'},
    {"full", no_argument, &opt_full, 'f'},
    {"cities", no_argument, &opt_cities, 0},
    {"datetime", required_argument, NULL, 'd'},
    {"help", no_argument, &opt_help, 'h'},
    {"latlong", required_argument, NULL, 'l'},
    {"longhelp", no_argument, &opt_longhelp, 0},
    {"quiet", no_argument, &opt_quiet, 'q'},
    {"utc", no_argument, &opt_utc, 'v'},
    {"version", no_argument, &opt_version, 'v'},
    {"list-astro-events", no_argument, &opt_list_astro_events, 0},
    {"list-named-days", no_argument, &opt_list_named_days, 0},
    {"list-moon-events", no_argument, &opt_list_moon_events, 0},
    {0, 0, 0, 0},
    };

  while (1)
    {
    int option_index = 0;
    int opt = getopt_long (argc, argv, "?hvc:l:d:ufq", long_options, 
      &option_index);
    if (opt == -1) break;
    switch (opt)
      {
      case 0: // Long option
        {
        if (strcmp (long_options[option_index].name, "longhelp") == 0)
          {
          opt_longhelp = TRUE;
          }
        if (strcmp (long_options[option_index].name, "utc") == 0)
          {
          opt_utc = TRUE;
          }
        if (strcmp (long_options[option_index].name, "list-named-days") == 0)
          {
          opt_list_named_days = TRUE;
          }
        if (strcmp (long_options[option_index].name, "list-moon-events") == 0)
          {
          opt_list_moon_events = TRUE;
          }
        if (strcmp (long_options[option_index].name, "list-astro-events") == 0)
          {
          opt_list_astro_events = TRUE;
          }
        else if (strcmp (long_options[option_index].name, "cities") == 0)
          {
          opt_cities = TRUE;
          }
        else if (strcmp (long_options[option_index].name, "full") == 0)
          {
          opt_full = TRUE;
          }
        else if (strcmp (long_options[option_index].name, "city") == 0)
          {
          city = strdup (optarg);
          }
        else if (strcmp (long_options[option_index].name, "latlong") == 0)
          {
          latlong = strdup (optarg);
          }
        else if (strcmp (long_options[option_index].name, "datetime") == 0)
          {
          datetime = strdup (optarg);
          }
        } // End of long options
        break;
      case 'd':
        datetime = strdup (optarg); 
        break;
      case 'c':
        city = strdup (optarg); 
        break;
      case 'l':
        latlong = strdup (optarg); 
        break;
      case 'h':
        opt_help = TRUE;
        break;
      case 'v':
        opt_version = TRUE;
        break;
      case '?':
        opt_help = TRUE;
        break;
      case 'u':
        opt_utc = TRUE;
        break;
      case 'f':
        opt_full = TRUE;
        break;
      case 'q':
        opt_quiet = TRUE;
        break;
      }
    }

  if (opt_version)
    {
    printf ("solunar version %s\n(c)2000-2012 Kevin Boone\n", VERSION);
    exit (0);
    }
  
  if (opt_cities)
    {
    print_cities();
    exit (0);
    }
  
  if (opt_help)
    {
    print_short_usage(argv[0]);
    exit (0);
    }
  
  if (opt_longhelp)
    {
    print_long_usage(argv[0]);
    exit (0);
    }
  
  if (city)
    {
    PointerList *cities = City_get_matching_name (city);
    int i, l = PointerList_get_length (cities);
   
    if (l == 0)
      {
      fprintf (stderr, 
        "No city matching \"%s\"\n\"%s --cities\" shows a list\n",
        city, argv[0]);
      PointerList_free (cities, TRUE);
      exit (-1);
      }
    
    if (l > 1)
      {
      fprintf (stderr, "Ambiguous city \"%s\". Matches:\n", city);
      for (i = 0; i < l; i++)
        {
        const char *name = PointerList_get_const_pointer (cities, i);
        printf ("  %s\n", name);
        }
      PointerList_free (cities, TRUE);
      exit (0);
      }

    // If we get here, we have been told exactly one city. 
    cityObj = City_new_from_name (PointerList_get_const_pointer (cities, 0));
    PointerList_free (cities, TRUE);
    }

  if (latlong)
    {
    if (strcmp (latlong, "help") == 0)
      {
      print_latlong_help ();
      exit (0);
      }
    Error *error = NULL;
    latlongObj = LatLong_new_parse (latlong, &error);
    if (error)
      {
      fprintf (stderr, Error_get_message (error));
      fprintf (stderr, "\n\"%s --latlong help\" for syntax.\n", argv[0]);
      Error_free (error);
      exit (-1);
      }
    }

  if (cityObj)
    {
    if (!opt_quiet)
      printf ("Selected city %s\n", cityObj->name);
    tz = cityObj->name;
    }

  if (latlongObj)
    {
    workingLatlong = LatLong_clone (latlongObj);
    if (cityObj)
      {
      if (!opt_quiet)
        printf ("Overriding city location with specified lat/long\n");
      }
    else
      {
      // No city specified -- use lat/long but don't display warning
      }
    }
  else
   {
   if (cityObj)
     {
     workingLatlong = City_get_latlong (cityObj);
     }
   else
     {
     // ...else we have no location
     }
   }

  if (workingLatlong)
    {
    if (!opt_quiet)
      {
      char *s = LatLong_to_string (workingLatlong);
      printf ("Using location %s\n", s);
      free (s);
      }
    }


  if (datetime)
    {
    if (strcmp (datetime, "help") == 0)
      {
      print_datetime_help ();
      free_events (named_days);
      free_events (moon_events);
      free_events (astro_events);
      exit (0);
      }

    Error *e = NULL;
    datetimeObj = DateTime_new_parse (datetime, &e, tz, opt_utc);
    if (e)
      {
      fprintf (stderr, Error_get_message (e));
      fprintf (stderr, "\n\"%s --datetime help\" for syntax.\n", argv[0]);
      Error_free (e);
      free_events (named_days);
      free_events (moon_events);
      free_events (astro_events);
      exit (-1);
      }
    }
  else
    datetimeObj = DateTime_new_today ();

  if (show_today || opt_list_named_days)
    {
    int year, dummy;
    DateTime_get_ymdhms (datetimeObj, &year, &dummy, &dummy, &dummy, 
      &dummy, &dummy, tz, opt_utc);
    named_days = initialize_named_days (tz, opt_utc, year);
    }

  if (show_today || opt_list_moon_events)
    {
    int year, dummy;
    DateTime_get_ymdhms (datetimeObj, &year, &dummy, &dummy, &dummy, 
      &dummy, &dummy, tz, opt_utc);
    moon_events = initialize_moon_events (tz, opt_utc, year);
    }

  if (show_today || opt_list_astro_events)
    {
    int year, dummy;
    DateTime_get_ymdhms (datetimeObj, &year, &dummy, &dummy, &dummy, 
      &dummy, &dummy, tz, opt_utc);
    astro_events = initialize_astro_events (tz, opt_utc, year);
    }


  if (!opt_quiet)
    {
    char *s;
    if (opt_utc)
      s = DateTime_to_string_UTC (datetimeObj);
    else
      s = DateTime_to_string_local (datetimeObj, tz);
    printf ("Date/time %s %s\n", s, opt_utc?"UTC":"local");
    free (s);
    printf ("\n");
    }

  if (opt_list_named_days)
    {
    if (!datetimeObj)
      {
      fprintf (stderr, 
        "Can't list days because "
        "no starting date has been specified.\n");
      free_events (named_days);
      free_events (astro_events);
      free_events (moon_events);
      exit (-1);
      }
    DateTime *jan_first = DateTime_get_jan_first (datetimeObj, tz, opt_utc);
    list_events (named_days, jan_first, tz, opt_utc);
    DateTime_free (jan_first);
    exit (0);
    }

  if (opt_list_moon_events)
    {
    if (!datetimeObj)
      {
      fprintf (stderr, 
        "Can't list events because "
        "no starting date has been specified.\n");
      free_events (named_days);
      free_events (moon_events);
      free_events (astro_events);
      exit (-1);
      }
    DateTime *jan_first = DateTime_get_jan_first (datetimeObj, tz, opt_utc);
    list_events (moon_events, jan_first, tz, opt_utc);
    DateTime_free (jan_first);
    exit (0);
    }

  if (opt_list_astro_events)
    {
    if (!datetimeObj)
      {
      fprintf (stderr, 
        "Can't list events because "
        "no starting date has been specified.\n");
      free_events (named_days);
      free_events (moon_events);
      free_events (astro_events);
      exit (-1);
      }
    DateTime *jan_first = DateTime_get_jan_first (datetimeObj, tz, opt_utc);
    list_events (astro_events, jan_first, tz, opt_utc);
    DateTime_free (jan_first);
    exit (0);
    }

  if (show_today)
    {
    if (!datetimeObj)
      {
      fprintf (stderr, 
        "Can't calculate astronomical dates because "
        "no calendar date/time has been specified.\n");
      free_events (named_days);
      free_events (moon_events);
      free_events (astro_events);
      exit (-1);
      }
    if (!opt_quiet)
      printf ("Today\n");
    char *s = DateTime_date_to_string (datetimeObj);
    printf ("                          Date: %s\n", s);
    free (s);

    PointerList *events = get_events_today 
      (named_days, astro_events, moon_events, datetimeObj);
    int i, l = PointerList_get_length (events);
    if (l > 0)
      {
      printf ("                      Today is: ");
      for (i = 0; i < l; i++)
        {
        DateTime *event = PointerList_get_pointer (events, i);
        const char *name = DateTime_get_name (event);
        if (name)
          {
          if (i != 0) printf (", ");
          printf (name);
          int dummy, hour, min, sec;
          DateTime_get_ymdhms (event, &dummy, &dummy, &dummy, &hour,
            &min, &sec, tz, opt_utc);
          if (hour == 0 && min == 0 && sec == 0)
            {
            // All day event
            }
          else
            {
            printf (" (%02d:%02d)", hour, min);
            }
          }
        }
      printf ("\n");
      }
    PointerList_free (events, FALSE);

    if (opt_full)
      {
      printf ("                   Day of year: %d\n", 
         DateTime_get_day_of_year (datetimeObj));
      printf ("                   Julian date: %.2lf\n", 
         DateTime_get_julian_date (datetimeObj));
      printf ("          Modified Julian date: %.2lf\n", 
        DateTime_get_modified_julian_date (datetimeObj));
      }
    if (!opt_quiet)
      printf ("\n");
    } 

  if (show_sunrise_sunset)
    {
    if (!workingLatlong)
      {
      fprintf (stderr, 
        "Can't calculate sunrise/set times because "
        "no location has been specified.\n");
      free_events (named_days);
      free_events (moon_events);
      free_events (astro_events);
      exit (-1);
      }
    if (!datetimeObj)
      {
      fprintf (stderr, 
        "Can't calculate sunrise/set times because "
        "no date has been specified.\n");
      exit (-1);
      }

    if (!opt_quiet)
      printf ("Sun\n");
    print_sunrise_time ("                       Sunrise: ", 
      SUNTIMES_DEFAULT_ZENITH, opt_utc, tz, 
      workingLatlong, datetimeObj); 
    print_sunset_time ("                        Sunset: ", 
      SUNTIMES_DEFAULT_ZENITH, opt_utc, tz, 
      workingLatlong, datetimeObj); 
    if (opt_full)
      {
      print_sunrise_time ("         Civil twilight starts: ", 
        SUNTIMES_CIVIL_TWILIGHT, 
        opt_utc, tz, workingLatlong, datetimeObj); 
      print_sunset_time ("           Civil twilight ends: ", 
        SUNTIMES_NAUTICAL_TWILIGHT, 
        opt_utc, tz, workingLatlong, datetimeObj); 

      print_sunrise_time ("      Nautical twilight starts: ", 
        SUNTIMES_NAUTICAL_TWILIGHT, 
        opt_utc, tz, workingLatlong, datetimeObj); 
      print_sunset_time ("        Nautical twilight ends: ", 
        SUNTIMES_NAUTICAL_TWILIGHT, 
        opt_utc, tz, workingLatlong, datetimeObj); 
      print_sunrise_time ("  Astronomical twilight starts: ", 
        SUNTIMES_ASTRONOMICAL_TWILIGHT, 
        opt_utc, tz, workingLatlong, datetimeObj); 
      print_sunset_time ("    Astronomical twilight ends: ", 
        SUNTIMES_ASTRONOMICAL_TWILIGHT, 
        opt_utc, tz, workingLatlong, datetimeObj); 
      double jd = DateTime_get_julian_date (datetimeObj);
      printf ("                  Sun distance: %.3lf AU\n", 
        Astro_get_earth_sun_distance (jd));
      }
    if (!opt_quiet)
      printf ("\n");
    }

  if (show_moon_state || TRUE)
    {
    if (!datetimeObj)
      {
      fprintf (stderr, 
        "Can't calculate moon phase because "
        "no date has been specified.\n");
      free_events (moon_events);
      free_events (astro_events);
      free_events (named_days);
      exit (-1);
      }
    if (!opt_quiet)
      printf ("Moon\n");
    if (show_moon_state)
      {
      double phase, age, distance;
      MoonTimes_get_moon_state (datetimeObj, &phase, &age, &distance); 
      const char *phase_name = MoonTimes_get_phase_name (phase);
      printf ("                    Moon phase: %.2lf %s\n", phase, phase_name);
      if (opt_full)
        {
        printf ("                      Moon age: %.1lf days\n", age);
        printf ("                 Moon distance: %.lf km\n", distance);
        }
      }
    if (show_moon_rise_set)
      {
      if (!workingLatlong)
        {
        fprintf (stderr, 
          "Can't calculate sunrise/set times because "
          "no location has been specified.\n");
        free_events (moon_events);
        free_events (named_days);
        free_events (astro_events);
        exit (-1);
        }
      int i, nevents = 0;
      DateTime *start = DateTime_get_day_start (datetimeObj);
      DateTime *end = DateTime_get_day_end (datetimeObj);
      DateTime *events[4];
      MoonTimes_get_moon_rises (workingLatlong, start, end, 
        15 * 60, events, 4, &nevents);
      for (i = 0; i < nevents; i++)
        {
        char *s = DateTime_time_to_string_local (events[i], tz);
        printf ("                      Moonrise: %s\n", s);
        free (s);
        DateTime_free (events[i]);
        }
      MoonTimes_get_moon_sets (workingLatlong, start, end, 
        15 * 60, events, 4, &nevents);
      for (i = 0; i < nevents; i++)
        {
        char *s = DateTime_time_to_string_local (events[i], tz);
        printf ("                       Moonset: %s\n", s);
        free (s);
        DateTime_free (events[i]);
        }
      DateTime_free (start);
      DateTime_free (end);
      }
    if (!opt_quiet)
      printf ("\n");
    }

  if (datetime) free (datetime);
  if (datetimeObj) DateTime_free (datetimeObj);
  if (latlong) free (latlong);
  if (latlongObj) LatLong_free (latlongObj);
  if (workingLatlong) LatLong_free (workingLatlong);
  if (city) free (city);
  if (cityObj) City_free (cityObj);
  free_events (named_days);
  free_events (moon_events);
  free_events (astro_events);

  return 0;
  }

