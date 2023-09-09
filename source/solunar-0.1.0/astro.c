/*=======================================================================
astrodays.c
Methods for fundamental astronomical constants
=======================================================================*/

#include <math.h>
#include "roundutil.h" 
#include "trigutil.h" 

static double DegRad = M_PI / 180.0;

const double Astro_moon_epoch  = 2444238.5;

/*
* Constants defining the Sun's apparent orbit
*
* Ecliptic longitude of the Sun at epoch 1980.0
*/
const double Astro_elonge = 278.833540;
/*
* Ecliptic longitude of the Sun at perigee
*/
const double Astro_elongp = 282.596403;
/*
* Eccentricity of Earth's orbit
*/
const double Astro_eccent = 0.016718;
/*
* Semi-major axis of Earth's orbit, km
*/
const double Astro_sunsmax = 1.495985e8;
/*
* Sun's angular size, degrees, at semi-major axis distance
*/
const double Astro_sunangsiz = 0.533128;
/*
* Moon mean longitude at epoch
*/
double Astro_mmlong = 64.975464;
/*
* Mean longitude of the perigee at the epoch
*/
double Astro_mmlongp = 349.383063;
/*
* Mean longitude of the node at the epoch
*/
double Astro_mlnode = 151.950429;
/*
* Inclination of the Moon's orbit
*/
double Astro_minc = 5.145396;
/*
* Eccentricity of the Moon's orbit
*/
double Astro_mecc = 0.054900;
/*
* Moon's angular size at distance a from Earth
*/
double Astro_mangsiz = 0.5181;
/*
* Semi-major axis of Moon's orbit in km
*/
double Astro_msmax = 384401.0;
/*
* Parallax at distance a from Earth
*/
double Astro_mparallax = 0.9507;
/*
* Synodic month (new Moon to new Moon)
*/
double Astro_synmonth = 29.53058868;
/*
* Base date for E. W. Brown's numbered series of lunations (1923 Januar
y 16)
*/
double Astro_moonRad = 1737.4;
double Astro_lunatbase = 2423436.0;
/*
* Properties of the Earth
* Radius of Earth in kilometres
*/
double Astro_earthrad = 6378.16;

/*
* Limiting parameter for the Kepler equation.
*/
double Astro_kEpsilon = 1.0e-6;


/*=======================================================================
Astro_kepler
used by get_moon_state
=======================================================================*/
static double kepler (double m, double ecc)
{
  m *= DegRad;
  double delta, e = m;
  for (delta = 1.0; fabs(delta) > Astro_kEpsilon;)
        {
                delta = e - ecc * sin(e) - m;
                e -= delta / (1 - ecc * cos(e));
        }
  return e;
}


/*=======================================================================
Astro_get_moon_state
Get phase, age, and distance of moon at specified data/time
=======================================================================*/
void Astro_get_moon_state (double jd, double *phase, double *age,
   double *distance)
{
  double day = jd - Astro_moon_epoch;
 
  // Mean anomaly of the Sun   
  double N = fixAngle((360.0 / 365.2422) * day);                  
  // Convert from perigee co-ordinates to epoch 1980.0 
  double M = fixAngle (N + Astro_elonge - Astro_elongp);
  double Ec = kepler (M, Astro_eccent);

  // Solve equation of Kepler 
  Ec = sqrt((1.0 + Astro_eccent) / (1.0 - Astro_eccent)) * tan(Ec / 2.0);
  Ec = 2.0 * atan(Ec) / DegRad; // True anomaly in degrees 

  // Sun's geocentric ecliptic * longitude
  double Lambdasun = fixAngle(Ec + Astro_elongp);
  // Orbital distance factor 
  //double F = ((1.0 + Astro_eccent * cosDeg(Ec)) / (1.0 - Astro_eccent * MoonTimes_eccent));

  // Distance to Sun in km
  //double SunDist = Astro_sunsmax / F;
  // Sun's angular size in degrees 
  //double SunAng = F * Astro_sunangsiz;

  // Calculation of the Moon's position 

  // Moon's mean longitude
  double ml = fixAngle(13.1763966 * day + Astro_mmlong);        
  // Moon's mean anomaly
  double MM = fixAngle(ml - 0.1114041 * day - Astro_mmlongp);                 // Ascending node mean anomaly
  //double MN = fixAngle(Astro_mlnode - 0.0529539 * day);                 
  // Evection
  double Ev = 1.2739 * sinDeg(2.0 * (ml - Lambdasun) - MM);     
  // Annual equation
  double Ae = 0.1858 * sinDeg(M);
  double A3 = 0.37 * sinDeg(M);
  // Correction term 
  double MmP = MM + Ev - Ae - A3;
  // Corrected anomaly 
  double mEc = 6.2886 * sinDeg (MmP);
  // Correction for center equation
  double A4 = 0.214 * sinDeg(2 * MmP);
  // Yet another correction term 
  double lP = ml + Ev + mEc - Ae + A4;
  // Corrected longitude
  double V = 0.6583 * sinDeg(2 * (lP - Lambdasun));
  // Variation     
  double lPP = lP + V;
  // True longitude 
  //double NP = MN - 0.16 * sinDeg(M);

/*
  double x = cosDeg (lPP - NP);
  double y = sinDeg (lPP - NP);
  double Lambdamoon = atan2Deg (y, x);
  Lambdamoon += NP;
  double BetaM = asin (sinDeg (lPP - NP) * sinDeg (Astro_minc)) / DegRad;
*/

  /* Calculation of the phase of the Moon */

  // Moon age in degrees   
  double MoonAgeDegrees = lPP - Lambdasun;
  //double MoonPhase = (1.0 - cosDeg(MoonAgeDegrees)) / 2.0;  
  // Distance from earth ctr
  double MoonDist = (Astro_msmax * 
    (1.0 - Astro_mecc * Astro_mecc)) /
       (1.0 + Astro_mecc * cosDeg(MmP + mEc));

  // Moon diameter fraction
  //double MoonDFrac = MoonDist / Astro_msmax;
  // Moon angular diameter 
  //double MoonAng = Astro_mangsiz / MoonDFrac;
  //double MoonPar = Astro_mparallax / MoonDFrac; // Moon parallax 
  double MoonFraction = fixAngle (MoonAgeDegrees) / 360.0;
  *phase = MoonFraction;
  *age = MoonFraction * Astro_synmonth;
  *distance = MoonDist;
}

