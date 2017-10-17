#include <stdafx.h>
#include <environments/Gridworld687.hpp>
#include <policies/TabularSoftmax.hpp>

using namespace std;
using namespace Eigen;

Gridworld687::Gridworld687()
{
	x = y = 0;
}

Gridworld687::~Gridworld687() {}

void Gridworld687::draw(const bool & realtimePause)
{
	if (w.userClosed())
		return;

	// Make sure the window exists
	if (w.isClosed())
		w.open(_SUGGESTED_WIN_WIDTH, _SUGGESTED_WIN_HEIGHT, "Gridworld687"); // The window is closed - open it (the user didn't close it)
	
	// Clear window
	w.drawRectangle(0, 0, _SUGGESTED_WIN_WIDTH, _SUGGESTED_WIN_HEIGHT, WHITE, true);

	// Get cell width and height
	int cellWidth = _SUGGESTED_WIN_WIDTH / size,
		cellHeight = _SUGGESTED_WIN_HEIGHT / size;

	// Draw the grid
	for (int i = 1; i < size; i++)
	{
		w.drawLine(i*cellWidth, 0, i*cellWidth, _SUGGESTED_WIN_HEIGHT);
		w.drawLine(0, i*cellHeight, _SUGGESTED_WIN_WIDTH, i*cellHeight);
	}

	// Fill in the obstacles
	w.drawRectangle(2 * cellWidth, 2 * cellHeight, 3 * cellWidth, 4 * cellHeight, BLACK, true);

	// Draw the "spikes"
	w.drawRectangle(2 * cellWidth, 4 * cellHeight, 3 * cellWidth, 5 * cellHeight, BLACK, true, 0.5);

	// Draw the agent
	int radius = min(cellWidth, cellHeight) / 3;
	w.drawCircle(x*cellWidth + cellWidth / 2, y*cellHeight + cellHeight / 2, radius, RED, true);

	// Update the window
	w.update();

	// Pause if realtime
	if ((realtimePause) && (w.isClosed() == false))
		pause(200);
}

void Gridworld687::closeWindow()
{
	w.close();
}

Environment::Description Gridworld687::getDesc() const
{
	Environment::Description result;
	result.episodic = true;

	// No guarantee an episode will end (could never reach goal)
	result.finiteHorizon = false;
	result.indefiniteHorizon = false;
	result.infiniteHorizon = true;

	result.suggestedMaxT = 10000;
	result.suggestedMaxEps = 1000; // 50 really, but for Cross-Entropy method we make it more

	result.gamma = 0.95;
	
	result.actDesc = ThingDesc(4);
	result.obsDesc = ThingDesc(size*size - 1);
	
	result.stochasticStateTransitions = false;
	result.stochasticRewards = false;
	result.MarkovianObservations = true;
	result.deterministicInitialState = true;
	result.minReward = -1;
	result.maxReward = 0;
	result.minReturn = -INFINITY;
	result.maxReturn = -(size + size - 2);

	result.suggestedPlotMinPerformance = -4;
	result.suggestedPlotMaxPerformance = 8;

	return result;
}

double Gridworld687::update(const Thing & a, mt19937_64 & generator)
{
	ensure((a.disc >= 0) && (a.disc < 4), "Gridworld687::update", "Invalid action.");

	int newX = x, newY = y;
	switch (a.disc)
	{
	case 0:
		newY--;
		break;
	case 1:
		newY++;
		break;
	case 2:
		newX--;
		break;
	case 3:
		newX++;
		break;
	}
	// Check for leaving grid
	newX = min<int>(newX, size - 1);
	newX = max<int>(newX, 0);
	newY = min<int>(newY, size - 1);
	newY = max<int>(newY, 0);

	// Check for hitting an obstacle
	if (((newX == 2) && (newY == 2)) ||
		((newX == 2) && (newY == 3)))
	{
		newX = x;
		newY = y;
	}

	x = newX;
	y = newY;

	if ((x == 2) && (y == 4))
		return -10;
	if (inTerminalState())
		return 10;

	return 0;
}

void Gridworld687::getObs(Thing & buff, std::mt19937_64 & generator) const
{
	buff = x*size + y;
}

bool Gridworld687::inTerminalState() const
{
	return ((x == size - 1) && (y == size - 1));
}

void Gridworld687::newEpisode(mt19937_64 & generator)
{
	x = y = 0;
}

Policy * Gridworld687::getInitialPolicy() const
{
	return new TabularSoftmax(size*size - 1, 4);
}

Gridworld687 * Gridworld687::clone(std::mt19937_64 & generator) const
{
	return new Gridworld687(*this);
}

const char * Gridworld687::getName() const
{
	return "Gridworld-687";
}