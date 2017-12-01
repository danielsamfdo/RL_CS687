#pragma once

#include <stdafx.hpp>

// Pure abstract base class for all environments
class Environment
{
public:
	virtual ~Environment() {};
	virtual Eigen::VectorXd getState() = 0;
	virtual double update(int action) = 0;
	virtual bool inTerminalState() = 0;
	virtual void newEpisode() = 0;
	virtual int getStateDim() = 0;
	virtual int getNumActions() = 0;
	virtual Eigen::VectorXd getMinState() = 0;
	virtual Eigen::VectorXd getMaxState() = 0;
	virtual double getGamma() = 0;
};