/*=======================================================================
Astro_get_lunar_ra_and_dec
Get right ascension and declination of moon at specified date/time
=======================================================================*/
void Astro_get_lunar_ra_and_dec (double jd, double *_ra, double *_dec)
{
  const double CosEPS = 0.91748;
  const double SinEPS = 0.39778;
  const double ARC = 206264.8062;

  const double P2 = M_PI * 2.0;
  const double JD = jd; 
  double t = (JD - 2451545.0)/36525.0;

  double L0 = roundutil_pascalFrac(0.606433 + 1336.855225 * t);  // mean longitude of moon
  double L = P2 * roundutil_pascalFrac(0.374897 + 1325.552410 * t); //mean anomaly of Moon
  double LS = P2 * roundutil_pascalFrac(0.993133 + 99.997361 * t); //mean anomaly of Sun
  double D = P2 * roundutil_pascalFrac(0.827361 + 1236.853086 * t); //diff in longitude of moon and sun
  double F = P2 * roundutil_pascalFrac(0.259086 + 1342.227825 * t); //mean argument of latitude

  double DL =  22640 * sin(L)  -4586 * sin(L - 2*D) +2370 * sin(2*D);
  DL +=  +769 * sin(2*L)  -668 * sin(LS) -412 * sin(2*F);
  DL +=  -212 * sin(2*L - 2*D) -206 * sin(L + LS - 2*D);
  DL +=  +192 * sin(L + 2*D) -165 * sin(LS - 2*D);
  DL +=  -125 * sin(D) -110 * sin(L + LS) +148 * sin(L - LS);
  DL +=   -55 * sin(2*F - 2*D);

  double S = F + (DL + 412 * sin(2*F) + 541* sin(LS)) / ARC;
  double H = F - 2*D;
  double N =   -526 * sin(H)+44 * sin(L + H) -31 * sin(-L + H);
  N +=   -23 * sin(LS + H) +11 * sin(-LS + H) -25 * sin(-2*L + F);
  N +=   +21 * sin(-L + F);

  double L_moon = P2 * roundutil_pascalFrac(L0 + DL / 1296000);
  double B_moon = (18520.0 * sin(S) + N) /ARC;
  double CB = cos(B_moon);
  double X = CB * cos(L_moon);
  double V = CB * sin(L_moon);
  double W = sin(B_moon);
  double Y = CosEPS * V - SinEPS * W;
  double Z = SinEPS * V + CosEPS * W;
  double RHO = sqrt(1.0 - Z*Z);
  double dec = (360.0 / P2) * atan(Z / RHO);
  double ra = (48.0 / P2) * atan(Y / (X + RHO));

  if (ra < 0) ra += 24 ;

  *_ra = ra;
  *_dec = dec;
}

