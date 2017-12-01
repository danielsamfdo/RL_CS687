#pragma once

#define _USE_MATH_DEFINES	// Have math.h include constants like M_PI

#include <stdlib.h>
#include <iostream>
#include <Eigen/Dense>
#include <math.h>
#include <fstream>
#include <random>
#include <vector>
#include <stdarg.h>			// For variadic functions

#include <FourierBasis.hpp>
#include <Environment.hpp>
#include <Gridworld.hpp>
#include <Acrobot.hpp>
#include <CartPole.hpp>
#include <MountainCar.hpp>
#include <Agent.hpp>
#include <QLambda.hpp>
#include <ActorCritic.hpp>
#include <REINFORCE.hpp>