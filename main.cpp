// Check OS
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
#include <filesystem>
#include <string>

// libzip
#include <zip.h>

// Plog
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

// BasedCore
#include "BasedCore/Managers/BEAppManager.h"

static int archiveLogs(const std::string& path);

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
        //appManager->createApp();
        appManager->createApp(BasedEngine::OPENGL);
        //appManager->createApp(BasedEngine::DIRECTX);
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

    filename << date << '-' << time;

    std::filesystem::copy("./logs/latest.log", "./logs/" + filename.str() + ".log");

    if (int error = archiveLogs(filename.str()); error != 0)
    {
        exitCode = error;
        return exitCode;
    }

    std::filesystem::remove("./logs/" + filename.str() + ".log");

    return exitCode;
}

static int archiveLogs(const std::string& path)
{
    int error = 0;

    zip_t* archive = zip_open(("./logs/" + path + ".zip").c_str(), ZIP_CREATE | ZIP_TRUNCATE, &error);

    if (!archive)
    {
        std::cerr << "Failed to open output ZIP archive! Error code: " << error << '\n';
        PLOGE << "Failed to open output ZIP archive! Error code: " << error;
        return error;
    }

    zip_source_t* source = zip_source_file(archive, ("./logs/" + path + ".log").c_str(), 0, ZIP_LENGTH_TO_END);
    if (!source)
    {
        std::cerr << "Failed to create source from file: " << zip_strerror(archive) << '\n';
        PLOGE << "Failed to create source from file: " << zip_strerror(archive);
        zip_close(archive);
        return -1;
    }

    zip_int64_t index = zip_file_add(archive, (path + ".log").c_str(), source, ZIP_FL_ENC_UTF_8);
    if (index < 0)
    {
        std::cerr << "Failed to add file to archive: " << zip_strerror(archive) << '\n';
        PLOGE << "Failed to add file to archive: " << zip_strerror(archive);
        zip_source_free(source);
        zip_close(archive);
        return -1;
    }

    if (zip_close(archive) < 0)
    {
        std::cerr << "Failed to write and close ZIP archive: " << zip_strerror(archive) << '\n';
        PLOGE << "Failed to write and close ZIP archive: " << zip_strerror(archive);
        return -1;
    }

    PLOGI << "Archived " << (path + ".log");

    return error;
}
