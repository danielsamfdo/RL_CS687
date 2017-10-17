//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: my_tinv.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 06-Jun-2017 13:26:47
//

#include <stdafx.h>

// Function Declarations
static double b_eml_erfcore(double x);
static double rt_powd_snf(double u0, double u1);
static double eml_erfcore(double x);
static void gammaln(double *x);
static double betainc_cf(double x, double a, double b);
static double eml_PHIc(double z);
static double eml_betainc(double x, double a, double b, double lbeta, bool
	upper);
static double eml_gammainc(double x, double a, double la, double lgap1,
	bool upper);
static double b_eml_betaincinv(double p, double a, double b, double lbeta);
static double eml_betaincinv(double p, double a, double b, double lbeta);

// Function Definitions
//
// Arguments    : double p
//                int n
// Return Type  : double
//
double tinv(double p, int n)
{
	double t;
	double y;
	double z;
	int nIterations;
	double xn;
	double xn5;
	double xn7;
	int k;
	double b_p;
	if ((n > 0) && (0.0 <= p) && (p <= 1.0)) {
		if (p == 0.0) {
			t = -INFINITY;
		}
		else if (p == 1.0) {
			t = INFINITY;
		}
		else if (n == 1) {
			t = tan(3.1415926535897931 * (p - 0.5));
		}
		else if (n < 1000) {
			z = fabs(p - 0.5);
			if (z < 0.25) {
				y = (double)n / 2.0;
				xn = 0.5;
				gammaln(&xn);
				xn5 = y;
				gammaln(&xn5);
				xn7 = 0.5 + y;
				gammaln(&xn7);
				xn = eml_betaincinv(2.0 * z, 0.5, y, (xn + xn5) - xn7);
				z = 1.0 - xn;
			}
			else {
				y = (double)n / 2.0;
				xn = y;
				gammaln(&xn);
				xn5 = 0.5;
				gammaln(&xn5);
				xn7 = y + 0.5;
				gammaln(&xn7);
				z = b_eml_betaincinv(2.0 * z, y, 0.5, (xn + xn5) - xn7);
				xn = 1.0 - z;
			}

			if (p - 0.5 < 0.0) {
				b_p = -1.0;
			}
			else if (p - 0.5 > 0.0) {
				b_p = 1.0;
			}
			else if (p - 0.5 == 0.0) {
				b_p = 0.0;
			}
			else {
				b_p = p - 0.5;
			}

			t = b_p * sqrt(n * (xn / z));
		}
		else {
			y = 2.0 * p;
			nIterations = 1;
			if (y == 0.0) {
				xn = INFINITY;
			}
			else if (y == 2.0) {
				xn = -INFINITY;
			}
			else {
				if (y > 1.7) {
					z = sqrt(-log((2.0 - y) / 2.0));
					xn = -(((1.641345311 * z + 3.429567803) * z + -1.624906493) * z +
						-1.970840454) / ((1.6370678 * z + 3.5438892) * z + 1.0);
				}
				else if (y < 0.3) {
					z = sqrt(0.69314718055994529 - log(y));
					if (y < 1.0947644252537633E-47) {
						if (y < 7.7532508072625747E-267) {
							nIterations = 3;
						}
						else {
							nIterations = 2;
						}
					}

					xn = (((1.641345311 * z + 3.429567803) * z + -1.624906493) * z +
						-1.970840454) / ((1.6370678 * z + 3.5438892) * z + 1.0);
				}
				else {
					z = (1.0 - y) * (1.0 - y);
					xn = (1.0 - y) * (((-0.140543331 * z + 0.914624893) * z + -1.645349621)
						* z + 0.886226899) / ((((0.012229801 * z +
							-0.329097515) * z + 1.442710462) * z + -2.118377725) * z + 1.0);
				}

				if (y < 0.3) {
					for (k = 0; k <= nIterations; k++) {
						z = -(eml_erfcore(xn) - y) / (1.1283791670955126 * exp(-xn * xn));
						xn -= z / (1.0 + xn * z);
					}
				}
				else if (y > 1.7) {
					for (k = 0; k <= nIterations; k++) {
						z = (eml_erfcore(-xn) - (2.0 - y)) / (1.1283791670955126 * exp(-xn *
							xn));
						xn -= z / (1.0 + xn * z);
					}
				}
				else {
					for (k = 0; k <= nIterations; k++) {
						z = (b_eml_erfcore(xn) - (1.0 - y)) / (1.1283791670955126 * exp(-xn *
							xn));
						xn -= z / (1.0 + xn * z);
					}
				}
			}

			xn *= -1.4142135623730951;
			z = rt_powd_snf(xn, 3.0);
			xn5 = rt_powd_snf(xn, 5.0);
			xn7 = rt_powd_snf(xn, 7.0);
			t = (((xn + (z + xn) / (4.0 * n)) + ((5.0 * xn5 + 16.0 * z) + 3.0 * xn) /
				(96.0 * (n * n))) + (((3.0 * xn7 + 19.0 * xn5) + 17.0 * z) - 15.0 *
					xn) / (384.0 * rt_powd_snf(n, 3.0))) + ((((79.0 * rt_powd_snf(xn,
						9.0) + 776.0 * xn7) + 1482.0 * xn5) - 1920.0 * z) - 945.0 * xn) /
						(92160.0 * rt_powd_snf(n, 4.0));
		}
	}
	else {
		t = NAN;
	}

	return t;
}

