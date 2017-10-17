#pragma once

#include <stdafx.h>

class ThingDesc;

// Object used for states, actions, and observations
class Thing {
public:
	Thing();
	Thing(const Thing & rhs);
	Thing & operator=(const Thing & rhs);
	Thing & operator=(const int & rhs);
	Thing & operator=(const Eigen::VectorXd & rhs);
	Thing(int disc);
	Thing(const Eigen::VectorXd & cont);
	Thing(int disc, const Eigen::VectorXd & cont);

	friend std::ostream & operator <<(std::ostream &out, const Thing &t);
	friend std::istream & operator >>(std::istream  &in, Thing & t);

	int disc;				// Discrete component
	Eigen::VectorXd cont;	// Continuous component
};

// Description of a thing
class ThingDesc {
public:
	ThingDesc();
	ThingDesc(const ThingDesc & rhs);
	ThingDesc & operator=(const ThingDesc & rhs);
	ThingDesc(int numDisc);
	ThingDesc(
		int numDisc, 
		int contDim, 
		std::pair<Eigen::VectorXd, Eigen::VectorXd> contRange);

	friend std::ostream & operator <<(std::ostream &out, const ThingDesc &t);
	friend std::istream & operator >>(std::istream  &in, ThingDesc & t);

	int numDisc;			// Number of possible discrete values
	int contDim;			// Size of the cont vector
	std::pair<Eigen::VectorXd, Eigen::VectorXd> contRange;	// Range of cont (for elements that are bounded)
};