// Check OS
#ifdef __unix__
	#define UNIX
#elif defined(_WIN32) || defined(WIN32)
	#define WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
	#error "Mac OS X is not supported!"
#endif

// STD
#include <iostream>

// BasedCore
#include "BasedCore/BEapp.h"


int main()
{
	int exitCode = EXIT_SUCCESS;

	BEapp app {800, 600, 10, "BasedEngine", BasedCore::OPENGL};

	try
	{
		app.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		exitCode = EXIT_FAILURE;
	}
	
	return exitCode;
}