//
// Arguments    : double x
// Return Type  : double
//
static double b_eml_erfcore(double x)
{
	double y;
	double absx;
	double S;
	double s;
	double R;
	int eint;

	// ========================== COPYRIGHT NOTICE ============================
	//  The algorithms for calculating ERF(X) and ERFC(X) are derived
	//  from FDLIBM, which has the following notice:
	//
	//  Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
	//
	//  Developed at SunSoft, a Sun Microsystems, Inc. business.
	//  Permission to use, copy, modify, and distribute this
	//  software is freely granted, provided that this notice
	//  is preserved.
	// =============================    END    ================================
	absx = fabs(x);
	if (std::isnan(x)) {
		y = x;
	}
	else if (std::isinf(x)) {
		if (x < 0.0) {
			y = -1.0;
		}
		else {
			y = 1.0;
		}
	}
	else if (absx < 0.84375) {
		if (absx < 3.7252902984619141E-9) {
			if (absx < 2.8480945388892178E-306) {
				y = 0.125 * (8.0 * x + 1.0270333367641007 * x);
			}
			else {
				y = x + 0.12837916709551259 * x;
			}
		}
		else {
			s = x * x;
			y = x + x * ((0.12837916709551256 + s * (-0.3250421072470015 + s *
				(-0.02848174957559851 + s * (-0.0057702702964894416 + s *
					-2.3763016656650163E-5)))) / (1.0 + s * (0.39791722395915535 + s *
					(0.0650222499887673 + s * (0.0050813062818757656 + s *
						(0.00013249473800432164 + s * -3.9602282787753681E-6))))));
		}
	}
	else if (absx < 1.25) {
		S = -0.0023621185607526594 + (absx - 1.0) * (0.41485611868374833 + (absx -
			1.0) * (-0.37220787603570132 + (absx - 1.0) * (0.31834661990116175 + (absx
				- 1.0) * (-0.11089469428239668 + (absx - 1.0) * (0.035478304325618236 +
				(absx - 1.0) * -0.0021663755948687908)))));
		s = 1.0 + (absx - 1.0) * (0.10642088040084423 + (absx - 1.0) *
			(0.540397917702171 + (absx - 1.0) * (0.071828654414196266 + (absx - 1.0) *
			(0.12617121980876164 + (absx - 1.0) * (0.013637083912029051 + (absx - 1.0)
				* 0.011984499846799107)))));
		if (x >= 0.0) {
			y = 0.84506291151046753 + S / s;
		}
		else {
			y = -0.84506291151046753 - S / s;
		}
	}
	else if (absx > 6.0) {
		if (x < 0.0) {
			y = -1.0;
		}
		else {
			y = 1.0;
		}
	}
	else {
		s = 1.0 / (absx * absx);
		if (absx < 2.8571434020996094) {
			R = -0.0098649440348471482 + s * (-0.69385857270718176 + s *
				(-10.558626225323291 + s * (-62.375332450326006 + s *
				(-162.39666946257347 + s * (-184.60509290671104 + s * (-81.2874355063066
					+ s * -9.8143293441691455))))));
			S = 1.0 + s * (19.651271667439257 + s * (137.65775414351904 + s *
				(434.56587747522923 + s * (645.38727173326788 + s * (429.00814002756783
					+ s * (108.63500554177944 + s * (6.5702497703192817 + s *
						-0.0604244152148581)))))));
		}
		else {
			R = -0.0098649429247001 + s * (-0.799283237680523 + s *
				(-17.757954917754752 + s * (-160.63638485582192 + s *
				(-637.56644336838963 + s * (-1025.0951316110772 + s * -483.5191916086514)))));
			S = 1.0 + s * (30.338060743482458 + s * (325.79251299657392 + s *
				(1536.729586084437 + s * (3199.8582195085955 + s * (2553.0504064331644 +
					s * (474.52854120695537 + s * -22.440952446585818))))));
		}

		if ((!std::isinf(absx)) && (!std::isnan(absx))) {
			s = frexp(absx, &eint);
		}
		else {
			s = absx;
			eint = 0;
		}

		s = floor(s * 2.097152E+6) / 2.097152E+6 * rt_powd_snf(2.0, (double)eint);
		y = exp(-s * s - 0.5625) * exp((s - absx) * (s + absx) + R / S) / absx;
		if (x < 0.0) {
			y--;
		}
		else {
			y = 1.0 - y;
		}
	}

	return y;
}

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_powd_snf(double u0, double u1)
{
	double y;
	double d0;
	double d1;
	if (std::isnan(u0) || std::isnan(u1)) {
		y = NAN;
	}
	else {
		d0 = fabs(u0);
		d1 = fabs(u1);
		if (std::isinf(u1)) {
			if (d0 == 1.0) {
				y = NAN;
			}
			else if (d0 > 1.0) {
				if (u1 > 0.0) {
					y = INFINITY;
				}
				else {
					y = 0.0;
				}
			}
			else if (u1 > 0.0) {
				y = 0.0;
			}
			else {
				y = INFINITY;
			}
		}
		else if (d1 == 0.0) {
			y = 1.0;
		}
		else if (d1 == 1.0) {
			if (u1 > 0.0) {
				y = u0;
			}
			else {
				y = 1.0 / u0;
			}
		}
		else if (u1 == 2.0) {
			y = u0 * u0;
		}
		else if ((u1 == 0.5) && (u0 >= 0.0)) {
			y = sqrt(u0);
		}
		else if ((u0 < 0.0) && (u1 > floor(u1))) {
			y = NAN;
		}
		else {
			y = pow(u0, u1);
		}
	}

	return y;
}

