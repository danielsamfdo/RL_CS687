#pragma once

#include <stdafx.h>

// Code for timing how long something takes.
class Timer {
public:
	Timer();

	void start();

	// Get the number of milli sec since start was called
	double getMS();

	// Get the number of seconds since start was called
	double getS();

	// Get the number of minutes since start was called
	double getM();

	// Get the number of hours since start was called
	double getH();

private:
	clock_t startTime;
};