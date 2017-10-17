#pragma once

#include <stdafx.h>

struct History
{
	int length;								// How many actions long is this history?
	bool terminal;							// Does it end with a terminal state? I.e., is this a trajectory?
	bool includesActionProbabilities;		// Is actionProbabilities vector loaded?
	std::vector<Thing> observations;		// Size = length, unless not terminal, in which case it might be length+1 (including sPrime)
	std::vector<Thing> actions;
	std::vector<double> actionProbabilities;// Size = 0 or length depending on "includeActionProbabilities"
	std::vector<double> rewards;
};