//
// Arguments    : double x
// Return Type  : double
//
static double eml_erfcore(double x)
{
	double y;
	double absx;
	double S;
	double s;
	double R;
	int eint;

	// ========================== COPYRIGHT NOTICE ============================
	//  The algorithms for calculating ERF(X) and ERFC(X) are derived
	//  from FDLIBM, which has the following notice:
	//
	//  Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
	//
	//  Developed at SunSoft, a Sun Microsystems, Inc. business.
	//  Permission to use, copy, modify, and distribute this
	//  software is freely granted, provided that this notice
	//  is preserved.
	// =============================    END    ================================
	absx = fabs(x);
	if (std::isnan(x)) {
		y = x;
	}
	else if (std::isinf(x)) {
		if (x < 0.0) {
			y = 2.0;
		}
		else {
			y = 0.0;
		}
	}
	else if (absx < 0.84375) {
		if (absx < 1.3877787807814457E-17) {
			y = 1.0 - x;
		}
		else {
			s = x * x;
			y = (0.12837916709551256 + s * (-0.3250421072470015 + s *
				(-0.02848174957559851 + s * (-0.0057702702964894416 + s *
					-2.3763016656650163E-5)))) / (1.0 + s * (0.39791722395915535 + s *
					(0.0650222499887673 + s * (0.0050813062818757656 + s *
						(0.00013249473800432164 + s * -3.9602282787753681E-6)))));
			if (x < 0.25) {
				y = 1.0 - (x + x * y);
			}
			else {
				y = 0.5 - (x * y + (x - 0.5));
			}
		}
	}
	else if (absx < 1.25) {
		S = -0.0023621185607526594 + (absx - 1.0) * (0.41485611868374833 + (absx -
			1.0) * (-0.37220787603570132 + (absx - 1.0) * (0.31834661990116175 + (absx
				- 1.0) * (-0.11089469428239668 + (absx - 1.0) * (0.035478304325618236 +
				(absx - 1.0) * -0.0021663755948687908)))));
		s = 1.0 + (absx - 1.0) * (0.10642088040084423 + (absx - 1.0) *
			(0.540397917702171 + (absx - 1.0) * (0.071828654414196266 + (absx - 1.0) *
			(0.12617121980876164 + (absx - 1.0) * (0.013637083912029051 + (absx - 1.0)
				* 0.011984499846799107)))));
		if (x >= 0.0) {
			y = 0.15493708848953247 - S / s;
		}
		else {
			y = 1.0 + (0.84506291151046753 + S / s);
		}
	}
	else if (x < -6.0) {
		y = 2.0;
	}
	else if (x >= 28.0) {
		y = 0.0;
	}
	else {
		s = 1.0 / (absx * absx);
		if (absx < 2.8571414947509766) {
			R = -0.0098649440348471482 + s * (-0.69385857270718176 + s *
				(-10.558626225323291 + s * (-62.375332450326006 + s *
				(-162.39666946257347 + s * (-184.60509290671104 + s * (-81.2874355063066
					+ s * -9.8143293441691455))))));
			S = 1.0 + s * (19.651271667439257 + s * (137.65775414351904 + s *
				(434.56587747522923 + s * (645.38727173326788 + s * (429.00814002756783
					+ s * (108.63500554177944 + s * (6.5702497703192817 + s *
						-0.0604244152148581)))))));
		}
		else {
			R = -0.0098649429247001 + s * (-0.799283237680523 + s *
				(-17.757954917754752 + s * (-160.63638485582192 + s *
				(-637.56644336838963 + s * (-1025.0951316110772 + s * -483.5191916086514)))));
			S = 1.0 + s * (30.338060743482458 + s * (325.79251299657392 + s *
				(1536.729586084437 + s * (3199.8582195085955 + s * (2553.0504064331644 +
					s * (474.52854120695537 + s * -22.440952446585818))))));
		}

		if ((!std::isinf(absx)) && (!std::isnan(absx))) {
			s = frexp(absx, &eint);
		}
		else {
			s = absx;
			eint = 0;
		}

		s = floor(s * 2.097152E+6) / 2.097152E+6 * rt_powd_snf(2.0, (double)eint);
		y = exp(-s * s - 0.5625) * exp((s - absx) * (s + absx) + R / S) / absx;
		if (x < 0.0) {
			y = 2.0 - y;
		}
	}

	return y;
}

//
// Arguments    : double *x
// Return Type  : void
//
static void gammaln(double *x)
{
	double t;
	double r;
	static const double table100[100] = { 0.0, 0.0, 0.69314718055994529,
		1.791759469228055, 3.1780538303479458, 4.7874917427820458,
		6.5792512120101012, 8.5251613610654147, 10.604602902745251,
		12.801827480081469, 15.104412573075516, 17.502307845873887,
		19.987214495661885, 22.552163853123425, 25.19122118273868, 27.89927138384089,
		30.671860106080672, 33.505073450136891, 36.395445208033053,
		39.339884187199495, 42.335616460753485, 45.380138898476908,
		48.471181351835227, 51.606675567764377, 54.784729398112319,
		58.003605222980518, 61.261701761002, 64.557538627006338, 67.88974313718154,
		71.257038967168015, 74.658236348830158, 78.0922235533153, 81.557959456115043,
		85.054467017581516, 88.580827542197682, 92.1361756036871, 95.7196945421432,
		99.330612454787428, 102.96819861451381, 106.63176026064346,
		110.32063971475739, 114.03421178146171, 117.77188139974507,
		121.53308151543864, 125.3172711493569, 129.12393363912722,
		132.95257503561632, 136.80272263732635, 140.67392364823425,
		144.5657439463449, 148.47776695177302, 152.40959258449735, 156.3608363030788,
		160.3311282166309, 164.32011226319517, 168.32744544842765,
		172.35279713916279, 176.39584840699735, 180.45629141754378,
		184.53382886144948, 188.6281734236716, 192.7390472878449, 196.86618167289,
		201.00931639928152, 205.1681994826412, 209.34258675253685,
		213.53224149456327, 217.73693411395422, 221.95644181913033,
		226.1905483237276, 230.43904356577696, 234.70172344281826,
		238.97838956183432, 243.26884900298271, 247.57291409618688,
		251.89040220972319, 256.22113555000954, 260.56494097186322,
		264.92164979855278, 269.29109765101981, 273.67312428569369,
		278.06757344036612, 282.4742926876304, 286.893133295427, 291.32395009427029,
		295.76660135076065, 300.22094864701415, 304.68685676566872,
		309.1641935801469, 313.65282994987905, 318.1526396202093, 322.66349912672615,
		327.1852877037752, 331.71788719692847, 336.26118197919845, 340.815058870799,
		345.37940706226686, 349.95411804077025, 354.53908551944079,
		359.1342053695754 };

	int i;
	static const double p1[8] = { 4.9452353592967269, 201.8112620856775,
		2290.8383738313464, 11319.672059033808, 28557.246356716354,
		38484.962284437934, 26377.487876241954, 7225.8139797002877 };

	static const double p2[8] = { 4.974607845568932, 542.4138599891071,
		15506.938649783649, 184793.29044456323, 1.0882047694688288E+6,
		3.33815296798703E+6, 5.1066616789273527E+6, 3.0741090548505397E+6 };

	static const double q1[8] = { 67.482125503037778, 1113.3323938571993,
		7738.7570569353984, 27639.870744033407, 54993.102062261576,
		61611.221800660023, 36351.2759150194, 8785.5363024310136 };

	static const double q2[8] = { 183.03283993705926, 7765.0493214450062,
		133190.38279660742, 1.1367058213219696E+6, 5.2679641174379466E+6,
		1.3467014543111017E+7, 1.7827365303532742E+7, 9.5330955918443538E+6 };

	static const double p4[8] = { 14745.0216605994, 2.4268133694867045E+6,
		1.2147555740450932E+8, 2.6634324496309772E+9, 2.9403789566345539E+10,
		1.7026657377653989E+11, 4.926125793377431E+11, 5.6062518562239514E+11 };

	static const double c[7] = { -0.001910444077728, 0.00084171387781295,
		-0.00059523799130430121, 0.0007936507935003503, -0.0027777777777776816,
		0.083333333333333329, 0.0057083835261 };

	static const double q4[8] = { 2690.5301758708993, 639388.56543000927,
		4.1355999302413881E+7, 1.120872109616148E+9, 1.4886137286788137E+10,
		1.0168035862724382E+11, 3.4174763455073773E+11, 4.4631581874197131E+11 };

	if (std::isnan(*x)) {
	}
	else if (*x < 0.0) {
		*x = NAN;
	}
	else if (*x > 2.55E+305) {
		*x = INFINITY;
	}
	else if (*x <= 2.2204460492503131E-16) {
		*x = -log(*x);
	}
	else if (*x <= 0.5) {
		t = 1.0;
		r = 0.0;
		for (i = 0; i < 8; i++) {
			r = r * *x + p1[i];
			t = t * *x + q1[i];
		}

		*x = -log(*x) + *x * (-0.57721566490153287 + *x * (r / t));
	}
	else if (*x <= 0.6796875) {
		t = 1.0;
		r = 0.0;
		for (i = 0; i < 8; i++) {
			r = r * ((*x - 0.5) - 0.5) + p2[i];
			t = t * ((*x - 0.5) - 0.5) + q2[i];
		}

		*x = -log(*x) + ((*x - 0.5) - 0.5) * (0.42278433509846713 + ((*x - 0.5) -
			0.5) * (r / t));
	}
	else if ((*x == floor(*x)) && (*x <= 100.0)) {
		*x = table100[(int)*x - 1];
	}
	else if (*x <= 1.5) {
		t = 1.0;
		r = 0.0;
		for (i = 0; i < 8; i++) {
			r = r * ((*x - 0.5) - 0.5) + p1[i];
			t = t * ((*x - 0.5) - 0.5) + q1[i];
		}

		*x = ((*x - 0.5) - 0.5) * (-0.57721566490153287 + ((*x - 0.5) - 0.5) * (r /
			t));
	}
	else if (*x <= 4.0) {
		t = 1.0;
		r = 0.0;
		for (i = 0; i < 8; i++) {
			r = r * (*x - 2.0) + p2[i];
			t = t * (*x - 2.0) + q2[i];
		}

		*x = (*x - 2.0) * (0.42278433509846713 + (*x - 2.0) * (r / t));
	}
	else if (*x <= 12.0) {
		t = -1.0;
		r = 0.0;
		for (i = 0; i < 8; i++) {
			r = r * (*x - 4.0) + p4[i];
			t = t * (*x - 4.0) + q4[i];
		}

		*x = 1.791759469228055 + (*x - 4.0) * (r / t);
	}
	else {
		if (*x <= 2.25E+76) {
			r = 0.0057083835261;
			t = 1.0 / (*x * *x);
			for (i = 0; i < 6; i++) {
				r = r * t + c[i];
			}

			r /= *x;
		}
		else {
			r = 0.0;
		}

		t = log(*x);
		*x = ((r + 0.91893853320467278) - 0.5 * t) + *x * (t - 1.0);
	}
}

