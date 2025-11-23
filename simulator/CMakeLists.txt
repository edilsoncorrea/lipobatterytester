cmake_minimum_required(VERSION 3.10)
project(LiPoTesterSimulator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(lipo_simulator main.cpp)

# Platform-specific settings
if(WIN32)
    # Windows specific flags
    target_compile_options(lipo_simulator PRIVATE /W4)
else()
    # Linux/Mac flags
    target_compile_options(lipo_simulator PRIVATE -Wall -Wextra -pedantic)
endif()

# Enable threading support
find_package(Threads REQUIRED)
target_link_libraries(lipo_simulator Threads::Threads)
