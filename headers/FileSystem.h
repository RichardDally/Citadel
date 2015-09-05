#pragma once

#include <string>

// TODO C++14: use file system instead of this
// #include <experimental/filesystem>
// => std::current_path.string()
// Boost is also an option...
std::string GetCurrentWorkingDirectory();
char GetOsSeparator();