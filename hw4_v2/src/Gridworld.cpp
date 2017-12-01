#include <Gridworld.hpp>
#include <stdlib.h>

using namespace Eigen;
using namespace std;

Gridworld::Gridworld()
{
	newEpisode();
}

Gridworld::~Gridworld() {}

VectorXd Gridworld::getState()
{
	int i = x*size + y;
	VectorXd result = VectorXd::Zero(size*size - 1);
	result[i] = 1.0;
	return result;
}

double Gridworld::update(int action)
{
	switch (action)
	{
	case 0:
		y--;
		break;
	case 1:
		y++;
		break;
	case 2:
		x--;
		break;
	case 3:
		x++;
		break;
	}
	x = min<int>(x, size - 1);
	x = max<int>(x, 0);
	y = min<int>(y, size - 1);
	y = max<int>(y, 0);
	t++;
	return -1.0;
}

bool Gridworld::inTerminalState()
{
	return (((x == size - 1) && (y == size - 1)) || (t >= 1000));
}

void Gridworld::newEpisode()
{
	t = x = y = 0;
}

int Gridworld::getStateDim()
{
	return size*size - 1;
}

int Gridworld::getNumActions()
{
	return 4;
}

VectorXd Gridworld::getMinState()
{
	return VectorXd::Zero(size*size - 1);
}

VectorXd Gridworld::getMaxState()
{
	return VectorXd::Ones(size*size - 1);
}

double Gridworld::getGamma()
{
	return 1.0;
}