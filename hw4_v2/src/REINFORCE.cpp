#include <stdafx.hpp>

using namespace Eigen;
using namespace std;

REINFORCE::REINFORCE(int numFeatures, int numActions, double alpha_actor, double alpha_baseline, double gamma)
{
	// Copy over the provided arguments
	this->numActions = numActions;
	this->numFeatures = numFeatures;
	this->alpha_actor = alpha_actor;
	this->alpha_baseline = alpha_baseline;
	this->gamma = gamma;

	// Initialize the policy parameters to zero
	theta = VectorXd::Zero(numFeatures*numActions);

	// Initialize the baseline as zero
	baseline = 0;

	// Initialize the history of this episode to be empty
	rewardHistory.resize(0);
	actionHistory.resize(0);
	featureHistory.resize(0);

	// Create distribution that will be used during action selection
	d = uniform_real_distribution<double>(0, 1);
}

REINFORCE::~REINFORCE() {}

int REINFORCE::getAction(const Eigen::VectorXd & features, std::mt19937_64 & generator)
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
void REINFORCE::train(const VectorXd & features, const int & action, const double & reward, const VectorXd & newFeatures)
{
	// Non-termial update. Just add new experiences to the history of this episode
	featureHistory.push_back(features);
	rewardHistory.push_back(reward);
	actionHistory.push_back(action);
}

// Train function to be used when newFeatures correspond to a terminal state
void REINFORCE::train(const VectorXd & features, const int & action, const double & reward)
{
	// Add this last experience to the history of this episode
	featureHistory.push_back(features);
	rewardHistory.push_back(reward);
	actionHistory.push_back(action);

	// Update the policy. Start by computing the returns from each time step
	int len = (int)rewardHistory.size();
	VectorXd returns(len);
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 1 OF 3
	// @TODO: Load returns[t] with the discounted return starting with reward rewardHistory[t].
	//		Notice that rewardHistory should also be of length "len" and stores the observed rewards at each time step.
	for(int t=len-1;t>=0;t--){
		if(t==len-1){
			returns[t] = rewardHistory[t];
		}else{
			returns[t] = returns[t] + (gamma*rewardHistory[t+1]);
		}
	}

	// Get errors, which are the returns from each time step minus the current baseline
	VectorXd errors = returns.array() - baseline;
	
	// Update policy by looping over time steps and performing REINFORCE update (using a constant baseline) at each step
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 2 OF 3
	// @TODO: Notice that you have "errors" computed for you, which is G-baseline. You should likely also make use of the dlnpi function defined below
	for(int t=0;t<len;t++){
		VectorXd feature = featureHistory[t];
		int a = actionHistory[t];
		double r = rewardHistory[t];
		theta.segment(numFeatures*action, numFeatures) += (alpha_actor * (returns[t]) * dlnpi(feature,a).segment(numFeatures*action, numFeatures));
	}
	// Update baseline
	baseline = (1 - alpha_baseline)*baseline + alpha_baseline*returns.mean();

	// New episode will begin, so clear the stored history
	rewardHistory.resize(0);
	actionHistory.resize(0);
	featureHistory.resize(0);
}

VectorXd REINFORCE::dlnpi(const Eigen::VectorXd & features, const int & action)
{
	// Get action probabilities
	VectorXd actionProbabilities = getActionProbabilities(features);

	// Compute dlnpi
	VectorXd result(numFeatures*numActions);
	// VectorXd z(numFeatures*numActions);
	// for(int i=0;i<numFeatures*numActions;i++){
	// 	z[i] = 1;
	// }
	result = VectorXd::Constant(numFeatures*numActions,1.0) - theta;
	// @TODO	@TODO	@TODO	@TODO	@TODO	@TODO	@TODO @TODO	@TODO
	// @TODO 3 OF 3
	// @TODO: Fill in the code for computing dlnpi. You must first under stand how getAction
	//		uses the vector \theta, and then you must do math to compute $\frac{\partial \ln(\pi(s,a,a\theta))}{\partial \theta}$.

	return result;
}

VectorXd REINFORCE::getActionProbabilities(const VectorXd & features)
{
	// Get the probability of each action given that we are using softmax action selection
	VectorXd actionProbabilities(numActions);
	for (int a = 0; a < numActions; a++)
		actionProbabilities[a] = features.dot(theta.segment(numFeatures*a, numFeatures));
	actionProbabilities.array() = actionProbabilities.array().exp();
	actionProbabilities /= actionProbabilities.sum();
	return actionProbabilities;
}