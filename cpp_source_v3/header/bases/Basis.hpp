#pragma once

#include <stdafx.h>

class Basis
{
public:
	Basis();
	virtual ~Basis();

	////////////////////////////////////////////////////////////////////////////
	// Pure Virtual functions
	////////////////////////////////////////////////////////////////////////////
	// How many outputs?
	virtual int getNumOutputs() const = 0;

	// Produce features given input x and store in buff
	virtual void basify(Eigen::VectorXd & buff, const Eigen::VectorXd & x) const = 0;

	// Copy constructor
	virtual Basis * clone() const = 0;
};