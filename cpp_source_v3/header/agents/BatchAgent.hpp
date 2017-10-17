#pragma once

#include <stdafx.h>

// Batch RL Agent. For a *single* policy step. If you will do multiple steps,
// use the Agent class. Also, this class only works on episodic environments
class BatchAgent {
public:
	struct Description
	{
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

	BatchAgent();
	virtual ~BatchAgent();

	// Get description of this agent
	virtual BatchAgent::Description getDesc() const = 0;

	// Compute policy from batch of data. If algorithm requires other information like 
	// an initial policy, that should be given in the subclass's constructor.
	// If DSize != -1, we use the first DSize elements of D only and ignore the
	// remainder.
	virtual Policy * train(std::vector<History> & D, int DSize = -1) const = 0;

	virtual BatchAgent * clone(std::mt19937_64 & generator) const = 0;
};