//
// Arguments    : double x
//                double a
//                double b
// Return Type  : double
//
static double betainc_cf(double x, double a, double b)
{
	double y;
	double aplusb;
	double C;
	double Dinv;
	int m;
	int exitg1;
	double yold;
	int twom;
	double d;
	double b_y;
	aplusb = a + b;
	C = 1.0;
	Dinv = 1.0 - aplusb * x / (a + 1.0);
	y = 1.0 / Dinv;
	m = 0;
	do {
		exitg1 = 0;
		if (m < 1000) {
			yold = y;
			twom = (1 + m) << 1;
			d = (1.0 + (double)m) * (b - (1.0 + (double)m)) * x / (((a - 1.0) +
				(double)twom) * (a + (double)twom));
			b_y = d / C;
			C = 1.0 + d / C;
			Dinv = 1.0 + d / Dinv;
			y *= (1.0 + b_y) / Dinv;
			d = -(a + (1.0 + (double)m)) * (aplusb + (1.0 + (double)m)) * x / ((a +
				(double)twom) * ((a + 1.0) + (double)twom));
			C = 1.0 + d / C;
			Dinv = 1.0 + d / Dinv;
			y *= C / Dinv;
			if (fabs(y - yold) < 2.2204460492503131E-15) {
				exitg1 = 1;
			}
			else {
				m++;
			}
		}
		else {
			y = NAN;
			exitg1 = 1;
		}
	} while (exitg1 == 0);

	return y;
}

//
// Arguments    : double z
// Return Type  : double
//
static double eml_PHIc(double z)
{
	double y;
	double x;
	double t;
	x = 0.70710678118654746 * z;
	t = 3.97886080735226 / (fabs(x) + 3.97886080735226);
	y = 0.5 * ((((((((((((((((((((((0.0012710976495261409 * (t - 0.5) +
		0.00011931402283834095) * (t - 0.5) + -0.0039638509736051354) * (t - 0.5) +
		-0.00087077963531729586) * (t - 0.5) + 0.0077367252831352668) * (t - 0.5) +
		0.0038333512626488732) * (t - 0.5) + -0.012722381378212275) * (t - 0.5) +
		-0.013382364453346007) * (t - 0.5) + 0.016131532973325226) * (t - 0.5) +
		0.039097684558848406) * (t - 0.5) + 0.0024936720005350331) * (t - 0.5) +
		-0.0838864557023002) * (t - 0.5) + -0.11946395996432542)
		* (t - 0.5) + 0.016620792496936737) * (t - 0.5) +
		0.35752427444953105) * (t - 0.5) + 0.80527640875291062) *
		(t - 0.5) + 1.1890298290927332) * (t - 0.5) +
		1.3704021768233816) * (t - 0.5) + 1.313146538310231) * (t -
			0.5) + 1.0792551515585667) * (t - 0.5) + 0.77436819911953858) * (t - 0.5) +
		0.49016508058531844) * (t - 0.5) + 0.27537474159737679) * t * exp(
			-x * x);
	if (x < 0.0) {
		y = 1.0 - y;
	}

	return y;
}

