#include <stdafx.h>

Timer::Timer() {
	// Might as well start now anyway - so the first start call is optional
	start();
}

void Timer::start() {
	startTime = clock();
}

// Get the number of milli sec since start was called
double Timer::getMS() {
	return 1000.0f * (float)(clock() - startTime) / (CLOCKS_PER_SEC);
}

// Get the number of seconds since start was called
double Timer::getS() {
	return (float)(clock() - startTime) / (CLOCKS_PER_SEC);
}

// Get the number of minutes since start was called
double Timer::getM() {
	return (float)(clock() - startTime) / (60.0f * CLOCKS_PER_SEC);
}

// Get the number of hours since start was called
double Timer::getH() {
	return (float)(clock() - startTime) / (3600.0f * CLOCKS_PER_SEC);
}