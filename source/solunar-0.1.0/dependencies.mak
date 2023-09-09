main.o: main.c defs.h city.h cityinfo.h pointerlist.h error.h datetime.h latlong.h suntimes.h nameddays.h astrodays.h
pointerlist.o: pointerlist.c pointerlist.h defs.h
city.o: city.c city.h cityinfo.h defs.h
latlong.o: latlong.c latlong.h error.h defs.h
error.o: error.c defs.h
datetime.o: datetime.c latlong.c datetime.h error.h defs.h
suntimes.o: suntimes.c suntimes.h datetime.h latlong.h defs.h trigutil.h roundutil.h timeutil.h
moontimes.o: moontimes.c moontimes.h datetime.h latlong.h defs.h trigutil.h roundutil.h timeutil.h
timeutil.o: timeutil.c timeutil.h
roundutil.o: roundutil.c roundutil.h
trigutil.o: trigutil.c trigutil.h
mathutil.o: mathutil.c mathutil.h
nameddays.o: defs.h nameddays.h datetime.h 
astrodays.o: defs.h astrodays.h datetime.h astrodays.c astro.h
astro.o: defs.h astro.h astro.c 

