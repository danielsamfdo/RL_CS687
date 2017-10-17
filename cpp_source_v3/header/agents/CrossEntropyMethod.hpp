#pragma once

#include <stdafx.h>

/*
Cross-Entropy Method, as defined in Freek Stulp's paper:
http://icml.cc/2012/papers/171.pdf
*/
class CrossEntropyMethod : public Agent {
public:
	CrossEntropyMethod(
		Policy * p,				// Initial parameterized policy. A clone will be made, so caller can destroy this object immediately after this function call
		const int & population,	// Number of policy parameters to sample per iteration. K in Freek Stulp's writeup
		const int & episodesPerPolicy,	// Number of episodes to run to evaluate each policy
		const int & numElite,			// The top this-many parameters are declared "elite"
		const double & sigma,	// Initial variance of random noise added to each policy parameter
		const double & gamma,
		std::mt19937_64 & generator);
	CrossEntropyMethod(const CrossEntropyMethod & rhs) = delete; // We need a generator to change sample to avoid common random numbers. Use clone function
	CrossEntropyMethod & operator=(const CrossEntropyMethod & rhs) = delete; // We need generator to change sample to avoid common random numbers. Use cloen function.

	~CrossEntropyMethod() override;

	Agent::Description getDesc() const override;
	void train(std::mt19937_64 & generator, const Thing & o, const Thing & a, const double & r) override;
	void train(std::mt19937_64 & generator, const Thing & o, const Thing & a, const double & r, const Thing & oPrime, const Thing * aPrime = nullptr) override;
	void newEpisode(std::mt19937_64 & generator) override;
	void getAction(Thing & buff, const Thing & o, std::mt19937_64 & generator) override;
	CrossEntropyMethod * clone(std::mt19937_64 & generator) const override;

private:
	Policy * p;				// The parameterized policy
	int numParams;			// The number of parameters that the parameterized policy cas
	int population;			// Number of policy parameters to sample per iteration. K in Freek Stulp's writeup
	int episodesPerPolicy;	// Number of episodes to run to evaluate each policy
	int numElite;			// The top this-many parameters are declared "elite"

	double sigma;			// Initial standard deviation of distribution over parameters

	Eigen::VectorXd theta;	// Mean policy parameters
	Eigen::MatrixXd Sigma;	// Covariance matrix for distirbution over policy parameters

	int policyCount;		// Counts up to population, saying which of the sampled policy parameter vectors we are using
	int episodeCount;		// Counts up to episodesPerPolicy, saying which episode we are on for the policyCount'th policy
	std::vector<std::pair<Eigen::VectorXd, double>> samples;	// [population]. First element is parameter vector, second is estimate of performance of that parameter vector. I.e., (theta, J(theta))

	double gamma;
	double curGamma;

	mvnrnd * paramDistribution;

	void sampleThetas(std::mt19937_64 & generator);	// Loads thetas with samples from N(theta,Sigma)

	// Function for comparing two samples
	static bool comparisonOperator(
		const std::pair<Eigen::VectorXd, double> & a, 
		const std::pair<Eigen::VectorXd, double> & b);
};