#pragma once

#include <iostream>
#include <string>
#include <fstream> // ofstream

enum Verbosity
{
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
};

class Logger
{
public:
    static Logger& GetInstance();

    static void SetVerbosity(const Verbosity verbosity);
    static const Verbosity GetVerbosity();

    static void SetStreamVerbosity(const Verbosity verbosity);
    static const Verbosity GetStreamVerbosity();

    using endl_type = std::ostream&(std::ostream&);

    Logger& operator<<(endl_type endl)
    {
        if (nextLine_ == false)
        {
            nextLine_ = true;
            outputFile_ << endl;
        }
        return *this;
    }

    Logger& operator<<(const Verbosity verbosity)
    {
        SetStreamVerbosity(verbosity);
        return *this;
    }

    template<typename T>
    Logger& operator<<(const T& data)
    {
        if (GetStreamVerbosity() <= GetVerbosity())
        {
            if (nextLine_)
            {
                outputFile_ << GetHeader(GetStreamVerbosity()) << data;
                nextLine_ = false;
            }
            else
            {
                outputFile_ << data;
            }
        }
        return *this;
    }

private:
    Logger();
    ~Logger();

    static std::string GetHeader(const Verbosity verbosity);
    static void InsertFormattedDateTime(std::stringstream& stream, const char* const dateTimeFormat);

    static std::ofstream outputFile_;
    static Verbosity verbosity_;
    static Verbosity streamVerbosity_;
    static bool nextLine_;
};