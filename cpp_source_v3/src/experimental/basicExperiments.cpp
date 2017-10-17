#include <stdafx.h>
#include <experimental/basicExperiments.hpp>

using namespace std;
using namespace Eigen;

Eigen::MatrixXd runAgentEnvironment(
	Agent * a,						// Pointer to the agent
	Environment * e,				// Pointer to the environment
	int numTrials,					// Number of times to run the agent on the environment
	int maxEps,						// How many episodes per trial?
	int maxT,						// Cut all episodes after how many time steps?
	std::mt19937_64 & generator,	// RNG
	int maxThreads,					// How many threads to use (upper limit)?
	const std::vector<int> & epsToDraw)		// Which episodes should be drawn?
{
	// Make copies of variables for each thread
	vector<Agent*> agents(numTrials);
	vector<Environment*> environments(numTrials);
	vector<mt19937_64> generators(numTrials);
	for (int i = 0; i < numTrials; i++)
	{
		agents[i] = a->clone(generator);
		environments[i] = e->clone(generator);
		generators[i].seed(generator());
	}

	// Matrix in which we will store the results
	MatrixXd result(numTrials, maxEps);

	// Get the agent and environment descriptions once
	Agent::Description agentDesc = a->getDesc();
	Environment::Description envDesc = e->getDesc();

	// Threaded loop
#ifdef  _USE_MULTITHREADING
	omp_set_num_threads(std::min<int>(_MAX_THREADS,maxThreads));
	#pragma omp parallel for
#endif
	for (int trial = 0; trial < numTrials; trial++)
	{
		Thing curObs, curAct, newObs, newAct;
		double reward, curReturn, curGamma, gamma = envDesc.gamma;
		for (int epCount = 0; epCount < maxEps; epCount++)
		{
			// Do we draw this episode?
			bool drawThisEpisode = ((trial == 0) && (find(epsToDraw.begin(), epsToDraw.end(), epCount) != epsToDraw.end()));

			// Prep for new episode
			if (epCount != 0)
			{
				agents[trial]->newEpisode(generators[trial]);
				environments[trial]->newEpisode(generators[trial]);
			}
			curReturn = 0;
			curGamma = 1.0;

			// Get initial observation and action
			environments[trial]->getObs(curObs, generator);
			agents[trial]->getAction(curAct, curObs, generators[trial]);

			// Draw initial state
			if (drawThisEpisode)
				environments[trial]->draw(true);
			for (int t = 0; true; t++)
			{
				// Update the environment and get the reward
				reward = environments[trial]->update(curAct, generators[trial]);

				// Draw the resulting state
				if (drawThisEpisode)
					environments[trial]->draw(true);

				// Update the current reutrn
				curReturn += curGamma*reward;
				curGamma *= gamma;

				// Check if in terminal state
				if ((environments[trial]->inTerminalState()) || (t == maxT))
				{
					// Do terminal update
					agents[trial]->train(generator, curObs, curAct, reward);

					// Store the return
					result(trial, epCount) = curReturn;

					// End this episode loop
					break;
				}

				// Get the new observation
				environments[trial]->getObs(newObs, generators[trial]);

				// Update (if we don't need newAction to train agent)
				if (agentDesc.canUpdateBeforeAPrime)
					agents[trial]->train(generator, curObs, curAct, reward, newObs);
				
				// Get newAction
				agents[trial]->getAction(newAct, newObs, generators[trial]);

				// Update (if we need newAction to train agent)
				if (!agentDesc.canUpdateBeforeAPrime)
					agents[trial]->train(generator, curObs, curAct, reward, newObs, &newAct);

				// Copy cur <-- new
				curObs = newObs;
				curAct = newAct;
			}			
		}
	}

	// Clean up memory!
	for (int i = 0; i < numTrials; i++)
	{
		// Both base clases have virtual destructors, so this delete will not leak
		delete agents[i];
		delete environments[i];
	}

	return result;
}