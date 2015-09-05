#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <iostream>
#include <cstdio>  /* defines FILENAME_MAX */
#include "FileSystem.h"

std::string GetCurrentWorkingDirectory()
{
    char path[FILENAME_MAX];

    if (GetCurrentDir(path, sizeof(path)) == false)
    {
        std::cerr << errno << std::endl;
        return "";
    }

    path[sizeof(path) - 1] = '\0';
    return path;
}

char GetOsSeparator()
{
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}