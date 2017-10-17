#include <stdafx.h>

using namespace std;
using namespace Eigen;

// Sigmoid (logistic) function
double sigmoid(const double & x) {
	return 1.0 / (1.0 + (double)exp(-x));
}

// The CDF of the normal distribution, usually written as \Phi
double normcdf(double value) {
	return 0.5 * erfc(-value * M_SQRT1_2);
}

// PDF of normal distribution
double normpdf(double x, double mu, double sigma) {
	double temp = x - mu;
	return 1.0 / (sigma*sqrt(2.0*M_PI))*exp(-(temp*temp) / (2.0*sigma*sigma));
}

// erf^{-1} = inverse of the erf error function
double erfinv(double x) {
	double x2, r, y;
	int  sign_x;
	if (x < -1 || x > 1)
		return NAN;
	if (x == 0)
		return 0;
	if (x > 0)
		sign_x = 1;
	else
	{
		sign_x = -1;
		x = -x;
	}
	if (x <= 0.7)
	{
		x2 = x * x;
		r =
			x * (((-0.140543331 * x2 + 0.914624893) * x2 + -1.645349621) * x2 + 0.886226899);
		r /= (((0.012229801 * x2 + -0.329097515) * x2 + 1.442710462) * x2 +
			-2.118377725) * x2 + 1.0;
	}
	else
	{
		y = sqrt(-log((1 - x) / 2));
		r = (((1.641345311 * y + 3.429567803) * y + -1.62490649) * y + -1.970840454);
		r /= ((1.637067800 * y + 3.543889200) * y + 1.0);
	}
	r = r * sign_x;
	x = x * sign_x;
	r -= (erf(r) - x) / (2 / sqrt(M_PI) * exp(-r * r));
	r -= (erf(r) - x) / (2 / sqrt(M_PI) * exp(-r * r));
	return r;
}

// Inverse of normalcdf
double norminv(double value) {
	assert(value > 0);
	assert(value < 1);
	return M_SQRT2*erfinv(2.0*value - 1.0);
}

VectorXd normalize(const VectorXd & x, const vector<bool> & bounded, const pair<VectorXd, VectorXd> & range)
{
	VectorXd result(x.size());
	for (int i = 0; i < (int)x.size(); i++)
	{
		if (bounded[i])
			result[i] = (x[i] - range.first[i]) / (range.second[i] - range.first[i]);
		else
			result[i] = sigmoid(x[i]);
	}
	return result;
}
