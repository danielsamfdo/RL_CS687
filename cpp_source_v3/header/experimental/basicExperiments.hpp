#pragma once

#include <stdafx.h>

/*
Run an agent on a specific environment repeatedly.

Returned matrix is of size [numTrials][maxEps], and holds the returns from each episode
*/
Eigen::MatrixXd runAgentEnvironment(
	Agent * a,						// Pointer to the agent
	Environment * e,				// Pointer to the environment
	int numTrials,					// Number of times to run the agent on the environment
	int maxEps,						// How many episodes per trial?
	int maxT,						// Cut all episodes after how many time steps?
	std::mt19937_64 & generator,	// RNG
	int maxThreads = 1,				// How many threads to use (upper limit)?
	const std::vector<int> & epsToDraw = std::vector<int>()	// Which episodes should be drawn?
	);