//
// Arguments    : double x
//                double a
//                double b
//                double lbeta
//                bool upper
// Return Type  : double
//
static double eml_betainc(double x, double a, double b, double lbeta, bool
	upper)
{
	double rval;
	int tail;
	bool b1;
	bool guard1 = false;
	double logx;
	double log1mx;
	tail = upper;
	if (!(a >= 0.0)) {
		rval = NAN;
	}
	else if (!(b >= 0.0)) {
		rval = NAN;
	}
	else {
		if ((0.0 < x) && (x < 1.0)) {
			b1 = true;
		}
		else {
			b1 = false;
		}

		if (!b1) {
			if (x == 0.0) {
				rval = tail;
			}
			else if (x == 1.0) {
				rval = 1.0 - (double)tail;
			}
			else {
				rval = NAN;
			}
		}
		else if (a == 0.0) {
			if (b > 0.0) {
				if (x == 0.0) {
					rval = tail;
				}
				else {
					rval = 1 - tail;
				}
			}
			else if (x == 0.0) {
				rval = tail;
			}
			else if (x == 1.0) {
				rval = 1.0 - (double)tail;
			}
			else {
				rval = NAN;
			}
		}
		else if (b == 0.0) {
			if (x < 1.0) {
				rval = tail;
			}
			else {
				rval = 1 - tail;
			}
		}
		else if (a == INFINITY) {
			if (b < INFINITY) {
				if (x < 1.0) {
					rval = tail;
				}
				else {
					rval = 1 - tail;
				}
			}
			else if (x == 0.0) {
				rval = tail;
			}
			else if (x == 1.0) {
				rval = 1.0 - (double)tail;
			}
			else {
				rval = NAN;
			}
		}
		else if (b == INFINITY) {
			if (x == 0.0) {
				rval = tail;
			}
			else {
				rval = 1 - tail;
			}
		}
		else {
			guard1 = false;
			if (a + b < 1.0E+7) {
				logx = log(x);
				if (1.0 - x != 1.0) {
					log1mx = log(1.0 - x) * (-x / ((1.0 - x) - 1.0));
				}
				else {
					log1mx = -x;
				}

				if (x < (a + 1.0) / ((a + b) + 2.0)) {
					rval = exp(((a * logx + b * log1mx) - log(a)) - lbeta) * betainc_cf(x,
						a, b);
					if (upper) {
						rval = 1.0 - rval;
					}
				}
				else {
					rval = exp(((a * logx + b * log1mx) - log(b)) - lbeta) * betainc_cf
					(1.0 - x, b, a);
					if (!upper) {
						rval = 1.0 - rval;
					}
				}

				if (rval == rval) {
				}
				else {
					guard1 = true;
				}
			}
			else {
				guard1 = true;
			}

			if (guard1) {
				logx = rt_powd_snf(b * x, 0.33333333333333331);
				log1mx = rt_powd_snf(a * (1.0 - x), 0.33333333333333331);
				if (((a + b) - 1.0) * (1.0 - x) > 0.8) {
					logx = 3.0 * ((1.0 - 1.0 / (9.0 * b)) * logx - (1.0 - 1.0 / (9.0 * a))
						* log1mx) / sqrt(logx * logx / b + log1mx * log1mx / a);
					if (upper) {
						rval = eml_PHIc(logx);
					}
					else {
						rval = eml_PHIc(-logx);
					}
				}
				else {
					logx = b;
					gammaln(&logx);
					rval = eml_gammainc(0.5 * (((a + b) - 1.0) * (3.0 - x) - (b - 1.0)) *
						(1.0 - x), b, log(b), logx, !upper);
				}
			}
		}
	}

	return rval;
}

