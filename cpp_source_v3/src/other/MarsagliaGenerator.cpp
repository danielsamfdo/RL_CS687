#include <stdafx.h>

using namespace std;

MarsagliaGenerator::MarsagliaGenerator(const int & RNGSeed, const unsigned long & max) {
	this->max = max;
	mt19937_64 generator(RNGSeed);
	uniform_int_distribution<int> d(INT_MIN, INT_MAX);
	x = d(generator);
	y = d(generator);
	z = d(generator);
}

unsigned long MarsagliaGenerator::sample() {
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;
	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;
	return z % (max + 1);
}