#pragma once

#include <stdafx.h>

// Class for generating samples from multivariate normal distributions.
// See matlab mvnrnd - multivariate normal distribution sampling.
// Code from: 
// http://www.boost.org/doc/libs/1_57_0/doc/html/boost/variate_generator.html
class mvnrnd {
public:
	mvnrnd(const Eigen::VectorXd & meanVec, const Eigen::MatrixXd & covarMat);

	void setCovar(const Eigen::MatrixXd & covarMat);

	void setMean(const Eigen::VectorXd & meanVec);

	// Sets both the mean and covariance matrix (like calling the constructor
	// again.
	void set(const Eigen::VectorXd & meanVec, const Eigen::MatrixXd & covarMat);

	void sample(Eigen::VectorXd & buff, std::mt19937_64 & generator);

	// If you will only sample once, or call mvnpdf once, then use these.
	// If you will call many times, make the object so you don't recompute
	// eigenvectors.
	static void sample(Eigen::VectorXd & buff, Eigen::VectorXd & meanVec,
		const Eigen::MatrixXd & covarMat, std::mt19937_64 & generator);

private:
	int n;	// Dimension
	std::normal_distribution<double> norm; // standard normal distribution
	Eigen::MatrixXd rot;	// [n][n]
	Eigen::VectorXd scl;	// [n][1]
	Eigen::VectorXd mean;	// [n][1]
};