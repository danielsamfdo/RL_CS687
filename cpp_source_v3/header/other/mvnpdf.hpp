#pragma once

#include <stdafx.h>

// Class for computing the Pr of outputs of a multivariate normal distribution.
// See matlab mvnpdf
class mvnpdf {
public:
	mvnpdf(const Eigen::VectorXd & meanVec, const Eigen::MatrixXd & covarMat);

	// Set the covariance matrix to a new matrix. Must be the same size as 
	// before, otherwise you should just create a new object
	void setCovar(const Eigen::MatrixXd & covarMat);

	// Set the mean vector. Must be the same length as before, otherwise you
	// should just create a new object
	void setMean(const Eigen::VectorXd & meanVec);

	// Sets both the mean and covariance matrix (like calling the constructor
	// again.
	void set(const Eigen::VectorXd & meanVec, const Eigen::MatrixXd & covarMat);

	// Get probability of the specified x
	// See http://www.cs.huji.ac.il/~csip/tirgul34.pdf
	double Pr(const Eigen::VectorXd & x);

	// If you will only sample once, or call mvnpdf once, then use these. If you
	// will call many times, make the object so you don't recompute eigenvectors
	static double Pr(const Eigen::VectorXd & x, const Eigen::VectorXd & meanVec,
		const Eigen::MatrixXd & covarMat);

private:
	int n;	// Dimension
	Eigen::VectorXd mu;
	Eigen::MatrixXd Sigma;
	Eigen::MatrixXd SigmaInv;
	double DetermSigma;
};