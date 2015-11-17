# Copyright (C) 2015 Mathieu Bernard <mathieu_bernard@laposte.net>
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
  "${PROJECT_BINARY_DIR}/include/gammatone/version.hpp"
  )

# add the binary tree to the search path for include files
# so that we will find version.hpp
include_directories("${PROJECT_BINARY_DIR}/include")
include_directories(include)

# build the tests
add_subdirectory(test)

# build documentation
add_subdirectory(doc)

# install header files from source and build directory
install(
  DIRECTORY include/ ${PROJECT_BINARY_DIR}/include/
  DESTINATION include
  PATTERN "*.in" EXCLUDE
  )

# install documentation
install(
  DIRECTORY ${PROJECT_BINARY_DIR}/doc/html/
  DESTINATION share/doc/gammatone
  )
