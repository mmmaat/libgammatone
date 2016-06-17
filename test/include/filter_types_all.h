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

#ifndef GAMMATONE_TEST_FILTER_TYPES_ALL_H
#define GAMMATONE_TEST_FILTER_TYPES_ALL_H

#include <gammatone/filter.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/convolution.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/joint_view.hpp>

// cores
template<class... X> using a1 = gammatone::core::cooke1993<X...>;
template<class... X> using a2 = gammatone::core::slaney1993<X...>;
template<class... X> using a3 = gammatone::core::convolution<X...>;

// bandwidths
template<class X> using b1 = gammatone::policy::bandwidth::glasberg1990<X>;
template<class X> using b2 = gammatone::policy::bandwidth::slaney1988<X>;
template<class X> using b3 = gammatone::policy::bandwidth::greenwood1990<X>;

// clipping
using c1 = gammatone::policy::clipping::on;
using c2 = gammatone::policy::clipping::off;


// enumeration of all possible filter types (core/policy combination)

template<class T>
using t1 = boost::mpl::list
  <
  gammatone::filter<T,a1,b1,c1>,
  gammatone::filter<T,a1,b1,c2>,

  gammatone::filter<T,a1,b2,c1>,
  gammatone::filter<T,a1,b2,c2>,

  gammatone::filter<T,a1,b3,c1>,
  gammatone::filter<T,a1,b3,c2>
  >;

template<class T>
using t2 = boost::mpl::list
  <
  gammatone::filter<T,a2,b1,c1>,
  gammatone::filter<T,a2,b1,c2>,

  gammatone::filter<T,a2,b2,c1>,
  gammatone::filter<T,a2,b2,c2>,

  gammatone::filter<T,a2,b3,c1>,
  gammatone::filter<T,a2,b3,c2>
  >;

template<class T>
using t3 = boost::mpl::list
  <
  gammatone::filter<T,a3,b1,c1>,
  gammatone::filter<T,a3,b1,c2>,

  gammatone::filter<T,a3,b2,c1>,
  gammatone::filter<T,a3,b2,c2>,

  gammatone::filter<T,a3,b3,c1>,
  gammatone::filter<T,a3,b3,c2>
  >;

template<class T>
using filter_types = typename boost::mpl::joint_view<
  typename boost::mpl::joint_view<t1<T>,t2<T> >::type,
  t3<T> >::type;

#endif // GAMMATONE_TEST_FILTER_TYPES_ALL_H
