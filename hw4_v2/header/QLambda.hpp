#pragma once

#include <stdafx.hpp>

// Q(lambda) algorithm using linear function approximation
// This version uses linear function approximation and assumes that the features are provided
// rather than the raw state.
class QLambda : public Agent
{
public:
	QLambda(int numFeatures, int numActions, double alpha, double gamma, double lambda, double epsilon);
	~QLambda();
	int getAction(const Eigen::VectorXd & state, std::mt19937_64 & generator) override;

	// Train function to be used for non-terminal updates
	void train(const Eigen::VectorXd & features, const int & action, const double & reward, const Eigen::VectorXd & newFeatures) override;

	// Train function to be used when newFeatures correspond to a terminal state
	void train(const Eigen::VectorXd & features, const int & action, const double & reward) override;

private:
	int numFeatures;				// How many features expected as input?
	int numActions;					// How many actions to select between?

	double alpha;					// Step size
	double gamma;					// Reward discount parameter
	double lambda;					// Eligibility trace decay rate
	
	Eigen::VectorXd w;				// Weights for linear q-approximation
	Eigen::VectorXd e;				// Eligibility traces for q

	std::bernoulli_distribution d;	// The distribution to decide whether or not to explore
	std::uniform_int_distribution<int> explorationDistribution;	// If exploring, this is the uniform distribution over actions.
};