cmake_minimum_required(VERSION 3.11.3)

# Project name and language
project(OSM_A_star_search CXX)

# Set C++ standard and flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Find SDL2 package
find_package(SDL2 REQUIRED)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

# Manually specify SDL2_image paths
set(SDL2_IMAGE_INCLUDE_DIRS "/usr/include/SDL2")
set(SDL2_IMAGE_LIBRARIES "-lSDL2_image")

# Find all source files
file(GLOB project_SRCS src/*.cpp)

# Add project executable
add_executable(sueca ${project_SRCS})

# Link SDL2 and SDL2_image libraries
target_link_libraries(sueca ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
