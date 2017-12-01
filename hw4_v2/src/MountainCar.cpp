#include <stdafx.hpp>

using namespace Eigen;
using namespace std;

MountainCar::MountainCar()
{
	state.resize(2);
	state << -0.5, 0;
	t = 0;
}

MountainCar::~MountainCar() {}

VectorXd MountainCar::getState()
{
	return state;
}

double MountainCar::update(int action)
{
	double x = state[0], xDot = state[1];

	// Conver act to a double in {-1, 0, 1}
	double u = (double)action - 1.0;

	// Update xDot first
	xDot = xDot + 0.001*u - 0.0025*cos(3.0*x);
	if (xDot < minXDot)
		xDot = minXDot;
	if (xDot > maxXDot)
		xDot = maxXDot;

	// Update x
	x = x + xDot;

	if (x < minX)
	{
		x = minX;
		xDot = 0;
	}
	if (x > maxX)
		x = maxX;

	// Store x and xDot in the state
	state << x, xDot;

	// Increment time counter
	t++;

	return -1;
}

bool MountainCar::inTerminalState()
{
	return ((state[0] >= maxX) || (t >= 5000));
}

void MountainCar::newEpisode()
{
	state << -0.5, 0;
	t = 0;
}

int MountainCar::getStateDim()
{
	return 2;
}

int MountainCar::getNumActions()
{
	return 3;
}

VectorXd MountainCar::getMinState()
{
	VectorXd result(2);
	result[0] = minX;
	result[1] = minXDot;
	return result;
}

VectorXd MountainCar::getMaxState()
{
	VectorXd result(2);
	result[0] = maxX;
	result[1] = maxXDot;
	return result;
}

double MountainCar::getGamma()
{
	return 1.0;
}