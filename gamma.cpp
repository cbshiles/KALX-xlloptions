// gamma.cpp
#include "xlloption.h"
#include "../fmsdual/dual.h"
#include "../fmsoption/option.h"
#include "../fmsoption/gamma.h"

using namespace xll;

static AddInX xai_distribution_gamma(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_distribution_gamma"), _T("DISTRIBUTION.GAMMA"))
	.Arg(XLL_DOUBLEX, _T("Expiration"), IS_EXPIRATION)
	.Arg(XLL_DOUBLEX, _T("Alpha"), _T("is the shape parameter."))
	.Arg(XLL_DOUBLEX, _T("Beta"), _T("is the inverse scale parameter. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a gamma distribution at Expiration."))
	.Documentation(
		_T("The shape and scale parameters are normalized so that when ") ENT_alpha _T(" and ") ENT_beta _T(" are 1 ")
		_T("the distribution at time 1 has the same mean and variance as <math>exp(B<subscript>1</subscript>)</math>, ")
		_T("where <math>B<subscript>1</subscript></math> is normal with mean 0 and variance 1. ")
	)
);
HANDLEX WINAPI
xll_distribution_gamma(double t, double a, double b)
{
#pragma XLLEXPORT
	typedef dual::number<double,2> d2;

	if (a == 0)
		a = 1;
	if (b == 0)
		b = 1;

	handle<option::distribution_base<d2,d2,d2,d2>> h = new option::gamma<d2,d2,d2,d2>(d2(t), a, b);

	return h.get();
}