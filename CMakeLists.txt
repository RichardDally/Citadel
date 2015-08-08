cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(Citadel CXX)

# Enabling C++11 and pthread for Linux, this is not necessary for "Windows"
if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")                                                                                                                                                    
  list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -pthread")
endif()

# Include FinancialMarketServer headers
include_directories(headers)

# FinancialMarketServer files
file(GLOB_RECURSE CITADEL_FILES headers/* src/*)

add_executable(Citadel ${CITADEL_FILES})