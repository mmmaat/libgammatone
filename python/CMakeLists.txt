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

# use setuptools to build the extension module
add_custom_target(_gammatone.so ALL
  COMMAND python setup.py build_ext --inplace
  DEPENDS gammatone/gammatone.cpp
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

# move the built extension module into the correct location (this is a total hack)
add_custom_target(python ALL
  COMMAND mkdir -p ${PROJECT_BINARY_DIR}/python/gammatone/
  COMMAND cp _gammatone.so ${PROJECT_BINARY_DIR}/python/gammatone/
  COMMAND cp gammatone/__init__.py ${PROJECT_BINARY_DIR}/python/gammatone/
  COMMAND cp test_wrapper.py ${PROJECT_BINARY_DIR}/python/
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

# make sure the above commands run in the correct order
add_dependencies(python _gammatone.so)
