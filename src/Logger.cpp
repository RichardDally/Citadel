#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time

#include "Logger.h"
#include "FileSystem.h"

namespace
{
    const char* const verbosityNames[] = 
    {
        "NONE",
        "ERROR",
        "WARNING",
        "INFO",
        "DEBUG",
    };
}

// Static Logger attributes
std::ofstream Logger::outputFile_;
Verbosity Logger::verbosity_ = Verbosity::INFO;
bool Logger::nextLine_ = true;

Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

void Logger::Log(const Verbosity verbosity, const std::string& message)
{
    if (verbosity > GetVerbosity())
    {
        return;
    }
    outputFile_ << GetHeader(verbosity) << message;
}

void Logger::SetVerbosity(const Verbosity verbosity)
{
    verbosity_ = verbosity;
}

const Verbosity Logger::GetVerbosity()
{
    return verbosity_;
}

Logger::Logger()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    // Filename
    std::stringstream stream;
    stream << GetCurrentWorkingDirectory() << GetOsSeparator() << std::put_time(std::gmtime(&in_time_t), "%Y%m%d_%H%M%S") << "_Citadel.log";

    outputFile_.open(stream.str());
}

Logger::~Logger()
{
    outputFile_.close();
}

std::string Logger::GetHeader(const Verbosity verbosity)
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    // Header
    std::stringstream stream;
    stream << std::put_time(std::gmtime(&in_time_t), "%Y%m%d_%H:%M:%S") << '[' << verbosityNames[verbosity] << "] ";
    return stream.str();
}