#include <stdafx.hpp>

using namespace Eigen;
using namespace std;

CartPole::CartPole()
{
	theta = 0;
	omega = 0;
	theta = 0;
	v = 0;
	x = 0;
	t = 0;
}

CartPole::~CartPole() {}

VectorXd CartPole::getState()
{
	VectorXd result(4);
	result[0] = x;
	result[1] = v;
	result[2] = theta;
	result[3] = omega;
	return result;
}

// Returns an integer that is +1, 0, or -1
static int sign(double x)
{
	return (x > 0) - (x < 0);
}

// Bound x between min and max
static double bound(const double & x, const double & minValue, const double & maxValue)
{
	return std::min<double>(maxValue, std::max<double>(minValue, x));
}

static double Mod(const double & x, const double & y) {
	if (0. == y) return x;
	double m = x - y * (double)std::floor(x / y);
	// handle boundary cases resulted from floating-point cut off:
	if (y > 0) {
		if (m >= y)
			return 0;
		if (m < 0) {
			if (y + m == y) return 0;
			else return (y + m);
		}
	}
	else
	{
		if (m <= y) return 0;
		if (m > 0) {
			if (y + m == y) return 0;
			else return (y + m);
		}
	}
	return m;
}

// wrap [rad] angle to [-PI..PI)
static double wrapPosNegPI(const double & theta) {
	return Mod((double)theta + M_PI, (double)2.0*M_PI) - (double)M_PI;
}

double CartPole::update(int action)
{
	double F = action*uMax + (action - 1)*uMax;

	// Run simSteps short steps, each using a forward Euler approximation of 
	// the dynamics.
	double omegaDot, vDot, subDt = dt / (double)simSteps;
	for (int i = 0; i < simSteps; i++)
	{
		// Update once with timestep subDt
		// First compute the time derivatives (inserted the cos(theta) at the 
		// beginning)
		omegaDot = (g*sin(theta) + cos(theta)*(muc*sign(v) - F - m*l*omega*omega*sin(theta)) / (m + mc) - mup*omega / (m*l)) / (l*(4.0 / 3.0 - m / (m + mc)*cos(theta)*cos(theta)));
		vDot = (F + m*l*(omega*omega*sin(theta) - omegaDot*cos(theta)) - muc*sign(v)) / (m + mc);

		// Update by adding the derivatives
		theta += subDt*omega;
		omega += subDt*omegaDot;
		x += subDt*v;
		v += subDt*vDot;

		// Keep in a nice range (-pi to pi so we can do the check below)
		theta = wrapPosNegPI(theta);

		t += subDt;
	}

	bound(x, -2.4, 2.4);

	if (inTerminalState())
		return -1;
	else
		return 1;
}

bool CartPole::inTerminalState()
{
	// This t bound is from getDesc, suggestedMaxT
	return ((fabs(theta) > M_PI / 15.0) || (fabs(x) >= 2.4) || (t >= 20.0 + 10 * dt));
}

void CartPole::newEpisode()
{
	theta = 0;
	omega = 0;
	theta = 0;
	v = 0;
	x = 0;
	t = 0;
}

int CartPole::getStateDim()
{
	return 4;
}

int CartPole::getNumActions()
{
	return 2;
}

VectorXd CartPole::getMinState()
{
	VectorXd result(4);
	result[0] = -2.4;
	result[1] = -6;
	result[2] = -12 * M_PI / 180.0;
	result[3] = -6;
	return result;
}

VectorXd CartPole::getMaxState()
{
	VectorXd result(4);
	result[0] = 2.4;
	result[1] = 6;
	result[2] = 12 * M_PI / 180.0;
	result[3] = 6;
	return result;
}

double CartPole::getGamma()
{
	return 1.0;
}