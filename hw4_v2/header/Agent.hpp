#pragma once

#include <stdafx.hpp>

// Pure abstract base class for all agents
class Agent
{
public:
	virtual ~Agent() {};
	virtual int getAction(const Eigen::VectorXd & state, std::mt19937_64 & generator) = 0;
	virtual void train(const Eigen::VectorXd & features, const int & action, const double & reward, const Eigen::VectorXd & newFeatures) = 0;
	virtual void train(const Eigen::VectorXd & features, const int & action, const double & reward) = 0;
};