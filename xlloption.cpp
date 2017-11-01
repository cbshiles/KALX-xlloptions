// xlloption.cpp - Option pricing with arbitrary distributions.
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
#include "xlloption.h"
#include "../fmsdual/dual.h"
#include "../fmsoption/option.h"
#include "../fmsoption/standard_brownian.h"

using namespace option;
using namespace xll;

static AddInX xai_option(
	DocumentX(CATEGORY)
	.Documentation(
		_T("Documentation.")
	)
);

static AddInX xai_option_black_value(
	FunctionX(XLL_DOUBLEX, _T("?xll_option_black_value"), _T("OPTION.BLACK.VALUE"))
	.Arg(XLL_DOUBLEX, _T("Forward"), IS_FORWARD)
	.Arg(XLL_DOUBLEX, _T("Volatility"), IS_VOLATILITY)
	.Arg(XLL_DOUBLEX, _T("Strike"), IS_STRIKE)
	.Arg(XLL_HANDLEX, _T("Distribution"), IS_DISTRIBUTION)
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the value of a call or put using the Black model with arbitrary distribution."))
	.Documentation(
		_T("The Black formula gives the forward value of an option in terms forward price of the underlying and does not depend on interest rates. ")
		_T("The Black-Scholes/Merton present value is <codeInline>D*OPTION.BLACK.VALUE(Spot/D, Volatility, Strike, Distribution)</codeInline>, ")
		_T("where <codeInline>D</codeInline> is the discount to expiration and the underlying pays no dividends. ")
	)
);
double WINAPI
xll_option_black_value(double f, double s, double k, HANDLEX t)
{
#pragma XLLEXPORT
	typedef dual::number<double,2> d2;
	d2 result;
	
	try {
		handle<distribution_base<d2,d2,d2,d2>> t_(t);
		ensure (t_);

		result = black<d2>::value(d2(f), d2(s), d2(k), *t_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		result = std::numeric_limits<double>::quiet_NaN();
	}

	return result.derivative(0);
}

static AddInX xai_option_black_delta(
	FunctionX(XLL_DOUBLEX, _T("?xll_option_black_delta"), _T("OPTION.BLACK.DELTA"))
	.Arg(XLL_DOUBLEX, _T("Forward"), IS_FORWARD)
	.Arg(XLL_DOUBLEX, _T("Volatility"), IS_VOLATILITY)
	.Arg(XLL_DOUBLEX, _T("Strike"), IS_STRIKE)
	.Arg(XLL_HANDLEX, _T("Distribution"), IS_DISTRIBUTION)
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the delta of a call or put using the Black model with arbitrary distribution."))
	.Documentation(
		_T("The <newTerm>delta</newTerm> is the derivative of the option value with respect to the <codeInline>Forward</codeInline>. ")
	)
);
double WINAPI
xll_option_black_delta(double f, double s, double k, HANDLEX t)
{
#pragma XLLEXPORT
	typedef dual::number<double,2> d2;
	d2 result;
	
	try {
		handle<distribution_base<d2,d2,d2,d2>> t_(t);
		ensure (t_);

		result = black<d2>::delta(d2(f), d2(s), d2(k), *t_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		result = std::numeric_limits<double>::quiet_NaN();
	}

	return result.derivative(0);
}

static AddInX xai_option_black_gamma(
	FunctionX(XLL_DOUBLEX, _T("?xll_option_black_gamma"), _T("OPTION.BLACK.GAMMA"))
	.Arg(XLL_DOUBLEX, _T("Forward"), IS_FORWARD)
	.Arg(XLL_DOUBLEX, _T("Volatility"), IS_VOLATILITY)
	.Arg(XLL_DOUBLEX, _T("Strike"), IS_STRIKE)
	.Arg(XLL_HANDLEX, _T("Distribution"), IS_DISTRIBUTION)
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the gamma of a call or put using the Black model with arbitrary distribution."))
	.Documentation(
		_T("The <newTerm>gamma</newTerm> is the second derivative of the option value with respect to the <codeInline>Forward</codeInline>. ")
	)
);
double WINAPI
xll_option_black_gamma(double f, double s, double k, HANDLEX t)
{
#pragma XLLEXPORT
	typedef dual::number<double,2> d2;
	d2 result;
	
	try {
		handle<distribution_base<d2,d2,d2,d2>> t_(t);
		ensure (t_);

		result = black<d2>::delta(d2(f,1), d2(s), d2(k), *t_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		result = std::numeric_limits<double>::quiet_NaN();
	}

	return result.derivative(1);
}

static AddInX xai_option_black_vega(
	FunctionX(XLL_DOUBLEX, _T("?xll_option_black_vega"), _T("OPTION.BLACK.VEGA"))
	.Arg(XLL_DOUBLEX, _T("Forward"), IS_FORWARD)
	.Arg(XLL_DOUBLEX, _T("Volatility"), IS_VOLATILITY)
	.Arg(XLL_DOUBLEX, _T("Strike"), IS_STRIKE)
	.Arg(XLL_HANDLEX, _T("Distribution"), IS_DISTRIBUTION)
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the vega of a call or put using the Black model with arbitrary distribution."))
	.Documentation(
		_T("The <newTerm>vega</newTerm> is the derivative of the option value with respect to the <codeInline>Volatility</codeInline>. ")
	)
);
double WINAPI
xll_option_black_vega(double f, double s, double k, HANDLEX t)
{
#pragma XLLEXPORT
	typedef dual::number<double,2> d2;
	d2 result;
	
	try {
		handle<distribution_base<d2,d2,d2,d2>> t_(t);
		ensure (t_);

		result = black<d2>::value(d2(f), d2(s,1), d2(k), *t_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		result = std::numeric_limits<double>::quiet_NaN();
	}

	return result.derivative(1);
}

// theta not working since t is scalar in distribution

#ifdef _DEBUG

int
xll_test_option()
{
	try {
		double x(0);
		ensure (x == 0);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenX> xao_test_option(xll_test_option);

#endif // _DEBUG