//
// Arguments    : double x
//                double a
//                double la
//                double lgap1
//                bool upper
// Return Type  : double
//
static double eml_gammainc(double x, double a, double la, double lgap1,
	bool upper)
{
	double rval;
	int tail;
	double asq;
	double stirlerr;
	static const double dv0[31] = { 0.0, 0.15342640972002736, 0.081061466795327261,
		0.054814121051917651, 0.0413406959554093, 0.033162873519936291,
		0.027677925684998338, 0.023746163656297496, 0.020790672103765093,
		0.018488450532673187, 0.016644691189821193, 0.015134973221917378,
		0.013876128823070748, 0.012810465242920227, 0.01189670994589177,
		0.011104559758206917, 0.010411265261972096, 0.0097994161261588039,
		0.0092554621827127329, 0.0087687001341393862, 0.00833056343336287,
		0.00793411456431402, 0.0075736754879518406, 0.007244554301320383,
		0.00694284010720953, 0.0066652470327076821, 0.0064089941880042071,
		0.0061717122630394576, 0.0059513701127588475, 0.0057462165130101155,
		0.0055547335519628011 };

	double t;
	double xD0;
	double old;
	double term;
	double logpax;
	int exitg4;
	double fac;
	double n;
	bool exitg1;
	double a1;
	int exitg3;
	int exitg2;
	tail = upper;
	if (!(a > 0.0)) {
		if (a == 0.0) {
			if (x == x) {
				rval = 1.0 - (double)tail;
			}
			else {
				rval = NAN;
			}
		}
		else {
			rval = NAN;
		}
	}
	else if (a == INFINITY) {
		if (x < INFINITY) {
			rval = tail;
		}
		else {
			rval = NAN;
		}
	}
	else if (!(x > 0.0)) {
		if (x == 0.0) {
			rval = tail;
		}
		else {
			rval = NAN;
		}
	}
	else if (x == INFINITY) {
		if (a < INFINITY) {
			rval = 1.0 - (double)tail;
		}
		else {
			rval = NAN;
		}
	}
	else {
		if (a <= 15.0) {
			asq = 2.0 * a;
			if (asq == floor(asq)) {
				stirlerr = dv0[(int)(asq + 1.0) - 1];
			}
			else {
				stirlerr = ((lgap1 - (a + 0.5) * la) + a) - 0.91893853320467267;
			}
		}
		else {
			asq = a * a;
			stirlerr = (0.083333333333333329 + (-0.0027777777777777779 +
				(0.00079365079365079365 + (-0.00059523809523809529 +
					0.00084175084175084171 / asq) / asq) / asq) / asq) / a;
		}

		if (fabs(a - x) > 0.1 * (a + x)) {
			t = a / x;
			if (t < 2.2250738585072014E-308) {
				xD0 = x;
			}
			else if (t > 1.7976931348623157E+308) {
				xD0 = (a * la - a * log(x)) - a;
			}
			else {
				xD0 = (a * log(t) + x) - a;
			}
		}
		else {
			t = x / a;
			asq = (1.0 - t) / (1.0 + t);
			t = asq * asq;
			xD0 = (a - x) * asq;
			old = xD0;
			term = 2.0 * (a * asq);
			asq = 3.0;
			do {
				exitg4 = 0;
				term *= t;
				xD0 += term / asq;
				if (xD0 == old) {
					exitg4 = 1;
				}
				else {
					old = xD0;
					asq += 2.0;
				}
			} while (exitg4 == 0);
		}

		logpax = (-0.5 * (1.8378770664093453 + la) - stirlerr) - xD0;
		if (x > 1.0E+6) {
			xD0 = sqrt(x);
			t = fabs((a - x) - 1.0) / xD0;
			asq = t * t;
			if (t < 15.0) {
				a1 = eml_PHIc(t) * 2.5066282746310002 * exp(0.5 * asq);
				old = (a1 * ((asq - 3.0) * t) - (asq - 4.0)) / 6.0;
				stirlerr = (a1 * ((asq * asq - 9.0) * asq + 6.0) - ((asq - 1.0) * asq -
					6.0) * t) / 72.0;
				asq = (a1 * (((((5.0 * asq + 45.0) * asq - 81.0) * asq - 315.0) * asq +
					270.0) * t) - ((((5.0 * asq + 40.0) * asq - 111.0) * asq -
						174.0) * asq + 192.0)) / 6480.0;
			}
			else {
				a1 = (1.0 + (-1.0 + (3.0 - 15.0 / asq) / asq) / asq) / t;
				old = (1.0 + (-4.0 + (25.0 - 210.0 / asq) / asq) / asq) / asq;
				stirlerr = (1.0 + (-11.0 + (130.0 - 1750.0 / asq) / asq) / asq) / (asq *
					t);
				asq = (1.0 + (-26.0 + (546.0 - 11368.0 / asq) / asq) / asq) / (asq * asq);
			}

			if (x < a - 1.0) {
				asq = a * (((a1 / xD0 - old / x) + stirlerr / (x * xD0)) - asq / (x * x));
				if (logpax < 709.782712893384) {
					rval = exp(logpax) * asq;
				}
				else {
					rval = exp(logpax + log(asq));
				}

				if (upper) {
					rval = 1.0 - rval;
				}
			}
			else {
				asq = a * (((a1 / xD0 + old / x) + stirlerr / (x * xD0)) + asq / (x * x));
				if (logpax < 709.782712893384) {
					rval = exp(logpax) * asq;
				}
				else {
					rval = exp(logpax + log(asq));
				}

				if (!upper) {
					rval = 1.0 - rval;
				}
			}
		}
		else if (upper && (x < 1.0) && (a < 1.0)) {
			fac = a * -x;
			xD0 = fac / (a + 1.0);
			n = 2.0;
			do {
				exitg3 = 0;
				fac = -x * fac / n;
				term = fac / (a + n);
				xD0 += term;
				if (fabs(term) <= fabs(xD0) * 2.2204460492503131E-16) {
					exitg3 = 1;
				}
				else {
					n++;
				}
			} while (exitg3 == 0);

			asq = a * log(x) - lgap1;
			t = exp(asq);
			if (t == 1.0) {
			}
			else if (t - 1.0 == -1.0) {
				asq = -1.0;
			}
			else {
				asq = (t - 1.0) * asq / log(t);
			}

			rval = -((xD0 + asq) + xD0 * asq);
			if (rval < 0.0) {
				rval = 0.0;
			}
		}
		else if ((x < a) || (x < 1.0)) {
			fac = x / a;
			n = 1.0;
			if (fac == INFINITY) {
				rval = 1.0 - (double)tail;
			}
			else {
				if (fac > 2.2204460492503131E-16) {
					n = 2.0;
					do {
						exitg2 = 0;
						fac = x * fac / (a + (n - 1.0));
						if (fac < 2.2204460492503131E-16) {
							exitg2 = 1;
						}
						else {
							n++;
						}
					} while (exitg2 == 0);
				}

				asq = 0.0;
				for (tail = 0; tail < (int)-(1.0 + (-1.0 - (n - 1.0))); tail++) {
					asq = x * (1.0 + asq) / (a + ((n - 1.0) + -(double)tail));
				}

				asq++;
				if (logpax < 709.782712893384) {
					rval = exp(logpax) * asq;
				}
				else {
					rval = exp(logpax + log(asq));
				}

				if (rval > 1.0) {
					rval = 1.0;
				}

				if (upper) {
					rval = 1.0 - rval;
				}
			}
		}
		else {
			fac = 1.0;
			n = 1.0;
			exitg1 = false;
			while ((!exitg1) && (n <= floor(a + x))) {
				fac = (a - n) * fac / x;
				if (fabs(fac) < 2.2204460492503131E-16) {
					exitg1 = true;
				}
				else {
					n++;
				}
			}

			if (n <= floor(a + x)) {
				asq = 1.0;
			}
			else {
				xD0 = a - floor(a);
				if (xD0 == 0.0) {
					asq = 1.0;
					n = floor(a);
				}
				else if (xD0 == 0.5) {
					asq = sqrt(3.1415926535897931 * x) * exp(x) * (2.0 * eml_PHIc(sqrt(2.0
						* x)));
					n = floor(a) + 1.0;
				}
				else {
					old = 1.0;
					a1 = x;
					stirlerr = 0.0;
					t = 1.0;
					fac = 1.0 / x;
					n = 1.0;
					asq = fac;
					term = 0.0;
					while (fabs(asq - term) > 2.2204460492503131E-16 * asq) {
						term = asq;
						asq = n - xD0;
						old = (a1 + old * asq) * fac;
						stirlerr = (t + stirlerr * asq) * fac;
						asq = n * fac;
						a1 = x * old + asq * a1;
						t = x * stirlerr + asq * t;
						fac = 1.0 / a1;
						asq = t * fac;
						n++;
					}

					asq *= x;
					n = floor(a) + 1.0;
				}
			}

			for (tail = 0; tail <= (int)n - 2; tail++) {
				asq = 1.0 + (a - ((n - 1.0) + -(double)tail)) * asq / x;
			}

			asq = asq * a / x;
			if (logpax < 709.782712893384) {
				rval = exp(logpax) * asq;
			}
			else {
				rval = exp(logpax + log(asq));
			}

			if (rval > 1.0) {
				rval = 1.0;
			}

			if (!upper) {
				rval = 1.0 - rval;
			}
		}
	}

	return rval;
}

