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

#ifndef GAMMATONE_POLICY_BANDWIDTH_GLASBERG1990_HPP
#define GAMMATONE_POLICY_BANDWIDTH_GLASBERG1990_HPP

#include <gammatone/policy/bandwidth/base.hpp>

namespace gammatone
{
  namespace policy
  {
    namespace bandwidth
    {
      //! Bandwidth parameters from \cite Glasberg1990
      template<class Scalar>
      class glasberg1990 : public base<Scalar>
      {
      public:
        static inline Scalar bandwidth(const Scalar& center_frequency);

        static const Scalar earq;
        static const Scalar minbw;
        static const std::size_t order;
      };
    }
  }
}


template<class Scalar>
Scalar gammatone::policy::bandwidth::glasberg1990<Scalar>::bandwidth(const Scalar& center_frequency)
{
  return gammatone::policy::bandwidth::base<Scalar>::bandwidth(center_frequency,earq,minbw,order);
}

template<class Scalar>
const Scalar gammatone::policy::bandwidth::glasberg1990<Scalar>::
earq = 9.26449;

template<class Scalar>
const Scalar gammatone::policy::bandwidth::glasberg1990<Scalar>::
minbw = 24.7;

template<class Scalar>
const std::size_t gammatone::policy::bandwidth::glasberg1990<Scalar>::
order = 1;



#endif // GAMMATONE_POLICY_BANDWIDTH_GLASBERG1990_HPP
