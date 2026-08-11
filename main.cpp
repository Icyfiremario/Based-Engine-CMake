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
#warning "Apple support is unproperly tested. Proceed knowing that the engine may not work."
#ifndef APPLE
#define APPLE
#endif
#endif

// STD
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <sstream>
#include <format>

// Plog
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

// libzip
#include <zip.h>

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
        appManager->createApp(800, 600, "Based Vulkan");
        //appManager->createApp(800, 600, "Based DirectX", BasedEngine::DIRECTX);
        PLOGI << "Running app.";
        appManager->getApp()->run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        exitCode = EXIT_FAILURE;
    }

    // Generate log name for archiving
    std::stringstream filename;

    const auto now = std::chrono::system_clock::now();
    const auto now_sec = std::chrono::system_clock::to_time_t(now);

    std::tm* local_tm = std::localtime(&now_sec);

    std::string date = std::format("{:%F}", now);
    std::string time = std::format("{:02}-{:02}-{:02}", local_tm->tm_hour, local_tm->tm_min, local_tm->tm_sec);

    filename << "./logs/" << date << '-' << time << ".log";

    std::filesystem::copy("./logs/latest.log", filename.str());

    return exitCode;
}
