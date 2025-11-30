// Check OS
#ifdef __unix__
#define UNIX
#elif defined(_WIN32) || defined(WIN32)
#define WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#error "Mac OS X is not supported!"
#endif

// STD
#include <iostream>

// Plog (Later)
//#include <plog/Log.h>

// BasedCore
#include "BasedCore/BEApp.h"

int main()
{
	int exitCode = EXIT_SUCCESS;

	try
	{
		BEApp app{};
		std::cout << "What fuck\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		exitCode = EXIT_FAILURE;
	}
	
	return exitCode;
}