#include <stdafx.h>
#include <environments/Pendulum.hpp>
#include <policies/LinearNormal.hpp>

using namespace std;
using namespace Eigen;

Pendulum::Pendulum(mt19937_64 & generator, bool deterministicInitialState)
{
	this->deterministicInitialState = deterministicInitialState;
	newEpisode(generator);
}

Pendulum::~Pendulum() {}

void Pendulum::draw(const bool & realtimePause)
{
	if (w.userClosed())
		return;

	// Make sure the window exists
	if (w.isClosed())
		w.open(_SUGGESTED_WIN_WIDTH, _SUGGESTED_WIN_HEIGHT, "Pendulum"); // The window is closed - open it (the user didn't close it)
	w.clearScreen();

	// The arm will be one unit long
	double unit = min(_SUGGESTED_WIN_WIDTH, _SUGGESTED_WIN_HEIGHT) / 3.0;
	int midX = _SUGGESTED_WIN_WIDTH / 2,
		midY = _SUGGESTED_WIN_HEIGHT / 2,
		r = (int)(0.2*unit);
	int handX = (int)(midX + sin(theta)*unit),
		handY = (int)(midY - cos(theta)*unit);
	w.drawCircle(midX, midY, r, BLUE_ZAFFRE);
	w.drawLine(midX, midY, handX, handY, BLUE_ZAFFRE);
	string s = "Time (s): " + to_string(t);
	w.drawText(10, 20, s);

	// Actually draw
	w.update();

	// Pause if realtime
	if ((realtimePause) && (w.isClosed() == false))
		pause((unsigned int)(dt*1000));
}

void Pendulum::closeWindow()
{
	w.close();
}

Environment::Description Pendulum::getDesc() const
{
	Environment::Description result;
	result.episodic = true;

	// No guarantee an episode will end (could never reach goal)
	result.finiteHorizon = false;
	result.indefiniteHorizon = false;
	result.infiniteHorizon = true;

	result.suggestedMaxT = suggestedMaxT;
	if (deterministicInitialState)
		result.suggestedMaxEps = 2000;
	else
		result.suggestedMaxEps = 20000;

	result.gamma = 1.0;

	result.actDesc.numDisc = 0;
	result.actDesc.contDim = 1;
	result.actDesc.contRange.first.resize(1);
	result.actDesc.contRange.second.resize(1);
	result.actDesc.contRange.first[0] = -uMax;
	result.actDesc.contRange.second[0] = uMax;

	result.obsDesc.numDisc = 0;
	result.obsDesc.contDim = 2;
	result.obsDesc.contRange.first.resize(2);
	result.obsDesc.contRange.second.resize(2);
	result.obsDesc.contRange.first[0] = -M_PI;
	result.obsDesc.contRange.first[1] = -3.0*M_PI;
	result.obsDesc.contRange.second[0] = M_PI;
	result.obsDesc.contRange.second[1] = 3.0*M_PI;

	result.stochasticStateTransitions = false;
	result.stochasticRewards = false;
	result.MarkovianObservations = true;
	result.deterministicInitialState = deterministicInitialState;
	result.minReward = -2;
	result.maxReward = 2;
	result.minReturn = -2 * suggestedMaxT;
	result.maxReturn = 2 * suggestedMaxT;

	// Let plot auto-set it
	if (deterministicInitialState)
	{
		result.suggestedPlotMinPerformance = -1700;
		result.suggestedPlotMaxPerformance = 1700;
	}
	else
	{
		result.suggestedPlotMinPerformance = -1500;
		result.suggestedPlotMaxPerformance = 1900;
	}

	return result;
}

double Pendulum::update(const Thing & a, std::mt19937_64 & generator)
{
	double u = a.cont(0);

	// Double check that the action is within legal bounds, and force it to be
	if (u < -uMax)
		u = -uMax;
	if (u > uMax)
		u = uMax;

	// Run simSteps short steps, each using a forward Euler approximation of 
	// the dynamics.
	double thetaDot, omegaDot, subDt = dt / (double)simSteps;
	double tUp = 0;
	for (int i = 0; i < simSteps; i++)
	{
		// Update once with timestep subDt

		// First compute the time derivatives
		thetaDot = omega;

		// From code, theta = 0 is DOWN. The equation in the paper has a 
		// different sign on one term because of theta difference
		omegaDot = (-mu*omega - m*g*l*sin(theta) + u) / (m*l*l);

		// Update by adding the derivatives
		theta += subDt*thetaDot;
		omega += subDt*omegaDot;

		// temp stores the angle-wrapped theta to check if vertical
		double temp = wrapPosNegPI(theta);

		// Check if the pendulum is within 45 degrees of vertical
		if (temp*temp > 9.0*M_PI*M_PI / 16.0)
			tUp += subDt;
	}

	// Keep angular velocity in range
	bound(omega, -3.0*M_PI, 3.0*M_PI);

	t += dt; // Increment the time counter

			 // Compute the reward
	double Su = 0;

	// Reward function that avoids overrotation by making
	// the spin-fast policy bad without the over-rotated reward-hack
	double Rx = -cos(theta) - (omega*omega) / 100.0;

	return Rx - Su;
}

void Pendulum::getObs(Thing & buff, std::mt19937_64 & generator) const
{
	buff.disc = 0;
	buff.cont.resize(2);
	buff.cont[0] = wrapPosNegPI(theta);
	buff.cont[1] = omega;
}

bool Pendulum::inTerminalState() const
{
	return t >= 20.0; // Time is up!
}

void Pendulum::newEpisode(std::mt19937_64 & generator)
{
	if (deterministicInitialState)
		theta = 0;
	else
		theta = uniform_real_distribution<double>(-M_PI, M_PI)(generator);
	omega = 0;
	t = 0;
}

Policy * Pendulum::getInitialPolicy() const
{
	Environment::Description envDesc = getDesc();
	int iOrder = 6, dOrder = 5;
	double sigma = 1.0;
	FourierBasis fb(envDesc.obsDesc.contDim, envDesc.obsDesc.contRange.first, envDesc.obsDesc.contRange.second, iOrder, dOrder);
	return new LinearNormal(envDesc.obsDesc, true, sigma, &fb);
}

Pendulum* Pendulum::clone(std::mt19937_64 & generator) const
{
	return new Pendulum(*this);
}

const char * Pendulum::getName() const
{
	if (deterministicInitialState)
		return "Pendulum (Deterministic Start)";
	else
		return "Pendulum (Stochastic Start)";
}