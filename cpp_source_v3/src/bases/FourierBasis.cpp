#include <stdafx.h>
#include <bases/FourierBasis.hpp>

using namespace std;
using namespace Eigen;

FourierBasis::FourierBasis(
	const int & inputDimension,
	const VectorXd & inputLowerBound,
	const VectorXd & inputUpperBound,
	int iOrder,
	int dOrder)
{
	// Copy over the provided arguments
	this->inputDimension = inputDimension;
	this->inputLowerBound = inputLowerBound;
	this->inputUpperBound = inputUpperBound;

	// Compute the total number of terms
	int iTerms = iOrder*inputDimension;						// Number of independent terms
	int dTerms = ipow(dOrder + 1, inputDimension);			// Number of dependent terms
	int oTerms = min(iOrder, dOrder)*inputDimension;			// Overlap of iTerms and dTerms
	nTerms = iTerms + dTerms - oTerms;

	// Initialize c
	ensure(inputDimension > 0, "FourierBasis::FourierBasis", "Requires at least one input dimension");
	c.resize(nTerms, inputDimension);
	VectorXi counter(inputDimension);
	// First add the dependent terms
	counter.setZero();							        // Set all terms to zero in the counter (which counts in base dOrder
	int termCount = 0;
	for (; termCount < dTerms; termCount++)
	{
		for (int i = 0; i < inputDimension; i++)
			c(termCount, i) = (float)counter(i);
		incrementCounter(counter, dOrder);
	}
	// Add the independent terms
	for (int i = 0; i < inputDimension; i++)
	{
		for (int j = dOrder + 1; j <= iOrder; j++)
		{
			c.row(termCount).setZero();
			c(termCount, i) = (float)j;
			termCount++;
		}
	}

	// initialize maxMinusMinValues
	inputRange = (inputUpperBound - inputLowerBound);
}

FourierBasis::~FourierBasis() {};

int FourierBasis::getNumOutputs() const
{
	return nTerms;
}

void FourierBasis::basify(VectorXd & buff, const VectorXd & x) const
{
	// Normalize the state
	VectorXd normalizedInput = (x - inputLowerBound).array() / inputRange;

	// Compute the features
	buff = (M_PI*c*normalizedInput).array().cos();
}

FourierBasis * FourierBasis::clone() const
{
	return new FourierBasis(*this);
}