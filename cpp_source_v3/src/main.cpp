#include <stdafx.h>
#include <experimental/CMPSCI687/HW1.hpp>

using namespace std;
using namespace Eigen;

int main(int argc, char * argv[])
{
#ifdef _MSC_VER
	SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_BEGIN);
#endif

	HW1();

#ifdef _CLOSE_MATLAB_ENGINE_ON_TERMINATE
	MatlabEngine::closeMatlab();
#endif
	 
	return 0;
}