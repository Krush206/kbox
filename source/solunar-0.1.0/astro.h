#pragma once

double Astro_get_earth_sun_distance (double jd);
void Astro_get_solar_ra_and_dec (double jd, double *ra, double *dec);
void Astro_get_lunar_ra_and_dec (double mjd, double *ra, double *dec);
void Astro_get_moon_state (double jd, double *phase, double *age,
   double *distance);

