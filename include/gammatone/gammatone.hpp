/*
  Copyright (C) 2015 Mathieu Bernard <mathieu_bernard@laposte.net>

  This file is part of libgammatone

  libgammatone is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with libgammatone. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GAMMATONE_HPP
#define GAMMATONE_HPP


// All libgammatone public headers.
// Include this if you are too lazy to include each file individually.

#include <gammatone/filter.hpp>
#include <gammatone/filterbank.hpp>
#include <gammatone/detail/impulse_response.hpp>

#include <gammatone/core/cooke1993.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/convolution.hpp>

#include <gammatone/policy/bandwidth.hpp>
#include <gammatone/policy/channels.hpp>
#include <gammatone/policy/clipping.hpp>


// Above are some general comments on libgammatone for Doxygen doc


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
  
  \brief Gammatone core processing implementations
*/

/*!  
  \namespace gammatone::policy 
  
  \brief Policy classes for filter and filterbank
  customization
*/

/*!
  \namespace gammatone::detail

  \brief Utility classes and internal details of libgammatone

  Users does not have to interact with that code.
 */

#endif // GAMMATONE_HPP
