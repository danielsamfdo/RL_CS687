#pragma once

#include <stdafx.h>

class Pendulum : public Environment {
public:
	Pendulum(
		std::mt19937_64 & generator,
		bool deterministicInitialState = false);
	~Pendulum() override;

	void draw(const bool & realtimePause) override;

	void closeWindow() override;

	Environment::Description getDesc() const override;
	double update(const Thing & a, std::mt19937_64 & generator) override;
	void getObs(Thing & buff, std::mt19937_64 & generator) const override;
	bool inTerminalState() const override;
	void newEpisode(std::mt19937_64 & generator) override;
	Policy * getInitialPolicy() const override;
	Pendulum * clone(std::mt19937_64 & generator) const override;
	const char * getName() const override;

private:
	bool deterministicInitialState;

	// State variables
	double theta;	// Angle of pendulum (0 = down, pi = up)
	double omega;	// Time derivative of theta
	double t;	// Current time

	const double dt = 0.01;		// Time step size
	const double uMax = 5.0;	// Maximum torque in either direction
	const int simSteps = 10;	// Dynamics simulated at dt/simSteps
	const double m = 1;	// Mass
	const double l = 1;	// Length
	const double g = 9.8;	// Gravity
	const double mu = 0.1;	// Friction constant
	const int suggestedMaxT = (int)(20.0 / .01); // 20 seconds

	CImgWindow w;
};