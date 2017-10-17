#include <stdafx.h>

using namespace std;
using namespace Eigen;

mvnpdf::mvnpdf(const VectorXd & meanVec, const MatrixXd & covarMat) {
	n = (int)meanVec.size();
	setMean(meanVec);
	setCovar(covarMat);
}

// Set the covariance matrix to a new matrix. Must be the same size as 
// before, otherwise you should just create a new object
void mvnpdf::setCovar(const MatrixXd & covarMat) {
	assert((int)covarMat.rows() == n); // If not, make a new object
	assert((int)covarMat.cols() == n); // If not, make a new object
	Sigma = covarMat;
	SigmaInv = Sigma.pinv();
	DetermSigma = Sigma.determinant();
}

// Set the mean vector. Must be the same length as before, otherwise you
// should just create a new object
void mvnpdf::setMean(const VectorXd & meanVec) {
	assert((int)meanVec.size() == n); // If not, make a new object
	mu = meanVec;
}

// Sets both the mean and covariance matrix (like calling the constructor
// again.
void mvnpdf::set(const VectorXd & meanVec, const MatrixXd & covarMat) {
	n = (int)meanVec.size();
	setCovar(covarMat);
	setMean(meanVec);
}

// Get probability of the specified x
// See http://www.cs.huji.ac.il/~csip/tirgul34.pdf
double mvnpdf::Pr(const VectorXd & x) {
	double firstTerm = 1.0 / (pow(2.0*M_PI, (double)n / 2.0)
		* sqrt(DetermSigma));
	VectorXd temp = x - mu;
	double numerator = temp.transpose()*SigmaInv*temp;
	double exponent = -numerator / 2.0;
	return firstTerm * exp(exponent);
}

/*
If you will only sample once, or call mvnpdf once, then use these. If you
will call many times, make the object so you don't recompute eigenvectors.
*/
double mvnpdf::Pr(const VectorXd & x, const VectorXd & meanVec,
	const MatrixXd & covarMat) {
	int n = (int)meanVec.size();
	VectorXd mu;
	MatrixXd Sigma;
	MatrixXd SigmaInv;
	double DetermSigma;

	// setMean(meanVec);
	assert((int)meanVec.size() == n); // If not, make a new object
	mu = meanVec;

	// setCovar(covarMat);
	assert((int)covarMat.rows() == n); // If not, make a new object
	assert((int)covarMat.cols() == n); // If not, make a new object
	Sigma = covarMat;
	SigmaInv = Sigma.pinv();
	DetermSigma = Sigma.determinant();

	// Actually compute Pr
	double term1 = 1.0 / (pow(2.0*M_PI, (double)n / 2.0)*sqrt(DetermSigma));
	VectorXd temp = x - mu;
	double numerator = temp.transpose()*SigmaInv*temp;
	double exponent = -numerator / 2.0;
	return term1 * exp(exponent);
}