/*=======================================================================
Astro_R0
see get_earth_sun_distance
=======================================================================*/
double Astro_R0 (double t)
  {
  static double LArray [40][3] =
    {
    {100013989, 0, 0},
    {1670700, 3.0984635, 6283.0758500},
    {13956, 3.05525, 12566.15170},
    {3084, 5.1985, 77713.7715},
    {1628, 1.1739, 5753.3849},
    {1576, 2.8469, 7860.4194},
    {925, 5.453, 11506.770},
    {542, 4.564, 3930.210},
    {472, 3.661, 5884.927},
    {346, 0.964, 5507.553},
    {329, 5.900, 5223.694},
    {307, 0.299, 5573.143},
    {243, 4.273, 11790.629},
    {212, 5.847, 1577.344},
    {186, 5.022, 10977.079},
    {175, 3.012, 18849.228},
    {110, 5.055, 5486.778},
    {98, 0.89, 6069.78},
    {86, 5.69, 15720.84},
    {86, 1.27, 161000.69},
    {65, 0.27, 17260.15},
    {63, 0.92, 529.69},
    {57, 2.01, 83996.85},
    {56, 5.24, 71430.70},
    {49, 3.25, 2544.31},
    {47, 2.58, 775.52},
    {45, 5.54, 9437.76},
    {43, 6.01, 6275.96},
    {39, 5.36, 4694.00},
    {38, 2.39, 8827.39},
    {37, 0.83, 19651.05},
    {37, 4.90, 12139.55},
    {36, 1.67, 12036.46},
    {35, 1.84, 2942.46},
    {33, 0.24, 7084.90},
    {32, 0.18, 5088.63},
    {32, 1.78, 398.15},
    {28, 1.21, 6286.60},
    {28, 1.90, 6279.55},
    {26, 4.59, 10447.39}
    };

  double r = 0.0;
  int i;
  for (i = 0; i < 40; i++)
    r += LArray[i][0] * cos (LArray[i][1] + LArray[i][2]*t);
  return r;
  }


/*=======================================================================
Astro_R1
see get_earth_sun_distance
=======================================================================*/
double Astro_R1 (double t)
  {
  static double LArray [10][3] =
    {
    {103019, 1.107490, 6283.075850},
    {1721, 1.0644, 12566.1517},
    {702, 3.142, 0},
    {32, 1.02, 18849.23},
    {31, 2.84, 5507.55},
    {25, 1.32, 5223.69},
    {18, 1.42, 1577.34},
    {10, 5.91, 10977.08},
    {9, 1.42, 6275.96},
    {9, 0.27, 5486.78}
    };

  int i;
  double r = 0.0;
  for (i = 0; i < 10; i++)
    r += LArray[i][0] * cos (LArray[i][1] + LArray[i][2]*t);
  return r;
  }


/*=======================================================================
Astro_R2
see get_earth_sun_distance
=======================================================================*/
double Astro_R2 (double t)
  {
  static double LArray [6][3] =
    {
    {4359, 5.7846, 6283.0758},
    {124, 5.579, 12566.152},
    {12, 3.14, 0},
    {9, 3.63, 77713.77},
    {6, 1.87, 5573.14},
    {3, 5.47, 18849.23}
    };

  int i;
  double r = 0.0;
  for (i = 0; i < 6; i++)
    r += LArray[i][0] * cos (LArray[i][1] + LArray[i][2]*t);
  return r;
  }


/*=======================================================================
Astro_R3
see get_earth_sun_distance
=======================================================================*/
double Astro_R3 (double t)
  {
  static double LArray [2][3] =
    {
    {145, 4.273, 6283.076},
    {7, 3.92, 12566.15}
    };

  int i;
  double r = 0.0;
  for (i = 0; i < 2; i++)
    r += LArray[i][0] * cos (LArray[i][1] + LArray[i][2]*t);
  return r;
  }


/*=======================================================================
Astro_R4
see get_earth_sun_distance
=======================================================================*/
double Astro_R4 (double t)
  {
  static double LArray [1][3] =
    {
    {4, 2.56, 6283.08}
    };

  int i;
  double r = 0.0;
  for (i = 0; i < 1; i++)
    r += LArray[i][0] * cos (LArray[i][1] + LArray[i][2]*t);
  return r;
  }


/*=======================================================================
Astro_get_earth_sun_distance
Meeus' method for the earth-sun distance, using a series approximation
based on the millenia
In: a date/time as a Julian data
Out: a distance in AU
=======================================================================*/
double Astro_get_earth_sun_distance (double jd)
  {
  double T = (jd - 2451545.0)/365250.0;
  double R = (Astro_R0 (T) + Astro_R1(T)*T + Astro_R2(T)*T*T 
    + Astro_R3(T)*T*T*T + Astro_R4(T)*T*T*T*T) / 1.0E8;
  return R;
  }


/*=======================================================================
Astro_get_solar_ra_and_dec
Get right ascension and declination of sun at specified date/time
=======================================================================*/
void Astro_get_solar_ra_and_dec (double JD, double *ra, double *dec)
{
	const double CosEPS = 0.91748;
	const double SinEPS = 0.39778;
	//double JD = MJD + 2400000.5; 
	double T = (JD - 2451545.0)/36525.0;
	double P2 = M_PI * 2.0;
	double M		= P2 * roundutil_pascalFrac(0.993133 + 99.997361 * T);
	double DL		= 6893.0 * sin(M) + 72.0 * sin(M * 2.0);
	double L		= P2 * roundutil_pascalFrac(
						0.7859453
						+ M / P2
						+ (6191.2 * T + DL) / 1296e3
					);
	double SL		= sin(L);
	double X		= cos(L);
	double Y		= CosEPS * SL;
	double Z		= SinEPS * SL;
	double RHO		= sqrt(1.0 - Z * Z);
	*dec		= (360.0 / P2) * atan2(Z, RHO);
	*ra		= ( 48.0 / P2) * atan2(Y, (X + RHO));
	if (*ra < 0.0)
	{
			*ra += 24.0;
	}
}


