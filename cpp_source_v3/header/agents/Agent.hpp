#pragma once

#include <stdafx.h>

// Online and RL agent meant to be given a stream of on-policy data. It can
// choose to only do updates after an episode of batch of episodes, but it is
// not provided with a vector of histories/trajectories for training.
class Agent {
public:
	struct Description
	{
		bool canUpdateBeforeAPrime;

		bool handlesEpisodic;
		bool handlesContinuing;

		bool handlesFiniteHorizon;
		bool handlesIndefiniteHorizon;
		bool handlesInfiniteHorizon;

		bool handlesDiscreteObservations;
		bool handlesContinuousObservations;
		bool handlesHybridObservations;

		bool handlesDiscreteActions;
		bool handlesContinuousActions;
		bool handlesHybridActions;

		bool handlesStochasticTransitions;
		bool handlesStochasticRewards;
		bool handlesNonMarkovianObservations;
		bool handlesStochasticInitialState;
	};

	Agent();
	virtual ~Agent();

	////////////////////////////////////////////////////////////////////////////
	// Pure Virtual functions
	////////////////////////////////////////////////////////////////////////////
	// Get description of this agent
	virtual Agent::Description getDesc() const = 0;
	
	// Terminal update
	virtual void train(std::mt19937_64 & generator, const Thing & o, const Thing & a, const double & r) = 0;

	// Non-terminal update. aPrime = nullptr if canUpdateBeforeAPrime = true
	virtual void train(std::mt19937_64 & generator, const Thing & o, const Thing & a, const double & r, const Thing & oPrime, const Thing * aPrime = nullptr) = 0;

	// Even if a terminal update wasn't given, prepare for a new episode
	virtual void newEpisode(std::mt19937_64 & generator) = 0;

	// Function to get action
	virtual void getAction(Thing & buff, const Thing & o, std::mt19937_64 & generator) = 0;

	// Clone this agent. Generator is used to reset random variables to avoid common random numbers (e.g., CEM's "samples" vector)
	virtual Agent * clone(std::mt19937_64 & generator) const = 0;
};