// black.h - Fischer Black model that uses forwards and no rates.
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <cmath>
#include "normal.h"
#ifndef ensure
#define ensure(x)
#endif

namespace black {

	inline double
	d2(double f, double sigma, double k, double t)
	{
		ensure (f > 0);
		ensure (sigma > 0);
		ensure (t > 0);
		ensure (k != 0);

		double srt = sigma*sqrt(t);

		return log(f/fabs(k))/srt - srt/2;
	}

	inline double
	d1(double f, double sigma, double k, double t)
	{
		ensure (f > 0);
		ensure (sigma > 0);
		ensure (t > 0);
		ensure (k != 0);

		double srt = sigma*sqrt(t);

		return log(f/fabs(k))/srt + srt/2;
	}

	inline double
	Nd1(double f, double sigma, double k, double t)
	{
		return normal_cdf<ooura>(d1(f, sigma, k, t));
	}

	inline double
	Nd2(double f, double sigma, double k, double t)
	{
		return normal_cdf<ooura>(d2(f, sigma, k, t));
	}

	// Black call/put option price and greeks.
	// !!!Note this *increments* the pointer values!!! Handy for portfolios.
	inline double
	black(double f, double sigma, double k, double t, double* df = 0, double* ddf = 0, double* ds = 0, double* dt = 0)
	{
		ensure (f >= 0);
		ensure (sigma >= 0);
		ensure (t >= 0);

		double c = 1;
		// negative strike means put
		if (k < 0) {
			c = -1;
			k = -k;
		}

		// boundary cases
		if (f == 0 || sigma == 0 || t == 0) {
			if (df) *df += 1.*(c*f > c*k);
			if (ddf) *ddf += 0; // really delta function at k

			return c*f > c*k ? c*(f - k) : 0;
		}

		if (k == 0) {
			if (df) *df += 1;

			return f;
		}

		double srt = sigma*sqrt(t);
		double d2 = log(f/k)/srt - srt/2;
		double d1 = d2 + srt;
		double Nd1 = normal_cdf<ooura>(c*d1);
		double Nd2 = normal_cdf<ooura>(c*d2);
		double nd1(0);

		if (ddf || ds || dt)
			nd1 = normal_pdf(d1);

		if (df)
			*df += c*Nd1;

		if (ddf)
			*ddf += nd1/(f*srt);

		if (ds)
			*ds += f*srt*nd1/sigma;

		if (dt)
			*dt += -f*srt*nd1/(2*t); // negative of dv/dt

		return c*(f*Nd1 - k*Nd2);
	}

	inline double
	value(double f, double sigma, double k, double t)
	{
		return black(f, sigma, k, t);
	}
	inline double
	delta(double f, double sigma, double k, double t)
	{
		double v(0);

		black(f, sigma, k, t, &v);

		return v;
	}
	inline double
	gamma(double f, double sigma, double k, double t)
	{
		double v(0);

		black(f, sigma, k, t, 0, &v);

		return v;
	}
	inline double
	vega(double f, double sigma, double k, double t)
	{
		double v(0);

		black(f, sigma, k, t, 0, 0, &v);

		return v;
	} 
	inline double
	theta(double f, double sigma, double k, double t)
	{
		double v(0);

		black(f, sigma, k, t, 0, 0, 0, &v);

		return v;
	} 

	inline double
	implied(double f, double p, double k, double t, double s0, double eps, int max_iteration_count)
	{
		double c = 1, s1, p1;
		
		eps *= p;
		ensure (eps != 0);

		if (k < 0) {
			c = -1;
			k = -k;
		}

		// ensure price in 0 - infty vol range
		ensure (t > 0);
		ensure (p >= __max(c*(f - k),0.));
		ensure ((c == 1 && p < f) || (c == -1 && p < k));

		double p0 = black(f, s0, c*k, t) - p;

		// lucky guess
		if (fabs(p0) < eps)
			return s0;

		// bracket the root
		double m = 1.4;
		if (p0 > 0) {
			s1 = s0/m;
			p1 = black(f, s1, c*k, t) - p;
			while (p1 > 0) {
				s0 = s1;
				p0 = p1;
				s1 = s0/m;
				p1 = black(f, s1, c*k, t) - p;
			}
		}
		else {
			s1 = s0*m;
			p1 = black(f, s1, c*k, t) - p;
			while (p1 < 0) {
				s0 = s1;
				p0 = p1;
				s1 = s0*m;
				p1 = black(f, s1, c*k, t) - p;
			}
		}

		if (fabs(p1) < eps)
			return s1;

		ensure (p0*p1 < 0);

		// polish
		double ds = 0;
		double s2 = s0 - p0*(s1 - s0)/(p1 - p0);
		double p2 = black(f, s2, c*k, t, 0, 0, &ds) - p;

		// if sigma is too small use bisection
		if (ds < 1e-4) {
			while (fabs(p2) > eps) {
				if (p0*p2 < 0) {
					s1 = s2;
				}
				else {
					ensure (p1*p2 < 0);
					s0 = s2;
				}
				s2 = (s1 + s0)/2;
				p2 = black(f, s2, c*k, t) - p;
			}

			return s2;
		}

		// Newton-Raphson
		s0 = s2;
		p0 = p2;
		for (int i = 0; fabs(p0) > eps; ++i) {
			ensure (i < max_iteration_count);
			ensure (ds != 0);
			
			s1 = s0 - p0/ds;
			if (s1 < 0)
				s1 = s0/2;
			ds = 0;
			s0 = s1;
			p0 = black(f, s0, c*k, t, 0, 0, &ds) - p;
		}

		return s0;
	}

	inline double
	implied(double f, double p, double k, double t)
	{
		return implied(f, p, k, t, 0.2, 1e-10, 100);
	}

	// stochastic volatility inspired
	inline double gatheral_svi(double a, double b, double sigma, double rho, double m, double k)
	{
		return a + b*(rho*(k - m) + sqrt((k - m)*(k - m) + sigma*sigma));
	}

} // namespace black