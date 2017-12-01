#pragma once

#include <stdafx.hpp>

// REINFORCE with a constant baseline.
// This version uses softmax action selection with linear function approximation. It also
// assumes that the features are provided as input, rather than the raw state.
class REINFORCE : public Agent
{
public:
	REINFORCE(int numFeatures, int numActions, double alpha_actor, double alpha_baseline, double gamma);
	~REINFORCE();
	int getAction(const Eigen::VectorXd & state, std::mt19937_64 & generator) override;

	// Train function to be used for non-terminal updates
	void train(const Eigen::VectorXd & features, const int & action, const double & reward, const Eigen::VectorXd & newFeatures) override;

	// Train function to be used when newFeatures correspond to a terminal state
	void train(const Eigen::VectorXd & features, const int & action, const double & reward) override;

private:
	int numFeatures;			// How many features expected as input?
	int numActions;				// How many actions to select between?

	double alpha_actor;			// Step size for the actor
	double alpha_baseline;		// Step size for learning the baseline
	double gamma;				// Reward discount parameter
	
	// Data structures for storing episodes
	std::vector<Eigen::VectorXd> featureHistory;
	std::vector<int> actionHistory;
	std::vector<double> rewardHistory;

	Eigen::VectorXd theta;		// Policy parameters
	double baseline;			// Current baseline

	std::uniform_real_distribution<double> d;	// Uniform distribution over [0,1] used when selecting actions

	Eigen::VectorXd dlnpi(const Eigen::VectorXd & features, const int & action);	// $\frac{\partial \ln(\pi(features,action,\theta))}{\partial \theta}$
	Eigen::VectorXd getActionProbabilities(const Eigen::VectorXd & features);		// Get a vector containing the probabilities of each action given the current state is described by "features"
};