//
// Arguments    : double p
//                double a
//                double b
//                double lbeta
// Return Type  : double
//
static double b_eml_betaincinv(double p, double a, double b, double lbeta)
{
	double rval;
	bool upper;
	bool b2;
	double pLower;
	double r;
	double y;
	double t;
	double h;
	int sgn;
	double zlo;
	double zhi;
	int i;
	bool exitg1;
	bool guard1 = false;
	bool guard2 = false;
	bool guard3 = false;
	upper = true;
	if (!(a >= 0.0)) {
		rval = NAN;
	}
	else if (!(b >= 0.0)) {
		rval = NAN;
	}
	else {
		if ((0.0 < p) && (p < 1.0)) {
			b2 = true;
		}
		else {
			b2 = false;
		}

		if (!b2) {
			if (p == 0.0) {
				rval = 1.0;
			}
			else if (p == 1.0) {
				rval = 0.0;
			}
			else {
				rval = NAN;
			}
		}
		else if (a == 0.0) {
			if (b > 0.0) {
				rval = (p < 1.0);
			}
			else if (p == 0.0) {
				rval = 1.0;
			}
			else if (p == 1.0) {
				rval = 0.0;
			}
			else {
				rval = NAN;
			}
		}
		else if (b == 0.0) {
			rval = (p == 0.0);
		}
		else if (a == INFINITY) {
			if (b < INFINITY) {
				rval = (p == 0.0);
			}
			else if (p == 0.0) {
				rval = 1.0;
			}
			else if (p == 1.0) {
				rval = 0.0;
			}
			else {
				rval = NAN;
			}
		}
		else if (b == INFINITY) {
			rval = (p < 1.0);
		}
		else {
			if (p > 0.5) {
				p = 1.0 - p;
				upper = false;
			}

			if (upper) {
				pLower = 1.0 - p;
				if (1.0 - p == 1.0) {
					pLower = 0.9999999999999778;
				}
			}
			else {
				pLower = p;
			}

			r = sqrt(-2.0 * log(pLower));
			y = r - (2.30753 + 0.27061 * r) / (1.0 + (0.99229 + 0.04481 * r) * r);
			if ((a > 1.0) && (b > 1.0)) {
				r = (y * y - 3.0) / 6.0;
				pLower = 1.0 / (2.0 * a - 1.0);
				t = 1.0 / (2.0 * b - 1.0);
				h = 2.0 / (pLower + t);
				rval = a / (a + b * exp(2.0 * (y * sqrt(h + r) / h - (t - pLower) * ((r
					+ 0.83333333333333337) - 2.0 / (3.0 * h)))));
			}
			else {
				r = 2.0 * b;
				t = 1.0 / (9.0 * b);
				t = (1.0 - t) + y * sqrt(t);
				t *= r * t * t;
				if (t > 0.0) {
					t = ((4.0 * a + r) - 2.0) / t;
					if (t > 1.0) {
						rval = 1.0 - 2.0 / (t + 1.0);
					}
					else {
						rval = exp((log(pLower * a) + lbeta) / a);
					}
				}
				else {
					rval = 1.0 - exp((log((1.0 - pLower) * b) + lbeta) / b);
				}
			}

			if (rval >= 0.0001) {
				pLower = rval;
			}
			else {
				pLower = 0.0001;
			}

			if (pLower <= 0.9999) {
				rval = pLower;
			}
			else {
				rval = 0.9999;
			}

			r = INFINITY;
			y = INFINITY;
			if (p > 1.0021E-294) {
				t = 2.2204460492503131E-14 * p;
			}
			else {
				t = 2.2251089859537388E-308;
			}

			if (upper) {
				sgn = -1;
			}
			else {
				sgn = 1;
			}

			zlo = 0.0;
			zhi = 1.0;
			i = 0;
			exitg1 = false;
			while ((!exitg1) && (i < 1000)) {
				pLower = (double)sgn * (eml_betainc(rval, a, b, lbeta, upper) - p);
				if ((pLower * r < 0.0) && (fabs(r) <= fabs(pLower))) {
					rval = 0.5 * (rval + y);
					pLower = (double)sgn * (eml_betainc(rval, a, b, lbeta, upper) - p);
				}

				if (pLower > 0.0) {
					zhi = rval;
				}
				else {
					zlo = rval;
				}

				if ((fabs(pLower) < t) || (fabs(rval - y) < 2.2204460492503131E-16 *
					rval + 2.2250738585072014E-308)) {
					exitg1 = true;
				}
				else {
					guard1 = false;
					guard2 = false;
					guard3 = false;
					if (i < 200) {
						y = rval;
						r = pLower;
						if (1.0 - rval != 1.0) {
							h = log(1.0 - rval) * (-rval / ((1.0 - rval) - 1.0));
						}
						else {
							h = -rval;
						}

						rval -= pLower / exp(((a - 1.0) * log(rval) + (b - 1.0) * h) - lbeta);
						if (rval <= zlo) {
							if (zlo == 0.0) {
								pLower = eml_betainc(2.2250738585072014E-308, a, b, lbeta, upper)
									- p;
								if (fabs((double)upper - p) < fabs(pLower)) {
									rval = 0.0;
									exitg1 = true;
								}
								else {
									zlo = 2.2250738585072014E-308;
									guard3 = true;
								}
							}
							else {
								guard3 = true;
							}
						}
						else if (rval >= zhi) {
							if (zhi == 1.0) {
								pLower = eml_betainc(0.99999999999999989, a, b, lbeta, upper) -
									p;
								if (fabs((1.0 - (double)upper) - p) < fabs(pLower)) {
									rval = 1.0;
									exitg1 = true;
								}
								else {
									zhi = 0.99999999999999989;
									guard2 = true;
								}
							}
							else {
								guard2 = true;
							}
						}
						else {
							guard1 = true;
						}
					}
					else {
						rval = 0.5 * (zlo + zhi);
						guard1 = true;
					}

					if (guard3) {
						rval = 0.99 * zlo + 0.01 * zhi;
						guard1 = true;
					}

					if (guard2) {
						rval = 0.01 * zlo + 0.99 * zhi;
						guard1 = true;
					}

					if (guard1) {
						i++;
					}
				}
			}
		}
	}

	return rval;
}

