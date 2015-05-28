cmake_minimum_required(VERSION 3.2)

project(libgammatone VERSION 0.1)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
#set(CMAKE_BUILD_TYPE Release)

# libgammatone headers
include_directories(include)

add_subdirectory(test)
add_subdirectory(doc EXCLUDE_FROM_ALL)

