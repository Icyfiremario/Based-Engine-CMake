// STD
#include <iostream>

// Local
#include "BasedGL/BasedGL.h"

int main(int argc, char** argv)
{
	int exitCode = EXIT_SUCCESS;
	
	try
	{
		
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		exitCode = EXIT_FAILURE;
	}
	
	return exitCode;
}