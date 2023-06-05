cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME Citadel)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${PROJECT_NAME} CXX)

include_directories(third_party)

# Enabling C++11 and pthread for Linux, this is not necessary for
#if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
#  list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -pthread")
#endif()

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
file(GLOB_RECURSE SOURCE_FILES headers/*.h src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} rdkafka)

# Package Citadel with CPack
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION bin
        )
include (InstallRequiredSystemLibraries)

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
include(CPack)
