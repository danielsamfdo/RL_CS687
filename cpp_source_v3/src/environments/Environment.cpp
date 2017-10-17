#include <stdafx.h>

using namespace std;

Environment::Environment() {}

Environment::~Environment() {}

// Generate a set of trajectories (complete histories). Generator needed for policies that are not deterministic
void Environment::sampleEpsiodes(
	const int & numEps,
	Policy * p,
	const bool & includeActionProbabilities,
	vector<History> & buff,
	mt19937_64 & generator,
	const int & maxT)
{
	Thing observation, action;
	double reward;
	buff.resize(numEps);
	for (int epCount = 0; epCount < numEps; epCount++)
	{
		// Create new episode and clear the current history in buff
		newEpisode(generator);
		buff[epCount].actionProbabilities.resize(0);
		buff[epCount].actions.resize(0);
		buff[epCount].includesActionProbabilities = includeActionProbabilities;
		buff[epCount].length = 0;
		buff[epCount].observations.resize(0);
		buff[epCount].rewards.resize(0);

		for (int t = 0; true; t++)
		{
			getObs(observation, generator);
			buff[epCount].observations.push_back(observation);
			p->getAction(action, observation, generator);
			buff[epCount].actions.push_back(action);
			if (includeActionProbabilities)
				buff[epCount].actionProbabilities.push_back(p->pi(observation, action));
			reward = update(action, generator);
			buff[epCount].rewards.push_back(reward);

			buff[epCount].length++;
			if (inTerminalState())
			{
				buff[epCount].terminal = true;
				break;
			}
			if (t == maxT - 1)
			{
				buff[epCount].terminal = false;
				break;
			}
		}
	}
}

// Evaluate the provided policy. Generator should  be provided if anything is stochastic
// This function can be overwritten if an environment thinks that it can do this more efficiently.
double Environment::evaluatePolicy(
	Policy * p, 
	const int & numEps, 
	mt19937_64 & generator,
	const int & maxT)
{
	double result = 0;
	Thing observation, action;
	double reward, gamma = getDesc().gamma, curGamma;
	int numT = 0, t;
	for (int epCount = 0; epCount < numEps; epCount++)
	{
		// Create new episode and clear the current history in buff
		newEpisode(generator);
		curGamma = 1.0;
		t = 0;
		while (true)
		{
			getObs(observation, generator);
			p->getAction(action, observation, generator);
			reward = update(action, generator);
			result = result + curGamma*reward;
			curGamma *= gamma;
			t++;
			if ((inTerminalState()) || (t == maxT)) // if maxT==-1 this will never happen and it will run until termination.
				break;
		}
		numT += t;
	}
	if (getDesc().episodic)
		return result / (double)numEps;		// Mean discounted return
	return result / (double)numT;			// Average reward
}

void Environment::draw(const bool & realtimePause) {}

void Environment::closeWindow() {}