//
// Arguments    : double p
//                double a
//                double b
//                double lbeta
// Return Type  : double
//
static double eml_betaincinv(double p, double a, double b, double lbeta)
{
	double rval;
	bool upper;
	bool b0;
	double pLower;
	double r;
	double y;
	double t;
	double h;
	int sgn;
	double zlo;
	double zhi;
	int i;
	bool exitg1;
	bool guard1 = false;
	bool guard2 = false;
	bool guard3 = false;
	upper = false;
	if (!(a >= 0.0)) {
		rval = NAN;
	}
	else if (!(b >= 0.0)) {
		rval = NAN;
	}
	else {
		if ((0.0 < p) && (p < 1.0)) {
			b0 = true;
		}
		else {
			b0 = false;
		}

		if (!b0) {
			if (p == 0.0) {
				rval = 0.0;
			}
			else if (p == 1.0) {
				rval = 1.0;
			}
			else {
				rval = NAN;
			}
		}
		else if (a == 0.0) {
			if (b > 0.0) {
				rval = !(p < 1.0);
			}
			else if (p == 0.0) {
				rval = 0.0;
			}
			else if (p == 1.0) {
				rval = 1.0;
			}
			else {
				rval = NAN;
			}
		}
		else if (b == 0.0) {
			rval = !(p == 0.0);
		}
		else if (a == INFINITY) {
			if (b < INFINITY) {
				rval = !(p == 0.0);
			}
			else if (p == 0.0) {
				rval = 0.0;
			}
			else if (p == 1.0) {
				rval = 1.0;
			}
			else {
				rval = NAN;
			}
		}
		else if (b == INFINITY) {
			rval = !(p < 1.0);
		}
		else {
			if (p > 0.5) {
				p = 1.0 - p;
				upper = true;
			}

			if (upper) {
				pLower = 1.0 - p;
				if (1.0 - p == 1.0) {
					pLower = 0.9999999999999778;
				}
			}
			else {
				pLower = p;
			}

			r = sqrt(-2.0 * log(pLower));
			y = r - (2.30753 + 0.27061 * r) / (1.0 + (0.99229 + 0.04481 * r) * r);
			if ((a > 1.0) && (b > 1.0)) {
				r = (y * y - 3.0) / 6.0;
				pLower = 1.0 / (2.0 * a - 1.0);
				t = 1.0 / (2.0 * b - 1.0);
				h = 2.0 / (pLower + t);
				rval = a / (a + b * exp(2.0 * (y * sqrt(h + r) / h - (t - pLower) * ((r
					+ 0.83333333333333337) - 2.0 / (3.0 * h)))));
			}
			else {
				r = 2.0 * b;
				t = 1.0 / (9.0 * b);
				t = (1.0 - t) + y * sqrt(t);
				t *= r * t * t;
				if (t > 0.0) {
					t = ((4.0 * a + r) - 2.0) / t;
					if (t > 1.0) {
						rval = 1.0 - 2.0 / (t + 1.0);
					}
					else {
						rval = exp((log(pLower * a) + lbeta) / a);
					}
				}
				else {
					rval = 1.0 - exp((log((1.0 - pLower) * b) + lbeta) / b);
				}
			}

			if (rval >= 0.0001) {
				pLower = rval;
			}
			else {
				pLower = 0.0001;
			}

			if (pLower <= 0.9999) {
				rval = pLower;
			}
			else {
				rval = 0.9999;
			}

			r = INFINITY;
			y = INFINITY;
			if (p > 1.0021E-294) {
				t = 2.2204460492503131E-14 * p;
			}
			else {
				t = 2.2251089859537388E-308;
			}

			if (upper) {
				sgn = -1;
			}
			else {
				sgn = 1;
			}

			zlo = 0.0;
			zhi = 1.0;
			i = 0;
			exitg1 = false;
			while ((!exitg1) && (i < 1000)) {
				pLower = (double)sgn * (eml_betainc(rval, a, b, lbeta, upper) - p);
				if ((pLower * r < 0.0) && (fabs(r) <= fabs(pLower))) {
					rval = 0.5 * (rval + y);
					pLower = (double)sgn * (eml_betainc(rval, a, b, lbeta, upper) - p);
				}

				if (pLower > 0.0) {
					zhi = rval;
				}
				else {
					zlo = rval;
				}

				if ((fabs(pLower) < t) || (fabs(rval - y) < 2.2204460492503131E-16 *
					rval + 2.2250738585072014E-308)) {
					exitg1 = true;
				}
				else {
					guard1 = false;
					guard2 = false;
					guard3 = false;
					if (i < 200) {
						y = rval;
						r = pLower;
						if (1.0 - rval != 1.0) {
							h = log(1.0 - rval) * (-rval / ((1.0 - rval) - 1.0));
						}
						else {
							h = -rval;
						}

						rval -= pLower / exp(((a - 1.0) * log(rval) + (b - 1.0) * h) - lbeta);
						if (rval <= zlo) {
							if (zlo == 0.0) {
								pLower = eml_betainc(2.2250738585072014E-308, a, b, lbeta, upper)
									- p;
								if (fabs((double)upper - p) < fabs(pLower)) {
									rval = 0.0;
									exitg1 = true;
								}
								else {
									zlo = 2.2250738585072014E-308;
									guard3 = true;
								}
							}
							else {
								guard3 = true;
							}
						}
						else if (rval >= zhi) {
							if (zhi == 1.0) {
								pLower = eml_betainc(0.99999999999999989, a, b, lbeta, upper) -
									p;
								if (fabs((1.0 - (double)upper) - p) < fabs(pLower)) {
									rval = 1.0;
									exitg1 = true;
								}
								else {
									zhi = 0.99999999999999989;
									guard2 = true;
								}
							}
							else {
								guard2 = true;
							}
						}
						else {
							guard1 = true;
						}
					}
					else {
						rval = 0.5 * (zlo + zhi);
						guard1 = true;
					}

					if (guard3) {
						rval = 0.99 * zlo + 0.01 * zhi;
						guard1 = true;
					}

					if (guard2) {
						rval = 0.01 * zlo + 0.99 * zhi;
						guard1 = true;
					}

					if (guard1) {
						i++;
					}
				}
			}
		}
	}

	return rval;
}