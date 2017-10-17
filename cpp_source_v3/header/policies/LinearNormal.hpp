#pragma once

#include <stdafx.h>
#include <bases/FourierBasis.hpp>

/*
Normal policy using linear function approximation. Only handles a single
action dimension. Future versions should allow for multivariate normal policies.

Eventually perhaps this could include a Sigmoid to let the user bound the action
ranges.
*/
class LinearNormal : public Policy {
public:
	// Constructors and assignment operator. b must be provided if oDesc.contDim != 0, and should take o.cont as input
	LinearNormal(
		const ThingDesc & oDesc, 
		const bool & tuneSigma,					// Is sigma a tunable parameter?
		const double & sigma,					// Initial standard deviation
		const Basis * b);	// Initial action standard deviations. If not set, then set based on the range of the actions in a reasonable-ish way
	~LinearNormal() override;

	LinearNormal(const LinearNormal & rhs);
	LinearNormal & operator=(const LinearNormal & rhs);

	int getNumParams() const;
	Eigen::VectorXd getParams() const;
	void setParams(const Eigen::VectorXd & newParams);
	void addToParams(const Eigen::VectorXd & dParams);

	void getAction(Thing & buff, const Thing & o, std::mt19937_64 & generator) override;
	double pi(const Thing & o, const Thing & a) const override;
	void dpi(Eigen::VectorXd & buff, const Thing & o, const Thing & a) const override;
	void dlnpi(Eigen::VectorXd & buff, const Thing & o, const Thing & a) const override;
	LinearNormal * clone(std::mt19937_64 & generator) const override;

private:
	// Description of observations
	ThingDesc oDesc;

	bool tuneSigma;
	double sigma;	// Used if we aren't storing it in params

	// Basis for linear function approximation
	Basis * b;
	
	int numFeatures; // b->getNumOutputs();

	// The policy's parameters
	Eigen::VectorXd params;

	std::normal_distribution<double> d;
};