#include <stdafx.h>

using namespace std;
using namespace Eigen;

mvnrnd::mvnrnd(const VectorXd & meanVec, const MatrixXd & covarMat) {
	norm = normal_distribution<double>(0, 1);
	n = (int)meanVec.size();
	setCovar(covarMat);
	setMean(meanVec);
}

void mvnrnd::setCovar(const MatrixXd & covarMat) {
	// It must match up with the mean vector
	assert((covarMat.rows() == n) && (covarMat.cols() == n));
	SelfAdjointEigenSolver<MatrixXd> eigenSolver(covarMat);
	rot = eigenSolver.eigenvectors();
	scl = eigenSolver.eigenvalues();
	for (int i = 0; i < n; ++i)
		scl(i, 0) = sqrt(scl(i, 0));
}

void mvnrnd::setMean(const VectorXd & meanVec) {
	// It must match up with the covariance vector
	assert((int)meanVec.size() == n);
	mean = meanVec;
}

// Sets both the mean and covariance matrix (like calling the constructor
// again.
void mvnrnd::set(const VectorXd & meanVec, const MatrixXd & covarMat) {
	n = (int)meanVec.size();
	setCovar(covarMat);
	setMean(meanVec);
}

void mvnrnd::sample(VectorXd & buff, mt19937_64 & generator) {
	buff.resize(n);
	for (int i = 0; i < n; i++)
		buff(i, 0) = norm(generator)*scl(i, 0);
	buff = rot*buff + mean;
}

// If you will only sample once, or call mvnpdf once, then use these.
// If you will call many times, make the object so you don't recompute
// eigenvectors.
void mvnrnd::sample(VectorXd & buff, VectorXd & meanVec,
	const MatrixXd & covarMat, mt19937_64 & generator) {
	normal_distribution<double> norm(0, 1);
	int n = (int)meanVec.size();

	// Overwrite private vars
	MatrixXd rot;	// [n][n]
	VectorXd scl;	// [n][1]
	VectorXd mean;	// [n][1]

					// Set covariance matrix
	assert((covarMat.rows() == n) && (covarMat.cols() == n));	// If not, then make a new object.
	SelfAdjointEigenSolver<MatrixXd> eigenSolver(covarMat);
	rot = eigenSolver.eigenvectors();
	scl = eigenSolver.eigenvalues();
	for (int i = 0; i < n; ++i)
		scl(i, 0) = sqrt(scl(i, 0));

	// Set mean
	assert((int)meanVec.size() == n); // If not, make a new object
	mean = meanVec;

	// Sample
	buff.resize(n);
	for (int i = 0; i < n; i++)
		buff(i, 0) = norm(generator)*scl(i, 0);
	buff = rot*buff + mean;
}