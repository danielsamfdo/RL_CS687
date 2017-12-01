#pragma once

#include <stdafx.hpp>

// A simple gridworld environment
class Gridworld : public Environment
{
public:
	Gridworld();
	~Gridworld();
	Eigen::VectorXd getState() override;
	double update(int action) override;
	bool inTerminalState() override;
	void newEpisode() override;
	int getStateDim() override;
	int getNumActions() override;
	Eigen::VectorXd getMinState() override;
	Eigen::VectorXd getMaxState() override;
	double getGamma() override;

private:
	const int size = 5;
	int x;
	int y;
	int t;
};