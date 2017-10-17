#include <stdafx.h>

using namespace std;
using namespace Eigen;

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
- lambda is a population size parameter that can be set automatically

This file only uses two functions from other files:
	MathUtils.hpp: vector<int> sort_indexes(const vector<T> &v)
	IOUtils.hpp: forceGetchar (this is only used by the test of CMAES, not by
		the actual CMAES implementation).
*/
VectorXd CMAES(
	const VectorXd & initialMean,
	const double & initialSigma,
	int numIterations,
	double(*f)(const VectorXd &, void *[], mt19937_64 & gen),
	void * data[],
	bool minimize,
	const unsigned long & RNGSeed,
	int numThreads,
	int lambda)
{
	int N = (int)initialMean.size(), hsig;
	if (lambda == -1)
		lambda = 4 + (int)floor(3.0 * log(N));
	double sigma = initialSigma,
		mu = lambda / 2.0,
		eigeneval = 0;

	// number of parents/points for recombination
	double 	chiN = pow(N, 0.5)*(1.0 - 1.0 / (4.0*N) + 1.0 / (21.0*N*N));

	VectorXd xmean = initialMean, weights((int)mu);	// Initial center
	for (int i = 0; i < (int)mu; i++)
		weights[i] = i + 1;

	// muXone array for weighted recombination
	weights = log(mu + 1.0 / 2.0) - weights.array().log();

	mu = floor(mu);

	// normalize recombination weights array
	weights = weights / weights.sum();
	double mueff = weights.sum()*weights.sum() / weights.dot(weights);

	// time constant for cumulation for C
	double cc = (4.0 + mueff / N) / (N + 4.0 + 2.0*mueff / N);

	// t-const for cumulation for sigma control
	double cs = (mueff + 2.0) / (N + mueff + 5.0);

	// learning rate for rank-one update of c
	double c1 = 2.0 / ((N + 1.3)*(N + 1.3) + mueff);

	// and for rank-mu update.
	double cmu = min(1.0 - c1, 2.0*(mueff - 2.0 + 1.0 / mueff)
		/ ((N + 2.0)*(N + 2.0) + mueff));

	// damping for sigma, usually close to 1.
	double	damps = 1.0 + 2.0*max(0.0, sqrt((mueff - 1.0)
		/ (N + 1.0)) - 1.0) + cs;

	VectorXd pc = VectorXd::Zero(N),
		ps = VectorXd::Zero(N),
		D = VectorXd::Ones(N),
		DSquared = D,
		DInv = 1.0 / D.array(),
		xold,
		oneOverD;

	for (int i = 0; i < (int)DSquared.size(); i++)
		DSquared[i] *= DSquared[i];

	MatrixXd B = MatrixXd::Identity(N, N),
		C = B * DSquared.asDiagonal() * B.transpose(),
		invsqrtC = B * DInv.asDiagonal() * B.transpose(),
		arx(N, lambda),
		repmat(xmean.size(), (int)(mu + .1)),
		artmp;

	// arx(:,arindex(1:mu)) ---- Holds the columns from the best mu entries
	// according to fitness.
	MatrixXd arxSubMatrix(N, (int)(mu + .1));

	vector<double> arfitness(lambda);

	// Used later. Declare outside loop so that we can return values from it.
	vector<int> arindex;

	mt19937_64 generator(RNGSeed);
	vector<mt19937_64> threadGens(lambda);
	for (int i = 0; i < lambda; i++)
		threadGens[i].seed((unsigned long)generator());

	for (int counteval = 0; counteval < numIterations;) {
		for (int k = 0; k < lambda; k++) {
			// Load the k'th column of arx with a new point
			normal_distribution<double> distribution(0, 1);
			VectorXd randomVector(N);
			for (int i = 0; i < N; i++)
				randomVector[i] = D[i] * distribution(threadGens[k]);

			// Random vector includes the multiplication (element wise) by D.
			arx.col(k) = xmean + sigma * B * randomVector;
		}

		vector<VectorXd> fInputs(lambda);
		for (int i = 0; i < lambda; i++)
			fInputs[i] = arx.col(i);

		if (numThreads > 1) {
#ifdef _USE_MULTITHREADING
			omp_set_num_threads(std::min<int>(_MAX_THREADS, numThreads));
			#pragma omp parallel for
#endif
			for (int k = 0; k < lambda; k++) {
				// Compute the new point's fitness and store it
				arfitness[k] = (minimize ? 1 : -1)*f(fInputs[k], data, threadGens[k]);
			}
		}
		else {
			for (int k = 0; k < lambda; k++)
				arfitness[k] = (minimize ? 1 : -1)*f(fInputs[k], data, threadGens[k]); // // Compute the new point's fitness and store it
		}

		counteval += lambda;	// Update counteval

		arindex = sort_indexes(arfitness);

		xold = xmean;

		for (int col = 0; col < mu; col++)
			arxSubMatrix.col(col) = arx.col(arindex[col]);

		xmean = arxSubMatrix*weights;

		ps = (1.0 - cs)*ps + sqrt(cs*(2.0 - cs)*mueff) * invsqrtC
			* (xmean - xold) / sigma;

		hsig = (ps.norm() / sqrt(1.0 - pow(1.0 - cs, 2.0 * counteval / lambda))
			/ (double)chiN < 1.4 + 2.0 / (N + 1.0) ? 1 : 0);

		pc = (1 - cc)*pc + hsig * sqrt(cc*(2 - cc)*mueff) * (xmean - xold)
			/ sigma;

		for (int i = 0; i < repmat.cols(); i++)
			repmat.col(i) = xold;

		artmp = (1.0 / sigma) * (arxSubMatrix - repmat);

		C = (1 - c1 - cmu) * C + c1 * (pc*pc.transpose()
			+ (1 - hsig) * cc*(2 - cc) * C) + cmu * artmp
			* weights.asDiagonal() * artmp.transpose();

		sigma = sigma * exp((cs / damps)*(ps.norm() / (double)chiN - 1.0));

		if (counteval - eigeneval > lambda / (c1 + cmu) / (double)N / 10.0) {
			eigeneval = counteval;
			for (int r = 0; r < C.rows(); r++) {
				for (int c = r + 1; c < C.cols(); c++)
					C(r, c) = C(c, r);
			}
			EigenSolver<MatrixXd> es(C);	// Eigen solver for eigenvectors
			D = C.eigenvalues().real();
			B = es.eigenvectors().real();
			D = D.array().sqrt();
			for (int i = 0; i < B.cols(); i++)
				B.col(i) = B.col(i).normalized();
			oneOverD = 1.0 / D.array();
			invsqrtC = B * oneOverD.asDiagonal() * B.transpose();
		}
	}

	return arx.col(arindex[0]);
}