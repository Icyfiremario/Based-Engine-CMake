// Check OS
#include <filesystem>
#ifdef __unix__
#ifndef UNIX
#define UNIX
#endif
#elif defined(_WIN32) || defined(WIN32)
#ifndef WINDOWS
#define WINDOWS
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#warning "Apple support is limited."
#ifndef APPLE
#define APPLE
#endif
#endif

// STD
#include <iostream>
#include <fstream>

// Plog
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

// BasedCore
#include "BasedCore/Managers/BEAppManager.h"

int main(int argc, char** argv)
{
    int exitCode = EXIT_SUCCESS;

    std::filesystem::create_directory("logs");
    std::filesystem::remove("logs/latest.log");
    plog::init(plog::debug, "logs/latest.log");

    try
    {
        BasedEngine::Managers::BEAppManager* appManager = BasedEngine::Managers::BEAppManager::getInstance();
        PLOGI << "Creating app object.";
        appManager->createApp(600, 400, "Based Vulkan");
        PLOGI << "Running app.";
        appManager->getApp()->run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        PLOGF << e.what();
        exitCode = EXIT_FAILURE;
    }

    return exitCode;
}
