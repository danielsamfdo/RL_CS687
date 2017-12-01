#pragma once

#include <stdafx.hpp>

// Standard cart-pole environment
class CartPole : public Environment
{
public:
	CartPole();
	~CartPole();
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
	// Standard parameters for the CartPole domain
	const double  dt = 0.02;
	const int simSteps = 1;
	const double uMax = 10.0;
	const double l = 0.5;
	const double g = 9.8;
	const double m = 0.1;
	const double mc = 1;
	const double muc = 0.0005;		// Value from Andy's paper, not RLGlue, which used zero
	const double mup = 0.000002;	// Value from Andy's paper, not RLGlue, which used zero
	const double xMax = 3.0;

	// State variables
	double  x;
	double  v;
	double  theta;
	double  omega;

	// Other variables to track
	double t;
};