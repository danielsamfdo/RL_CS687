#include <stdafx.hpp>

using namespace std;
using namespace Eigen;

// Compute the sample variance of the provided vector
double var(const Eigen::VectorXd & v)
{
	double mu = v.mean();
	double result = 0;
	for (int i = 0; i < (int)v.size(); i++)
		result += (v[i] - mu)*(v[i] - mu);
	return result / (v.size() - 1.0);
}

// For the menu system: ask a question and return the answer. The "..." 
// arguments are numOptions const char * objects that contain the possible 
// answers.
int getSelection(const char * name, int numOptions, ...) {
	va_list vl;
	va_start(vl, numOptions);
	cout << "Select " << name << ":" << endl;
	for (int i = 0; i < numOptions; i++)
		cout << "[" << i << "] " << va_arg(vl, const char*) << endl;
	cout << "Selection: ";
	int result;
	cin >> result;
	getchar();
	return result;

}

// Run one agent on one environment for some number of episodes, and return the
// resulting discounted sums of rewards
VectorXd runAgentEnvironment(Agent * a, Environment * e, FourierBasis * fb, int numEpisodes, mt19937_64 & generator)
{
	VectorXd state, curFeatures, newFeatures, result(numEpisodes);
	double reward, gamma = e->getGamma(), curGamma, G;
	int action;
	for (int epCount = 0; epCount < numEpisodes; epCount++)
	{
		if (epCount != 0)
			e->newEpisode();
		G = 0;
		curGamma = 1.0;
		state = e->getState();
		fb->basify(curFeatures, state);
		for (int t = 0; true; t++)
		{
			action = a->getAction(curFeatures, generator);
			reward = e->update(action);
			G += curGamma*reward;
			curGamma *= gamma;
			if (e->inTerminalState())
			{
				a->train(curFeatures, action, reward);
				result[epCount] = G;
				break;
			}
			state = e->getState();
			fb->basify(newFeatures, state);
			a->train(curFeatures, action, reward, newFeatures);
			curFeatures = newFeatures;
		}
	}
	return result;
}

// Entry point - runs the menu system
int main(int argc, char * argv[])
{
	while (true)
	{
		string agentName, envName;
		double alpha, lambda, epsilon, alpha_critic, alpha_actor, gamma, alpha_baseline;

		// Get the agent to run
		int agentType = getSelection("Agent", 4, "Q(lambda)", "Actor-Critic", "REINFORCE", "Exit");
		if (agentType == 3)
			exit(0);

		// Get the environment to run
		int envType = getSelection("Environment", 5, "Gridworld", "Mountain Car", "Cart Pole", "Acrobot", "Exit");
		if (envType == 4)
			exit(0);

		// Ask what order Fourier basis (if not the Gridworld)
		int order;
		if (envType == 0)
			order = 0; // For gridworld, we will use identity basis, so this input doesn't matter.
		else
		{
			cout << "Enter Fourier basis order: ";
			cin >> order;
		}

		// As for the appropriate parameters for the specified agent
		if (agentType == 0)
		{
			agentName = "QLambda";
			cout << "Enter alpha: ";
			cin >> alpha;
			cout << "Enter lambda: ";
			cin >> lambda;
			cout << "Enter epsilon: ";
			cin >> epsilon;
			cout << "Enter gamma: ";
			cin >> gamma;
		}
		else if (agentType == 1)
		{
			agentName = "ActorCritic";
			cout << "Enter alpha (actor): ";
			cin >> alpha_actor;
			cout << "Enter alpha (critic): ";
			cin >> alpha_critic;
			cout << "Enter lambda: ";
			cin >> lambda;
			cout << "Enter gamma: ";
			cin >> gamma;
		}
		else if (agentType == 2)
		{
			agentName = "REINFORCE";
			cout << "Enter alpha (actor): ";
			cin >> alpha_actor;
			cout << "Enter alpha (baseline): ";
			cin >> alpha_baseline;
			cout << "Enter gamma: ";
			cin >> gamma;
		}
		else
			exit(1);

		// Ask how many trials to run
		int numTrials;
		cout << "Enter number of trials to run: ";
		cin >> numTrials;

		// Ask how many episodes to run in each trial
		int numEpisodes;
		cout << "Enter the number of episodes per trial: ";
		cin >> numEpisodes;

		// Create one generator per trial, since we will multi-thread and generators should not be shared across threads.
		vector<mt19937_64> generators(numTrials);
		for (int i = 0; i < numTrials; i++)
			generators[i].seed(i);

		// Create vector of vectors to store the results
		vector<VectorXd> results(numTrials);
		
		cout << "Running..." << endl;
		// Loop over the trials (threaded)
#pragma omp parallel for
		for (int trialCount = 0; trialCount < numTrials; trialCount++)
		{
			// Create the environment
			Environment * e;
			if (envType == 0)
			{
				e = new Gridworld();
				envName = "Gridworld";
			}
			else if (envType == 1)
			{
				e = new MountainCar();
				envName = "MountainCar";
			}
			else if (envType == 2)
			{
				e = new CartPole();
				envName = "CartPole";
			}
			else if (envType == 3)
			{
				e = new Acrobot();
				envName = "Acrobot";
			}
			else
				exit(1);

			// Create the Fourier basis
			FourierBasis fb(e->getStateDim(), e->getMinState(), e->getMaxState(), order, order, envType == 0);

			// Create the agent
			Agent * a;
			if (agentType == 0)
				a = new QLambda(fb.getNumOutputs(), e->getNumActions(), alpha, gamma, lambda, epsilon);
			else if (agentType == 1)
				a = new ActorCritic(fb.getNumOutputs(), e->getNumActions(), alpha_actor, alpha_critic, gamma, lambda);
			else if (agentType == 2)
				a = new REINFORCE(fb.getNumOutputs(), e->getNumActions(), alpha_actor, alpha_baseline, gamma);
			else
				exit(1);

			// Run one trial and save the resulting vector of returns in results[trialCount]
			results[trialCount] = runAgentEnvironment(a, e, &fb, numEpisodes, generators[trialCount]);
		}

		// Print results to a file
		string fileName = "out_" + agentName + "_" + envName + ".csv";
		ofstream out(fileName.c_str());
		out << "Mean Return,Standard Deviation" << endl;
		for (int epCount = 0; epCount < numEpisodes; epCount++)
		{
			// Collect results from all trials for this particular episode into one vector
			VectorXd v(numTrials);
			for (int trialCount = 0; trialCount < numTrials; trialCount++)
				v[trialCount] = results[trialCount][epCount];

			// Print the mean and sample standard deviation
			out << v.mean() << "," << sqrt(var(v)) << endl;
		}
		// Close and print a line saying that we're done with this experiment.
		out.close();
		cout << "Results printed to file." << endl << endl;
	}
}