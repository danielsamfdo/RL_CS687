#pragma once

#include <stdafx.h>

Eigen::VectorXd gradientDescent(
	double(*f)(const Eigen::VectorXd &, void*[]),	// Function to be minimized
	Eigen::VectorXd(*grad)(const Eigen::VectorXd &, void*[]), // gradient of function
	Eigen::VectorXd & initialSolution, // Initial solution
	void* d[] = nullptr, // Extra information passed to function (e.g., data set)
	const int & maxSteps = 1000, // Maximum steps before termination
	const double & maxStepSize = 100.0); // Maximum step size - this is divided by 2 100 times, so starting with 100 can still allow for small steps.