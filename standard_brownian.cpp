// standard_brownian.cpp
#include "xlloption.h"
#include "../fmsdual/dual.h"
#include "../fmsoption/option.h"
#include "../fmsoption/standard_brownian.h"

using namespace xll;

static AddInX xai_distribution_brownian(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_distribution_brownian"), _T("DISTRIBUTION.BROWNIAN"))
	.Arg(XLL_DOUBLEX, _T("Expiration"), IS_EXPIRATION)
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to standard Brownian motion distribution at Expriration."))
	.Documentation(
		_T("")
	)
);
HANDLEX WINAPI
xll_distribution_brownian(double t)
{
#pragma XLLEXPORT
	typedef dual::number<double,2> d2;

	handle<option::distribution_base<d2,d2,d2,d2>> h = new option::standard_brownian<d2,d2,d2,d2>(d2(t,1));

	return h.get();
}