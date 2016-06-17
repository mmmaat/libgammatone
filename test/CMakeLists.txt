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

add_custom_target(tests)

# test specific headers in test/include
include_directories(include)

# build in test
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)

###################
# unit tests
###################

# configure boost for use with boost::test
find_package(Boost 1.55 REQUIRED COMPONENTS unit_test_framework)
link_directories(${DEPS_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})
add_definitions(${DEPS_CFLAGS_OTHER})
include_directories(${Boost_INCLUDE_DIR})

# locate the sources
file(GLOB_RECURSE UNIT_TESTS unit/*.cpp)

# build unit tests (by make or make unit)
# tests are done on a subset of gammatone types
add_executable(unit ${UNIT_TESTS})
target_link_libraries(unit ${Boost_LIBRARIES})

# build all unit tests (by make unit-all)
# tests are done on all gammatone types (huge to compile !)
add_executable(unit-all EXCLUDE_FROM_ALL ${UNIT_TESTS})
target_link_libraries(unit-all ${Boost_LIBRARIES})
set_property(
  TARGET unit-all
  PROPERTY COMPILE_DEFINITIONS LIBGAMMATONE_TEST_ALL)


###################
# standalone tests
###################

# configure boost for use with gnuplot-iostream
find_package(Boost 1.55 REQUIRED COMPONENTS
  system
  filesystem
  iostreams
  timer)
link_directories(${DEPS_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})
add_definitions(${DEPS_CFLAGS_OTHER})
include_directories(${Boost_INCLUDE_DIR})

include_directories(
  ${PROJECT_SOURCE_DIR}/test/include
  ${PROJECT_BINARY_DIR}/test/include)


# build standalone tests
file( GLOB_RECURSE TESTS_SOURCE src/*.cpp )
foreach( src ${TESTS_SOURCE} )
  string( REPLACE ".cpp" "" bin ${src} )
  string( REGEX REPLACE ".*/" "" bin ${bin} )
  add_executable( ${bin} ${src} )
  target_link_libraries( ${bin} ${Boost_LIBRARIES} )
  add_dependencies(tests ${bin})
endforeach(src)
