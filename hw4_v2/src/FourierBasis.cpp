#include <stdafx.hpp>

using namespace std;
using namespace Eigen;

static int ipow(const int & a, const int & b)
{
	if (b == 0) return 1;
	if (b == 1) return a;

	int tmp = ipow(a, b / 2);
	if (b % 2 == 0) return tmp * tmp;
	else return a * tmp * tmp;
}

static void incrementCounter(VectorXi & counter, int maxDigit)
{
	for (int i = 0; i < (int)counter.size(); i++) {
		counter[i]++;
		if (counter[i] > maxDigit)
			counter[i] = 0;
		else
			break;
	}
}

FourierBasis::FourierBasis(
	const int & inputDimension,
	const VectorXd & inputLowerBound,
	const VectorXd & inputUpperBound,
	int iOrder,
	int dOrder,
	bool useIdentityBasis)
{
	// Copy over the provided arguments
	this->inputDimension = inputDimension;
	this->inputLowerBound = inputLowerBound;
	this->inputUpperBound = inputUpperBound;
	this->useIdentityBasis = useIdentityBasis;
	
	// Compute the total number of terms
	int iTerms = iOrder*inputDimension;						// Number of independent terms
	int dTerms = ipow(dOrder + 1, inputDimension);			// Number of dependent terms
	int oTerms = min(iOrder, dOrder)*inputDimension;			// Overlap of iTerms and dTerms
	nTerms = iTerms + dTerms - oTerms;

	// Initialize c
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
	if (useIdentityBasis)
		return inputDimension;
	return nTerms;
}

void FourierBasis::basify(VectorXd & buff, const VectorXd & x) const
{
	if (useIdentityBasis)
		buff = x;
	else
	{
		// Normalize the state
		VectorXd normalizedInput = (x - inputLowerBound).array() / inputRange;

		// Compute the features
		buff = (M_PI*c*normalizedInput).array().cos();
	}
}