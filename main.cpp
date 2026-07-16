// Check OS
#ifdef __unix__
#define UNIX
#elif defined(_WIN32) || defined(WIN32)
#define WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#warning "Apple support is limited."
#define APPLE
#endif

// STD
#include <iostream>

// Plog (Later)
#include <plog/Log.h>

// BasedCore
#include "BasedCore/Managers/BEAppManager.h"

int main()
{
    int exitCode = EXIT_SUCCESS;

    try
    {
        BEAppManager* appManager = BEAppManager::getInstance();
        appManager->createApp(600, 400, "Based Vulkan");
        appManager->getApp()->run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        exitCode = EXIT_FAILURE;
    }

    return exitCode;
}
