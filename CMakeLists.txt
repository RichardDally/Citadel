cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(PROJECT_NAME Citadel)
project(${PROJECT_NAME} CXX)

# Enabling C++11 and pthread for Linux, this is not necessary for 
if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")                                                                                                                                                    
  list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -pthread")
endif()

if("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")                                                                                                                                                    
	# Restrain to debug/release only
	if(CMAKE_CONFIGURATION_TYPES)
		set(CMAKE_CONFIGURATION_TYPES Debug Release)
		set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)
	endif()
 
	# Add .pdb files to Release
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG")
endif()

include_directories(headers)
file(GLOB_RECURSE SOURCE_FILES headers/* src/*)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})