#include <stdafx.hpp>

using namespace Eigen;
using namespace std;

QLambda::QLambda(int numFeatures, int numActions, double alpha, double gamma, double lambda, double epsilon)
{
	// Copy over the provided arguments
	this->numActions = numActions;
	this->numFeatures = numFeatures;
	this->alpha = alpha;
	this->gamma = gamma;
	this->lambda = lambda;

	// Initialize the weights for the q-approximation and their e-traces
	e = w = VectorXd::Zero(numFeatures*numActions);

	// Create the two random number distributions that we will use during action selection
	d = bernoulli_distribution(epsilon);
	explorationDistribution = uniform_int_distribution<int>(0, numActions - 1);
}

QLambda::~QLambda() {}

int QLambda::getAction(const Eigen::VectorXd & features, std::mt19937_64 & generator)
{
	// Check if we should explore
	if (d(generator)) // This returns true with probability epsilon, due to the way that d was initialized in the constructor (up top)
		return explorationDistribution(generator);	// Explore by selecting an action uniformly randomly (this is what the explorationDistribution was initialized to do)
	
	// Get q(s,a) for each a. This is just a dot-product
	VectorXd qValues(numActions);
	for (int a = 0; a < numActions; a++)
		qValues[a] = features.dot(w.segment(numFeatures*a, numFeatures));

	// Get a greedy action
	vector<int> bestActions(1);					// Here we will track which actions are currently believed to be optimal
	bestActions[0] = 0;							// Start with the 0'th action being optimal
	double bestActionValue = qValues[0];
	for (int a = 1; a < numActions; a++)		// Loop over other actions and see if they are better
	{
		if (qValues[a] == bestActionValue)
			bestActions.push_back(a);			// Action a is just as good as the current actions believed to be best. Append to bestActions
		else if (qValues[a] > bestActionValue)
		{
			bestActionValue = qValues[a];		// Action a is better than previous actions. Clear bestActions and place in only action a
			bestActions.resize(1);
			bestActions[0] = a;
		}
	}

	// If there's only one best action, return it
	if ((int)bestActions.size() == 1)
		return bestActions[0];

	// There are many best actions - select uniformly among them
	uniform_int_distribution<int> greedyIndexDistribution(0, (int)bestActions.size()-1);
	return bestActions[greedyIndexDistribution(generator)];
}

// Train function to be used for non-terminal updates
void QLambda::train(const VectorXd & features, const int & action, const double & reward, const VectorXd & newFeatures)
{
	// Compute q(s,a), which is q(features, action) using our variables here
	double curQ;
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 1 OF 9
	// @TODO: Fill in code to compute curQ. See getAction to get an idea of how the weights are used to compute q-values

	// Compute max_a', q(s',a'), where s' is described by newFeatures
	double newQ;
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 2 OF 9
	// @TODO: Fill in code to compute newQ

	// Compute the TD error using reward, newQ, curQ, and gamma
	double delta;
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 3 OF 9
	// @TODO: Fill in code to compute delta using reward, curQ, newQ, and gamma (already defined as part of the QLambda class)

	// Update the e-traces (e).
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 4 OF 9
	// @TODO: Fill in the code to update the vector, e
	
	// Update the q-estimate (w)
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 5 OF 9
	// @TODO: Fill in the code to update the vector, w
}

// Train function to be used when newFeatures correspond to a terminal state
void QLambda::train(const VectorXd & features, const int & action, const double & reward)
{
	// Compute q(s,a), which is q(features, action) using our variables here
	double curQ;
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 6 OF 9
	// @TODO: Fill in code to compute curQ. See getAction to get an idea of how the weights are used to compute q-values

	// Compute the TD error using reward, curQ, and gamma (newQ = 0 since this is a terminal update)
	double delta;
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 7 OF 9
	// @TODO: Fill in code to compute delta using reward, curQ, newQ, and gamma (already defined as part of the QLambda class)

	// Update the e-traces (e).
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 8 OF 9
	// @TODO: Fill in the code to update the vector, e

	// Update the q-estimate (w)
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 9 OF 9
	// @TODO: Fill in the code to update the vector, w

	// Clear the e-traces because this is a terminal update (the newFeatures would correspond to a terminal state)
	e.setZero();
}