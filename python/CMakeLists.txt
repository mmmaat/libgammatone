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

# Configure necessary packages, boost::python and python headers
find_package(PythonLibs 2.7 REQUIRED)
find_package(Boost COMPONENTS python REQUIRED)

include_directories(
  ${PYTHON_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR})

link_libraries(
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARIES})

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/python/gammatone)

python_add_module(_gammatone ${CMAKE_CURRENT_SOURCE_DIR}/gammatone.cpp)
file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
  DESTINATION ${LIBRARY_OUTPUT_PATH})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_wrapper.py DESTINATION ${PROJECT_BINARY_DIR}/python)
