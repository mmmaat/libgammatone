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

#ifndef GAMMATONE_POLICY_BANDWIDTH_SLANEY1988_HPP
#define GAMMATONE_POLICY_BANDWIDTH_SLANEY1988_HPP

#include <gammatone/policy/bandwidth/base.hpp>

namespace gammatone
{
  namespace policy
  {
    namespace bandwidth
    {
      //! Bandwidth parameters from \cite Slaney1988 .
      template<class Scalar>
      class slaney1988 : public base<Scalar>
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
Scalar gammatone::policy::bandwidth::slaney1988<Scalar>::bandwidth(const Scalar& center_frequency)
{
  return gammatone::policy::bandwidth::base<Scalar>::bandwidth(center_frequency,earq,minbw,order);
}

template<class Scalar>
const Scalar gammatone::policy::bandwidth::slaney1988<Scalar>::
earq = 8;

template<class Scalar>
const Scalar gammatone::policy::bandwidth::slaney1988<Scalar>::
minbw = 125;

template<class Scalar>
const std::size_t gammatone::policy::bandwidth::slaney1988<Scalar>::
order = 2;

#endif // GAMMATONE_POLICY_BANDWIDTH_SLANEY1988_HPP
