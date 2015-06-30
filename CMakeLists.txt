cmake_minimum_required(VERSION 3.2)

project(libgammatone)

# Setup general compilation flags
set(CMAKE_CXX_COMPILER c++)
set(CMAKE_BUILD_TYPE Release)
add_definitions(${CMAKE_CXX_FLAGS} -Wall -std=c++11)

# Manage the version number of the project
set(libgammatone_VERSION_MAJOR 0)
set(libgammatone_VERSION_MINOR 2)

# Configure version in source
configure_file(
  "${PROJECT_SOURCE_DIR}/include/gammatone/version.hpp.in"
  "${PROJECT_BINARY_DIR}/include/gammatone/version.hpp"
  )

# add the binary tree to the search path for include files
# so that we will find version.hpp
include_directories("${PROJECT_BINARY_DIR}/include")
include_directories(include)

# build the tests
add_subdirectory(test)

# build documentation
add_subdirectory(doc EXCLUDE_FROM_ALL)

# install header files
file(GLOB_RECURSE GAMMATONE_INC include/*)
install(FILES ${GAMMATONE_INC} DESTINATION include/gammatone)
