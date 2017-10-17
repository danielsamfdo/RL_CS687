#pragma once

#include <stdafx.h>

/*
Implements CMA-ES (http://en.wikipedia.org/wiki/CMA-ES). Return value is the
minimizer / maximizer

- initialMean is the starting point of the search
- initialSigma is the initial standard deviation of search around initialmean
- numIterations is the number of iterations to run before stopping
- f is the function to be optimized. The first input is the point, the second
	is any other necessary information.
- data[] Additional information to be sent to f whenever called
- minimize states whether f is to be minimized or maximized
- RNGSeed is a random number generator seed
- numThreads - if > 1, then evaluation of f is threaded
- lambda is a population size parameter that can be set automatically

This file only uses two functions from other files:
	MathUtils.hpp: vector<int> sort_indexes(const vector<T> &v)
	IOUtils.hpp: forceGetchar (this is only used by the test of CMAES, not by
		the actual CMAES implementation).
*/
Eigen::VectorXd CMAES(
	const Eigen::VectorXd & initialMean,
	const double & initialSigma,
	int numIterations,
	double(*f)(const Eigen::VectorXd &, void *[], std::mt19937_64 & gen),
	void * data[],
	bool minimize,
	const unsigned long & RNGSeed,
	int numThreads = 1,
	int lambda = -1);