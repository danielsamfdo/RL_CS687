#pragma once

#include <stdafx.h>

class Policy {
public:
	Policy();
	virtual ~Policy();

	////////////////////////////////////////////////////////////////////////////
	// Pure Virtual functions
	////////////////////////////////////////////////////////////////////////////
	// How many tunable parameters?
	virtual int getNumParams() const = 0;

	// Get the current parameter vector (if no parameters, return zero vector)
	virtual Eigen::VectorXd getParams() const = 0;

	// Set the parameters = newParams
	virtual void setParams(const Eigen::VectorXd & newParams) = 0;

	// Add dParams to the parameters
	virtual void addToParams(const Eigen::VectorXd & dParams) = 0;

	// Get an action given an observation.
	virtual void getAction(Thing & buff, const Thing & o, std::mt19937_64 & generator) = 0;

	// Get the action probability
	virtual double pi(const Thing & o, const Thing & a) const = 0;

	// Get the derivative of pi(s,a) with respect to the policy parameters. Return empty vector if no parameters and zero vector if not differentiable.
	virtual void dpi(Eigen::VectorXd & buff, const Thing & o, const Thing & a) const = 0;

	// Get the derivative of ln(pi(s,a)) with respect to the policy parameters. Return empty vector if no parameters and zero vector if not differentiable.
	virtual void dlnpi(Eigen::VectorXd & buff, const Thing & o, const Thing & a) const = 0;

	// Clone this policy
	virtual Policy * clone(std::mt19937_64 & generator) const = 0;
};