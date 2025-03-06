// STD
#include <iostream>
#include <memory>

// BasedCore
#include "BasedCore/BEapp.h"


int main(int argc, char** argv)
{
	int exitCode = EXIT_SUCCESS;

	BEapp app {800, 600, 10, "BasedEngine", BEapp::VULKAN};

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