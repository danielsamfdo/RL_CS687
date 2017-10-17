#include <stdafx.h>
#include <agents/CrossEntropyMethod.hpp>

using namespace std;
using namespace Eigen;

CrossEntropyMethod::CrossEntropyMethod(
	Policy * p, 
	const int & population, 
	const int & episodesPerPolicy, 
	const int & numElite, 
	const double & sigma, 
	const double & gamma, 
	mt19937_64 & generator)
{
	// Copy over arguments
	this->p = p->clone(generator);
	numParams = p->getNumParams();
	this->theta = p->getParams();
	this->population = population;
	this->episodesPerPolicy = episodesPerPolicy;
	this->numElite = numElite;
	this->sigma = sigma;
	this->Sigma = sigma*MatrixXd::Identity(numParams, numParams);
	this->gamma = gamma;
	curGamma = 1.0;

	// Check that numElite < population, else this won't work
	ensure(numElite < population, "CrossEntropyMethod::CrossEntropyMethod", "Must have numElite < population");

	// Resize the vectors
	samples.resize(population);
	for (int i = 0; i < population; i++)
		samples[i].first.resize(numParams);
	
	// Create the multivariate normal rand distribution
	paramDistribution = new mvnrnd(theta, Sigma);

	sampleThetas(generator);
}

CrossEntropyMethod::~CrossEntropyMethod()
{
	delete p;
	delete paramDistribution;
}

Agent::Description CrossEntropyMethod::getDesc() const
{
	Agent::Description result;
	result.canUpdateBeforeAPrime = true;			// We don't care - we store updates until episode ends anyway
	result.handlesEpisodic = true;
	result.handlesContinuing = false;
	result.handlesFiniteHorizon = true;
	result.handlesIndefiniteHorizon = true;
	result.handlesInfiniteHorizon = true;			// As long as we see many (cut) episodes
	result.handlesDiscreteObservations = true;
	result.handlesContinuousObservations = true;
	result.handlesHybridObservations = true;
	result.handlesDiscreteActions = true;
	result.handlesContinuousActions = false;
	result.handlesHybridActions = false;
	result.handlesStochasticTransitions = true;
	result.handlesStochasticRewards = true;
	result.handlesNonMarkovianObservations = true;
	result.handlesStochasticInitialState = true;
	return result;
}

void CrossEntropyMethod::train(mt19937_64 & generator, const Thing & o, const Thing & a, const double & r)
{
	samples[policyCount].second  += curGamma*r;
	curGamma *= gamma;
	
	// Don't update counters - do that on the newEpisode call, since one might call newEpisode without giving a terminal update
}

void CrossEntropyMethod::train(mt19937_64 & generator, const Thing & o, const Thing & a, const double & r, const Thing & oPrime, const Thing * aPrime)
{
	samples[policyCount].second += curGamma*r;
	curGamma *= gamma;
}

bool CrossEntropyMethod::comparisonOperator(
	const std::pair<Eigen::VectorXd, double> & a,
	const std::pair<Eigen::VectorXd, double> & b)
{
	return a.second > b.second;
}


/*
Function that implements the CEM update. The arguments are:

Output Variables:
1. VectorXd & theta: This vector should be loaded with the new mean vector. Although this is an output, its input value is the current mean parameter vector.
2. MatrixXd & Sigma: This matrix should be loaded with the new covariance matrix. Although this is an output, its input value is the current covariance matrix.

Input Variables:
3. vector<VectorXd> & thetas: This is the vector of policy parameter vectors, theta, that were tried. thetas[i] is a VectorXd containing the i'th parameter vector that was tested.
4. VectorXd & values: This is a vector containing the mean discounted return over episodesPerPolicy trials for the samples parameters. That is, values[i] is the mean discounted return over episodesPerPolicy trials of running thetas[i]
5. const int & numParams: The number of parameters---the length of each vector, thetas[i]
6. const int & numElite: The hyperparameter of CEM: The number of samples that will be labeled as elite
7. const int & population: The hyperparameter of CEM: The number of parameter vectors that are sampled per iteration. That is, the number of i for which thetas[i] and values[i] are defined
8. const int & episodesPerPolicy: The hyperparameter of CEM: The number of episodes that each of the sampled policies is run for when computing the mean discounted returns stored in values

NOTE: thetas and values have already been sorted such that the values vector is in descending order.

This function should load theta and Sigma with the new mean parameter vector and covariance matrix as dictated by the CEM algorithm.
*/
static void CEMUpdate(
	VectorXd & theta, 
	MatrixXd & Sigma, 
	const vector<VectorXd> & thetas,
	const VectorXd & values, 
	const int & numParams, 
	const int & numElite, 
	const int & population, 
	const int & episodesPerPolicy)
{
    theta.setZero();
    Sigma = MatrixXd::Zero(numParams, numParams);
    MatrixXd Update = MatrixXd::Identity(numParams, numParams);
    double epsilon = 10;
    for(int i=0;i<numElite;i++){
        theta += thetas[i];
    }
    theta = theta/numElite;

    for(int i=0;i<numElite;i++) {
        Sigma += (thetas[i] - theta) * ((thetas[i] - theta).transpose());
    }
    Sigma += Update * epsilon;
    Sigma = Sigma/(numElite+epsilon);
}

void CrossEntropyMethod::newEpisode(mt19937_64 & generator)
{
	curGamma = 1;
	episodeCount++;
	if (episodeCount == episodesPerPolicy)
	{
		// Convert sum of returns to average return in Js
		samples[policyCount].second /= (double)episodesPerPolicy;

		// Switch to next policy to sample from
		policyCount++;
		episodeCount = 0;

		if (policyCount == population)
		{
			// Do an update. First, sort the samples
			sort(samples.begin(), samples.end(), comparisonOperator);

			// Unpack into objects that are easier to deal with when not familiar with C++
			vector<VectorXd> thetas(samples.size());
			VectorXd values(samples.size());
			for (int i = 0; i < (int)samples.size(); i++)
			{
				thetas[i] = samples[i].first;
				values[i] = samples[i].second;
			}
			CEMUpdate(theta, Sigma, thetas, values, numParams, numElite, population, episodesPerPolicy);

			// Resample thetas
			sampleThetas(generator); // Resets policyCount, episodeCount, and Js
		}
		else
			p->setParams(samples[policyCount].first);
	}
}

void CrossEntropyMethod::getAction(Thing & buff, const Thing & o, std::mt19937_64 & generator) 
{
	p->getAction(buff, o, generator);
}

CrossEntropyMethod * CrossEntropyMethod::clone(mt19937_64 & generator) const
{
	// Copy, but reset "samples" to avoid the clone having the same first batch of parameters
	CrossEntropyMethod * result = new CrossEntropyMethod(p, population, episodesPerPolicy, numElite, sigma, gamma, generator);
	result->theta = theta;
	result->Sigma = Sigma;
	result->sampleThetas(generator);
	return result;
}

void CrossEntropyMethod::sampleThetas(mt19937_64 & generator)
{
	// Load current distribution parameters
	paramDistribution->set(theta, Sigma);

	// Sample
	for (int i = 0; i < population; i++)
	{
		paramDistribution->sample(samples[i].first, generator);
		samples[i].second = 0;
	}

	// Set counters to zero and clear Js
	policyCount = episodeCount = 0;
	
	// Set the policy parameters to the first sample
	p->setParams(samples[0].first);
}