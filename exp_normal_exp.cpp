// exp_normal_exp.cpp Exponential-normal-exponential model.
#include "xlloption.h"
#include "../fmsdual/dual.h"
#include "../fmsoption/option.h"
#include "../fmsoption/exp_normal_exp.h"

using namespace xll;

static AddInX xai_distribution_exp_normal_exp(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_distribution_exp_normal_exp"), _T("DISTRIBUTION.EXP.NORMAL.EXP"))
	.Arg(XLL_DOUBLEX, _T("Expiration"), IS_EXPIRATION)
	.Arg(XLL_DOUBLEX, _T("a"), _T("is the negative moneyness."))
	.Arg(XLL_DOUBLEX, _T("alpha"), _T("is the negative hazard rate."))
	.Arg(XLL_DOUBLEX, _T("b"), _T("is the positive moneyness."))
	.Arg(XLL_DOUBLEX, _T("beta"), _T("is the positive hazard rate."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to standard the exponential-normal-exponential distribution at Expriration."))
	.Documentation(
		_T("")
	)
);
HANDLEX WINAPI
xll_distribution_brownian(double t, double a, double alpha, double b, double beta)
{
#pragma XLLEXPORT
	typedef dual::number<double,2> d2;

	handle<option::distribution_base<d2,d2,d2,d2>> h = new option::exp_normal_exp<d2,d2,d2,d2>(d2(t,1), a, alpha, b, beta);

	return h.get();
}