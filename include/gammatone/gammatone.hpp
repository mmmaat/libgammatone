/*
  Copyright (C) 2015 Mathieu Bernard <mathieu_bernard@laposte.net>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GAMMATONE_HPP
#define GAMMATONE_HPP


// All libgammatone public headers.
// Include this if you are too lazy to include each file individually.

#include <gammatone/filter.hpp>
#include <gammatone/filterbank.hpp>
#include <gammatone/impulse_response.hpp>

#include <gammatone/core/cooke1993.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/convolution.hpp>
//#include <gammatone/core/holdsworth.hpp>

#include <gammatone/policy/bandwidth.hpp>
#include <gammatone/policy/order.hpp>
#include <gammatone/policy/channels.hpp>
#include <gammatone/policy/clipping.hpp>
#include <gammatone/policy/postprocessing.hpp>


// Above are some general comments on libgammatone for documentation
// generation with Doxygen.

/*!
  \mainpage libgammatone, a generic C++ implementation of gammatone auditory filters

  The C++ libgammatone library aims to be
  - Easy to use
  - Extensible and generic
  - Fast and efficient
  - Documented and tested

  \tableofcontents


  \section sec_download Download

  Retrieve the code from **git**:

  \code{.sh} git clone https://github.com/mmmaat/libgammatone.git  \endcode


  \section sec_build Build examples and tests

  - The build process used in libgammatone is **cmake**.

  - The library itself depends only on the C++ standard library and **boost**
  (just for a proper definition of \f$\pi\f$ !)

  - To build the tests and examples you need **gtest**, **gnuplot** and **gnuplot-iostream**.

  On a Debian based system, all the dependancies are in official repositories. To install it:
  ~~~{.sh}
  sudo apt-get install cmake gnuplot libgnuplot-iostream-dev libgtest-dev libboost-dev
  cmake .
  make
  make doc
  ~~~


  \section sec_using Using the library

  This section introduces the user to libgammatone.

  \subsection ssec_basic Basic usage

  Here is a minimal example initializing a gammatone filter and processing an input signal:
  \include minimal.cpp

  \subsection ssec_filter A single filter

  \subsection ssec_filterbank A whole filterbank

  \subsection ssec_template Template cores and policies

  ligammatone is endowed with different cores and processing policies,
  allowing the user to customize the gammatone implementation if
  needed.

  The provided cores are:
  - core::cooke1993
  - core::slaney1993
  - core::convolution

  The provided policies are:
  - policy::bandwidth
  - policy::channels
  - policy::order
  - policy::clipping
  - policy::postprocessing

  \section sec_licence Licence

  This program is free software: you can redistribute it and/or modify
  it under the terms of the **GNU General Public License** as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details. You should have received a
  copy of the GNU General Public License along with this program. If
  not, see <http://www.gnu.org/licenses/>.

  Copyright (C) 2015 Mathieu Bernard <mathieu_bernard@laposte.net>
*/

/*!
  \namespace gammatone
  \brief Root namespace of the libgammatone library

  The gammatone namespace is the root namespace of libgammatone. The
  main classes of this namespace are gammatone::filter and
  gammatone::filterbank, wich define respectively single and multi
  channels gammatone filters. In order to be instanciated, a filter
  (or filterbank) must be associated to one of the proposed processing
  cores. A gammatone::core contains a concrete implementation of the
  gammatone filter. Moreover, a set of gammatone::policy classes
  allows the static customization of the filters.
*/

/*!
  \namespace gammatone::core
  \brief Namespace containing gammatone filter implementations
*/

/*!
  \namespace gammatone::policy
  \brief Namespace containing template policies for filter and filterbank customization
*/


#endif // GAMMATONE_HPP
