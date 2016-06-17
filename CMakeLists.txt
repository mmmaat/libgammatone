# Copyright (C) 2015, 2016 Mathieu Bernard <mathieu_bernard@laposte.net>
#
# This file is part of libgammatone
#
# libgammatone is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with libgammatone. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.0)

# Define project name and version
project(libgammatone)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)

# Setup general compilation flags
set(CMAKE_CXX_COMPILER c++)
set(CMAKE_BUILD_TYPE Release)
add_definitions(${CMAKE_CXX_FLAGS} -Wall -std=c++11)

# Configure version.hpp in source
configure_file(
  "${PROJECT_SOURCE_DIR}/include/gammatone/version.hpp.in"
  "${PROJECT_BINARY_DIR}/include/gammatone/version.hpp")

# Configure path to share directory in test setup and setup.gp
set(LIBGAMMATONE_SHARE ${PROJECT_BINARY_DIR}/share)
configure_file(
  "${PROJECT_SOURCE_DIR}/test/include/test_setup.hpp.in"
  "${PROJECT_BINARY_DIR}/test/include/test_setup.hpp")
configure_file(
  "${PROJECT_SOURCE_DIR}/share/setup.gp.in"
  "${PROJECT_BINARY_DIR}/share/setup.gp")

# add the binary tree to the search path for include files
# so that we will find the configured version.hpp
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/include)

# copy files in share
add_subdirectory(share)

# build the tests
add_subdirectory(test)

# build documentation and examples
add_subdirectory(doc)

# build python wrapper
add_subdirectory(python)

# install header files from source and build directory
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  DESTINATION include
  PATTERN "*.in" EXCLUDE
  )
