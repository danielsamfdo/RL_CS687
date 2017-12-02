#include <stdafx.hpp>

using namespace Eigen;
using namespace std;

ActorCritic::ActorCritic(int numFeatures, int numActions, double alpha_actor, double alpha_critic, double gamma, double lambda)
{
	// Copy over the provided arguments
	this->numActions = numActions;
	this->numFeatures = numFeatures;
	this->alpha_actor = alpha_actor;
	this->alpha_critic = alpha_critic;
	this->gamma = gamma;
	this->lambda = lambda;

	// Initialize weights for value function approximation to zero, along with the traces for v
	ev = v = VectorXd::Zero(numFeatures);

	// Initialize the policy parameters and their traces to zero
	eTheta = theta = VectorXd::Zero(numFeatures*numActions);

	// Create d to be a uniform distribution over [0,1], used during action selection
	d = uniform_real_distribution<double>(0, 1);
}

ActorCritic::~ActorCritic() {}

int ActorCritic::getAction(const Eigen::VectorXd & features, std::mt19937_64 & generator)
{
	// Get action probabilities
	VectorXd actionProbabilities = getActionProbabilities(features);

	// Sample action probabilities
	double sum = 0, r = d(generator);
	for (int i = 0; i < numActions; i++)
	{
		sum += (double)actionProbabilities[i];
		if (sum >= r) return i;
	}
	// If we get here, there was a rounding error... doh. Pick a random action
	return uniform_int_distribution<int>(0, numActions - 1)(generator);
}

// Train function to be used for non-terminal updates
void ActorCritic::train(const VectorXd & features, const int & action, const double & reward, const VectorXd & newFeatures)
{
	// You need to implement the update below. The current state is described by the vector "features", the current action is "action"
	// the resulting reward is "reward" and the resulting next state is described by the vector "newFeatures". You can use variables
	// "lambda", "gamma", "alpha_actor", "alpha_critic", which have already been set for you.

	// Update the critic (update v, the weights for the linear value function approximation, and ev, the e-traces for v)
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 1 OF 5
	// @TODO: Fill in the code for the updates to ev, and then the update to v. Note that v(s) = v.dot(features) and v(s') = v.dot(newFeatures)
	double delta = reward + (gamma * v.dot(newFeatures)) - v.dot(features);
	ev = (gamma * lambda * ev) + features;
	v = v + (alpha_critic * delta * ev);
	// Update the actor (update theta, the policy parameters, eTheta, the e-traces for theta). Here you may want to use dlnpi
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 2 OF 5
	eTheta = (gamma * lambda * eTheta) + dlnpi(features,action);
	theta = theta + (alpha_actor * eTheta);
	// @TODO: Fill in the code for the updates to eTheta, and then the update to theta. This will likely use dlnpi, defined below
}

// Train function to be used when newFeatures correspond to a terminal state
void ActorCritic::train(const VectorXd & features, const int & action, const double & reward)
{
	// You need to implement the update below. The current state is described by the vector "features", the current action is "action"
	// the resulting reward is "reward". The "newFeatures" are not provided because this function is only called when "newFeatures" would
	// describe a terminal state, which has value zero. You can use variables "lambda", "gamma", "alpha_actor", "alpha_critic", which have 
	// already been set for you.

	// Update the critic (update v, the weights for the linear value function approximation, and ev, the e-traces for v)
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 3 OF 5
	// @TODO: Fill in the code for the updates to ev, and then the update to v. Note that v(s) = v.dot(features) and v(s') = v.dot(newFeatures)
	//			Notice that here newFeatures aren't provided because this function is only called during a terminal update the value of the new state is zero.
	double delta = reward - v.dot(features);
	ev = (gamma * lambda * ev) + features;
	v = v + (alpha_critic * delta * ev);
	// Update the actor (update theta, the policy parameters, eTheta, the e-traces for theta). Here you may want to use dlnpi
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 4 OF 5
	// @TODO: Fill in the code for the updates to eTheta, and then the update to theta. This will likely use dlnpi, defined below
	eTheta = (gamma * lambda * eTheta) + dlnpi(features,action);
	theta = theta + (alpha_actor * eTheta);
	// Clear e-traces since it is a terminal update
	ev.setZero();
	eTheta.setZero();
}

VectorXd ActorCritic::dlnpi(const Eigen::VectorXd & features, const int & action)
{
	// Get action probabilities
	VectorXd actionProbabilities = getActionProbabilities(features);

	// Compute dlnpi
	VectorXd result(numFeatures*numActions);
	
	result = VectorXd::Constant(numFeatures*numActions,1.0);
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 5 OF 5
	// @TODO: Fill in the code for computing dlnpi. You must first under stand how getAction
	//		uses the vector \theta, and then you must do math to compute $\frac{\partial \ln(\pi(s,a,a\theta))}{\partial \theta}$.

	return result;
}

VectorXd ActorCritic::getActionProbabilities(const VectorXd & features)
{
	// Get the probability of each action given that we are using softmax action selection
	VectorXd actionProbabilities(numActions);
	for (int a = 0; a < numActions; a++)
		actionProbabilities[a] = features.dot(theta.segment(numFeatures*a, numFeatures));
	actionProbabilities.array() = actionProbabilities.array().exp();
	actionProbabilities /= actionProbabilities.sum();
	return actionProbabilities;
}