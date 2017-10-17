#pragma once

#include <stdafx.h>

// The bare-minimum requirements for an environment
class Environment {
public:
	struct Description
	{
		// Episodic or continuing?
		bool episodic;

		// What kind of horizon? Only one of these should be set
		bool finiteHorizon;
		bool indefiniteHorizon;
		bool infiniteHorizon;

		// If you are going to cut episodes, when should you cut them?
		int suggestedMaxT;

		// How many episodes should an agent be given to learn on this env?
		int suggestedMaxEps;

		// Reward discount parameter. Only used if episodic (not for continuing tasks)
		double gamma;

		// Description of the actions
		ThingDesc actDesc;

		// Description of the observations
		ThingDesc obsDesc;

		// Various properties one might want to know
		bool stochasticStateTransitions;
		bool stochasticRewards;
		bool MarkovianObservations;
		bool deterministicInitialState;
		double minReward;	// Set to -INF if not known or not bounded
		double maxReward;	// Set to INF if not known or not bounded
		double minReturn;	// Set to -INF if not known or not bounded
		double maxReturn;	// Set to INF if not known or not bounded

		// Recommended plot y-axis
		double suggestedPlotMinPerformance;
		double suggestedPlotMaxPerformance;
	};

	// Default constructor
	Environment();

	// Virtual destructor so that deleting Environment doesn't leak memory
	virtual ~Environment();

	////////////////////////////////////////////////////////////////////////////
	// Virtual functions
	////////////////////////////////////////////////////////////////////////////
	// Generate a set of trajectories (complete histories).
	virtual void sampleEpsiodes(
		const int & numEps,
		Policy * p,
		const bool & includeActionProbabilities,
		std::vector<History> & buff,
		std::mt19937_64 & generator,
		const int & maxT = -1);

	// Evaluate the provided policy.
	virtual double evaluatePolicy(
		Policy * p,
		const int & numEps,
		std::mt19937_64 & generator,
		const int & maxT = -1);

	// Draw to a window. If not defined by subclass, it does nothing
	virtual void draw(const bool & realtimePause);		

	// Close the window. If not defined by subclass, it does nothing						
	virtual void closeWindow();

	////////////////////////////////////////////////////////////////////////////
	// Pure Virtual functions 
	////////////////////////////////////////////////////////////////////////////
	virtual Environment::Description getDesc() const = 0;

	// Apply the action and return the resulting reward.
	virtual double update(const Thing & a, std::mt19937_64 & generator) = 0;

	// Get the observation.
	virtual void getObs(Thing & buff, std::mt19937_64 & generator) const = 0;

	// Ask if in a terminal state. No randomnes - include randomness in state transition inside update
	virtual bool inTerminalState() const = 0;

	// Reset to start a new episode.
	virtual void newEpisode(std::mt19937_64 & generator) = 0;

	// Get some initial policy.
	virtual Policy * getInitialPolicy() const = 0;

	// Create a clone of this object
	virtual Environment * clone(std::mt19937_64 & generator) const = 0;

	// Get the name of the environment
	virtual const char * getName() const = 0;
};