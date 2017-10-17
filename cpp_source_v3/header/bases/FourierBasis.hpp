#pragma once

#include <stdafx.h>
#include <bases/Basis.hpp>

class FourierBasis : public Basis
{
public:
	FourierBasis(
		const int & inputDimension,
		const Eigen::VectorXd & inputLowerBound,
		const Eigen::VectorXd & inputUpperBound,
		int iOrder,
		int dOrder);
	~FourierBasis();

	int getNumOutputs() const override;
	void basify(Eigen::VectorXd & buff, const Eigen::VectorXd & x) const override;
	FourierBasis * clone() const override;

private:
	int nTerms;							// Total number of outputs
	int inputDimension;

	Eigen::MatrixXd c;					// Coefficients
	Eigen::VectorXd inputLowerBound;	// Min and max values of each input for normalization
	Eigen::VectorXd inputUpperBound;
	Eigen::ArrayXd inputRange;			// inputUpperBound - inputLowerBound. Only compute it once

};