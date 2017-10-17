#pragma once

#include <stdafx.h>

// Fast RNG that returns integers between 0 and max (inclusive).
// Returns number from 0-max.
// This generator has a period of 2^96 - 1
class MarsagliaGenerator
{
public:
	MarsagliaGenerator(const int & RNGSeed, const unsigned long & max);

	unsigned long sample();

private:
	unsigned long x;
	unsigned long y;
	unsigned long z;

	unsigned long max;
};