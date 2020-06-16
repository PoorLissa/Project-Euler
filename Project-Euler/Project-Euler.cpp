// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "projecteuler.h"

// =======================================================================================================================

int _tmain(int argc, _TCHAR* argv[])
{
	Chrono timer;
	{
		func00();
	}
	timer.end();

	std::cout << std::endl;

	std::cout << " -- Started at   : " << timer.getStartTime();
	std::cout << " -- Finished at  : " << timer.getEndTime();
	std::cout << " -- Elapsed time : " << timer.getElapsedTime() << std::endl;

	std::cout << std::endl;
	std::cout << " -- done -- " << std::endl;

	getchar();

	return 0;
}
// =======================================================================================================================
