#pragma once

#include <stdafx.h>

// Standard 5x5 gridworld used as an example in CMPSCI 687 at UMass (Fall 2017 at least)
class Gridworld687 : public Environment {
public:
	Gridworld687();
	~Gridworld687() override;

	void draw(const bool & realtimePause) override;
	void closeWindow() override;

	Environment::Description getDesc() const override;
	double update(const Thing & a, std::mt19937_64 & generator) override;
	void getObs(Thing & buff, std::mt19937_64 & generator) const override;
	bool inTerminalState() const override;
	void newEpisode(std::mt19937_64 & generator) override;
	Policy * getInitialPolicy() const override;
	Gridworld687 * clone(std::mt19937_64 & generator) const override;
	const char * getName() const override;

private:
	const int size = 5;
	int x;
	int y;
	
	CImgWindow w;
};