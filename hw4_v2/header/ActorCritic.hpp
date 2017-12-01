#pragma once

#include <stdafx.hpp>

// Actor-Critic agent, page 277 of 2nd edition draft of Sutton and Barto's book from November 5
// This version uses linear function approximation and assumes that the features are provided
// rather than the raw state.
class ActorCritic : public Agent
{
public:
	ActorCritic(int numFeatures, int numActions, double alpha_actor, double alpha_critic, double gamma, double lambda);
	~ActorCritic();
	int getAction(const Eigen::VectorXd & state, std::mt19937_64 & generator) override;

	// Train function to be used for non-terminal updates
	void train(const Eigen::VectorXd & features, const int & action, const double & reward, const Eigen::VectorXd & newFeatures) override;

	// Train function to be used when newFeatures correspond to a terminal state
	void train(const Eigen::VectorXd & features, const int & action, const double & reward) override;

private:
	int numFeatures;			// How many features expected as input?
	int numActions;				// How many actions to select between?

	double alpha_actor;			// Step size for the actor
	double alpha_critic;		// Step size for the critic
	double gamma;				// Reward discount parameter
	double lambda;				// Eligibility trace decay rate

	Eigen::VectorXd theta;		// Policy parameters
	Eigen::VectorXd eTheta;		// Eligibility traces for the policy parameters

	Eigen::VectorXd v;			// Value function estimation parameters
	Eigen::VectorXd ev;			// Eligibility traces for v

	Eigen::VectorXd dlnpi(const Eigen::VectorXd & features, const int & action);	// $\frac{\partial \ln(\pi(features,action,\theta))}{\partial \theta}$
	Eigen::VectorXd getActionProbabilities(const Eigen::VectorXd & features);		// Get a vector containing the probabilities of each action given the current state is described by "features"

	std::uniform_real_distribution<double> d;	// A uniform distribution over [0,1] used during action selection.
};