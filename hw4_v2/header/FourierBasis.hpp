#pragma once

#include <stdafx.hpp>

// Class for applying the Fourier basis to a state to provide the resulting features.
class FourierBasis
{
public:
	FourierBasis(
		const int & inputDimension,
		const Eigen::VectorXd & inputLowerBound,
		const Eigen::VectorXd & inputUpperBound,
		int iOrder,
		int dOrder,
		bool useIdentityBasis = false);
	~FourierBasis();

	int getNumOutputs() const;

	// Convers x into the Fourier basis features, and stores the result in buff
	void basify(Eigen::VectorXd & buff, const Eigen::VectorXd & x) const;

private:
	int nTerms;							// Total number of outputs
	int inputDimension;
	bool useIdentityBasis;

	Eigen::MatrixXd c;					// Coefficients
	Eigen::VectorXd inputLowerBound;	// Min and max values of each input for normalization
	Eigen::VectorXd inputUpperBound;
	Eigen::ArrayXd inputRange;			// inputUpperBound - inputLowerBound. Only compute it once
};