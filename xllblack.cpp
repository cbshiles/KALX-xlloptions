// black.cpp - Fischer Black model that uses forwards and no rates.
// Copyright (c) 2006-2009 KALX, LLC. All rights reserved. No warranty is made.
#include "xlloption.h"
#include "black.h"

using namespace black;
using namespace xll;

static AddInX xai_black_implied(
	FunctionX(XLL_DOUBLEX, _T("?xll_black_implied"), _T("BLACK.IMPLIED"))
	.Arg(XLL_DOUBLEX, _T("forward"), IS_FORWARD)
	.Arg(XLL_DOUBLEX, _T("price"), IS_PRICE)
	.Arg(XLL_DOUBLEX, _T("strike"), IS_STRIKE)
	.Arg(XLL_DOUBLEX, _T("expiration"), IS_EXPIRATION _T(". "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the implied volatility of a Black call or put option"))
	.Documentation(
		_T("The implied volatilty is that which returns <codeInline>price</codeInline> ")
		_T("from <codeInline>BLACK.VALUE</codeInline>. ")
		,
		xml::xlink(_T("BLACK.VALUE"))
	)
);
double WINAPI
xll_black_implied(double f, double p, double k, double t)
{
#pragma XLLEXPORT
	double value;

	try {
		value = black::implied(f, p, k, t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		value = std::numeric_limits<double>::quiet_NaN();
	}

	return value;
}

