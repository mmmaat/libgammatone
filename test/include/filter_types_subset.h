/*
  Copyright (C) 2015, 2016 Mathieu Bernard <mathieu_bernard@laposte.net>

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

#ifndef GAMMATONE_TEST_FILTER_TYPES_SUBSET_H
#define GAMMATONE_TEST_FILTER_TYPES_SUBSET_H

#include <gammatone/filter.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/convolution.hpp>
#include <boost/mpl/list.hpp>

// cores
template<class... X> using a1 = gammatone::core::cooke1993<X...>;
template<class... X> using a2 = gammatone::core::slaney1993<X...>;
template<class... X> using a3 = gammatone::core::convolution<X...>;

//using T = double;

template<class T> using filter_types = boost::mpl::list
  <
  gammatone::filter<T,a1>,
  gammatone::filter<T,a2>,
  gammatone::filter<T,a3>
  >;

#endif // GAMMATONE_TEST_FILTER_TYPES_SUBSET_H
