#pragma once

#include "defs.h"
#include "datetime.h"
#include "pointerlist.h"

DateTime *NamedDays_get_easter_sunday (int year, const char *tz, BOOL utc);
PointerList *NamedDays_get_list_for_year (int year, 
     const char *tz, BOOL utc);

