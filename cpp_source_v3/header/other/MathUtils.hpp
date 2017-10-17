#pragma once

#include <stdafx.h>

// Sigmoid (logistic) function
double sigmoid(const double & x);

// The CDF of the normal distribution, usually written as \Phi
double normcdf(double value);

// PDF of normal distribution
double normpdf(double x, double mu, double sigma);

// erf^{-1} = inverse of the erf error function
double erfinv(double x);

// Inverse of normalcdf
double norminv(double value);

// Normalize a continuous vector (applying sigmoid if not bounded)
Eigen::VectorXd normalize(const Eigen::VectorXd & x, const std::vector<bool> & bounded, const std::pair<Eigen::VectorXd, Eigen::VectorXd> & range);

////////////////////////////////////////////////////////////////////////////////
///// Templated functions, which must be defined in header
////////////////////////////////////////////////////////////////////////////////

template <typename Scalar>
bool isFinite(const Scalar & x) {
	return ((!isnan(x)) && (!isinf(x)));
}

template <typename Scalar>
Scalar dist(const Scalar & x1, const Scalar & y1, const Scalar & x2,
	const Scalar & y2) {
	return (Scalar)std::sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

template <typename Scalar>
Scalar distSquared(const Scalar & x1, const Scalar & y1, const Scalar & x2,
	const Scalar & y2) {
	return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
}

template <typename intType>
intType ipow(const intType & a, const intType & b) {
	if (b == 0) return 1;
	if (b == 1) return a;

	intType tmp = ipow(a, b / 2);
	if (b % 2 == 0) return tmp * tmp;
	else return a * tmp * tmp;
}

/*
Floating-point modulo:
The result (the remainder) has same sign as the divisor.
Similar to matlab's mod(); Not similar to fmod() because:
Mod(-3,4)= 1
fmod(-3,4)= -3
*/
template <typename Scalar>
Scalar Mod(const Scalar & x, const Scalar & y) {
	if (0. == y) return x;
	Scalar m = x - y * (Scalar)std::floor(x / y);
	// handle boundary cases resulted from floating-point cut off:
	if (y > 0) {
		if (m >= y)
			return 0;
		if (m < 0) {
			if (y + m == y) return 0;
			else return (y + m);
		}
	}
	else
	{
		if (m <= y) return 0;
		if (m > 0) {
			if (y + m == y) return 0;
			else return (y + m);
		}
	}
	return m;
}

// wrap [rad] angle to [-PI..PI)
template <typename Scalar>
Scalar wrapPosNegPI(const Scalar & theta) {
	return Mod((Scalar)theta + M_PI, (Scalar)2.0*M_PI) - (Scalar)M_PI;
}

// wrap [rad] angle to [0..TWO_PI)
template <typename Scalar>
Scalar wrapTwoPI(const Scalar & theta) {
	return Mod((Scalar)theta, (Scalar)(2.0*M_PI));
}

// wrap [deg] angle to [-180..180)
template <typename Scalar>
Scalar wrapPosNeg180(const Scalar & theta) {
	return Mod((Scalar)(theta + 180.0), (Scalar)360.0) - (Scalar)180.0;
}

// wrap [deg] angle to [0..360)
template <typename Scalar>
Scalar wrap360(const Scalar & theta) {
	return Mod((Scalar)theta, (Scalar)360.0);
}

/*
Treat counter as a vector containing digits (actually integers). The number
represented by the digits is incremented (zeroth digit is incremented first)
*/
template <typename IntVectorType>
void incrementCounter(IntVectorType & counter, int maxDigit) {
	for (int i = 0; i < (int)counter.size(); i++) {
		counter[i]++;
		if (counter[i] > maxDigit)
			counter[i] = 0;
		else
			break;
	}
}

/*
Returns an integer from 0 to num-1, each with the probability specified in
the provided probability (vector)
*/
template <typename RealVectorType, typename generatorType>
int randp(generatorType & generator, const RealVectorType & probabilities) {
	int num = (int)probabilities.size();
	double sum = 0, r = std::uniform_real_distribution<double>(0, 1)(generator);
	for (int i = 0; i < num; i++) {
		sum += (double)probabilities[i];
		if (sum >= r) return i;
	}
	// If we get here, there was a rounding error... doh. Pick a random action
	return std::uniform_int_distribution<int>(0, num - 1)(generator);
}

// Get a random number, such that it's log is uniformly distributed
template <typename generatorType>
double logRand(generatorType & generator, const double & min, const double& max) {
	return std::exp(std::uniform_real_distribution<double>(std::log(min), std::log(max))(generator));
}

// Concatenate two STL vectors. Places b at the end of a
template <typename T>
void append(std::vector<T> & a, const std::vector<T> & b) {
	a.insert(a.end(), b.begin(), b.end());
}

// Bound x between min and max
template <typename Scalar>
Scalar bound(const Scalar & x, const Scalar & minValue,
	const Scalar & maxValue) {
	return std::min<Scalar>(maxValue, std::max<Scalar>(minValue, x));
}

// Returns an integer that is +1, 0, or -1
template <typename Scalar>
int sign(Scalar x) {
	return (x > 0) - (x < 0);
}

// cin for Eigen objects
template<typename Derived>
std::istream& operator >> (std::istream& in, Eigen::DenseBase<Derived> & m) // input
{
	for (int row = 0; row < (int)m.rows(); row++) {
		for (int col = 0; col < (int)m.cols(); col++) {
			in >> m(row, col);
		}
	}
	return in;
}

// Treat array as std::pair array with index and value. Sort by values, but
// return the index vector. That is, sort the array and return an index saying
// which indexes into the original array go into the i'th position in the array
template <typename T>
std::vector<int> sort_indexes(const std::vector<T> &v)
{
	std::vector<int> idx(v.size());
	// initialize original index locations
	for (int i = 0; i < (int)idx.size(); ++i)
		idx[i] = i;

	// sort indexes based on comparing values in v
	std::sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] < v[i2]; });
	return idx;
}