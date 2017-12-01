#pragma once

#include <stdafx.hpp>

// Standard mountain car domain
class MountainCar : public Environment
{
public:
	MountainCar();
	~MountainCar();
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
	const double minX = -1.2;
	const double maxX = 0.5;
	const double minXDot = -0.07;
	const double maxXDot = 0.07;

	Eigen::VectorXd state; // [2] - x and xDot

	int t;
};