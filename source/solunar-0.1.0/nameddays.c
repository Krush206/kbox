/*=======================================================================
holidays.c
Methods for getting the dates of religious and civic festival days
(mainly UK/US at present) 
=======================================================================*/
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "datetime.h"
#include "nameddays.h"
#include "pointerlist.h"


/*=======================================================================
NamedDays_get_easter_sunday
Duffet-Smith's algorithm for Easter
=======================================================================*/
DateTime *NamedDays_get_easter_sunday (int year, const char *tz, BOOL utc)
  {
	int nYear = year;

	int nA			= 0;
	int nB			= 0;
	int nC			= 0;	
	int nD			= 0;
	int nE			= 0;
	int nF			= 0;
	int nG			= 0;
	int nH			= 0;
	int nI			= 0;
	int nK			= 0;
	int nL			= 0;
	int nM			= 0;
	int nP			= 0;
	int nYY			= 0;
	int nEasterMonth	= 0;
	int nEasterDay		= 0;

	nYY = nYear;
	nA = nYear % 19;
	nB = nYear / 100;
	nC = nYear % 100;
	nD = nB / 4;
	nE = nB % 4;
	nF = (nB + 8) / 25;
	nG = (nB - nF + 1) / 3;
	nH = (19 * nA + nB - nD - nG + 15) % 30;
	nI = nC / 4;
	nK = nC % 4;
	nL = (32 + 2 * nE + 2 * nI - nH - nK) % 7;
	nM=  (nA + 11 * nH + 22 * nL) / 451;

	//  [3=March, 4=April]
	nEasterMonth = (nH + nL - 7 * nM + 114) / 31;
	--nEasterMonth;
	nP = (nH + nL - 7 * nM + 114) % 31;

	// Date in Easter Month.
	nEasterDay = nP + 1;

	// Uncorrect for our earlier correction.
	nYear -= 1900;

  return DateTime_new_dmy_name 
     (nEasterDay, nEasterMonth + 1, year, "Easter Sunday", tz, utc);
  }


/*=======================================================================
NamedDays_get_list_for_year
=======================================================================*/
PointerList *NamedDays_get_list_for_year (int year, 
     const char *tz, BOOL utc)
  {
  PointerList *l = NULL;
  DateTime *p = DateTime_new_dmy_name (1, 1, year, "New year's day", tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (2, 1, year, "Candlemas", tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (5, 1, year, "Twelveth night", tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (6, 1, year, "Epiphany", tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (25, 1, year, "Burns' night", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (14, 2, year, "St Valentine's day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (1, 3, year, "St David's day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (25, 3, year, "Lady day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (17, 3, year, "St Patrick's day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (1, 4, year, "April fools' day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (23, 4, year, "St George's day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (1, 5, year, "May day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (15, 7, year, "St Swithin's day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (31, 10, year, "Hallowe'en", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (1, 11, year, "All saints' day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (2, 11, year, "All souls' day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (5, 11, year, "Guy Fawkes' day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (11, 11, year, "Armistice' day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (30, 11, year, "St Andrew's day", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (31, 12, year, "New year's eve", 
    tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (25, 12, year, "Christmas day", tz, utc);
  l = PointerList_append (l, p);
  p = DateTime_new_dmy_name (26, 12, year, "Boxing day", tz, utc);
  l = PointerList_append (l, p);

  DateTime *easter_sunday = NamedDays_get_easter_sunday (year, tz, utc);
  l = PointerList_append (l, easter_sunday);

  DateTime *easter_monday = DateTime_clone_offset_days (easter_sunday, 1, 
   "Easter Monday", tz, utc); 
  l = PointerList_append (l, easter_monday);

  l = PointerList_append (l, DateTime_clone_offset_days (easter_sunday, -47, 
   "Shrove Tuesday", tz, utc));
  l = PointerList_append (l, DateTime_clone_offset_days (easter_sunday, -2, 
   "Good Friday", tz, utc));
  l = PointerList_append (l, DateTime_clone_offset_days (easter_sunday, -3, 
   "Maundy Thursday", tz, utc));
  l = PointerList_append (l, DateTime_clone_offset_days (easter_sunday, -7, 
   "Palm Sunday", tz, utc));
  l = PointerList_append (l, DateTime_clone_offset_days (easter_sunday, -46, 
   "Ash Wednesday", tz, utc));
  l = PointerList_append (l, DateTime_clone_offset_days (easter_sunday, 49, 
   "Whitsun/Pentecost", tz, utc));
  l = PointerList_append (l, DateTime_clone_offset_days (easter_sunday, -21, 
   "Mothering Sunday", tz, utc));

  return l;
  }

