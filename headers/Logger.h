#pragma once

#include <iostream>
#include <string>
#include <fstream> // ofstream

enum Verbosity
{
    NONE,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
};

class Logger
{
public:
    static Logger& GetInstance();
    static void Log(const Verbosity verbosity, const std::string& message);
    static void SetVerbosity(const Verbosity verbosity);
    static const Verbosity GetVerbosity();

    using endl_type = std::ostream&(std::ostream&);

    Logger& operator<<(endl_type endl)
    {
        nextLine_ = true;
        outputFile_ << endl;
        return *this;
    }

    Logger& operator<<(const Verbosity verbosity)
    {
        SetVerbosity(verbosity);
        return *this;
    }

    template<typename T>
    Logger& operator<< (const T& data)
    {
        if (nextLine_)
        {
            outputFile_ << GetHeader(GetVerbosity()) << data;
            nextLine_ = false;
        }
        else
        {
            outputFile_ << data;
        }
        return *this;
    }

private:
    Logger();
    ~Logger();

    static std::string GetHeader(const Verbosity verbosity);

    static std::ofstream outputFile_;
    static Verbosity verbosity_;
    static bool nextLine_;
};