#include <chrono>  // chrono::system_clock
#include <ctime>   // gmtime/localtime
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

    // TODO: replace by meta programming to deduce longest string
    const char* const verbosityPadding[] = 
    {
        "   ",
        "  ",
        "",
        "   ",
        "  ",
    };

    static_assert(sizeof(verbosityPadding) / sizeof(verbosityPadding[0]) == sizeof(verbosityNames) / sizeof(verbosityNames[0]), "Arrays' size must match.");
}

// Static Logger attributes
std::ofstream Logger::outputFile_;
Verbosity Logger::verbosity_ = Verbosity::DEBUG;
Verbosity Logger::streamVerbosity_ = Logger::verbosity_;
bool Logger::nextLine_ = true;

Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

void Logger::SetVerbosity(const Verbosity verbosity)
{
    verbosity_ = verbosity;
}

const Verbosity Logger::GetVerbosity()
{
    return verbosity_;
}

void Logger::SetStreamVerbosity(const Verbosity verbosity)
{
    streamVerbosity_ = verbosity;
}

const Verbosity Logger::GetStreamVerbosity()
{
    return streamVerbosity_;
}

Logger::Logger()
{
    std::stringstream stream;
    stream << GetCurrentWorkingDirectory() << GetOsSeparator();
    static const char* const dateTimeFormat = "%Y%m%d_%H%M%S";
    InsertFormattedDateTime(stream, dateTimeFormat);
    stream << "_Citadel.log";
    outputFile_.open(stream.str());
}

Logger::~Logger()
{
    outputFile_.close();
}

std::string Logger::GetHeader(const Verbosity verbosity)
{
    std::stringstream stream;
    static const char* const dateTimeFormat = "%Y%m%d_%H:%M:%S";
    InsertFormattedDateTime(stream, dateTimeFormat);
    stream << verbosityPadding[verbosity] << " [" << verbosityNames[verbosity] << "] ";
    return stream.str();
}

void Logger::InsertFormattedDateTime(std::stringstream& stream, const char* const dateTimeFormat)
{
    // TODO: use std::put_time in GNU/Linux version when it's available in GCC/Clang
#ifdef _WIN32
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    stream << std::put_time(std::gmtime(&in_time_t), dateTimeFormat);
#else
    time_t rawtime;
    time(&rawtime);
    struct tm* timeinfo = gmtime(&rawtime);
    char buffer[80];
    strftime(buffer, sizeof(buffer) - 1, dateTimeFormat, timeinfo);
    stream << buffer;
#endif
}