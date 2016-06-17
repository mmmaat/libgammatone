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

#ifndef GAMMATONE_POLICY_BANDWIDTH_BASE_HPP
#define GAMMATONE_POLICY_BANDWIDTH_BASE_HPP

#include <gammatone/policy/policy.hpp>
#include <cmath>

namespace gammatone
{
  namespace policy
  {
    namespace bandwidth
    {
      //! Base class of bandwidth policy classes
      template<class Scalar>
      class base : public gammatone::policy::policy
      {
      public:
        //! Bandwidth correction factor. \see Holdsworth1988 .
        /*!
          let order n=4, bw correction factor is approximately 1.0190

          \TODO : reimplement with formula !
          \TODO : make it protected ?
        */
        static inline Scalar bw_correction(const std::size_t& order);

      protected:
        //! Returns the bandwidth of a filter from explicit parameters
        /*!
          \param center_frequency  The center frequency of the filter (Hz).
          \param earq   The asymptotic filter quality at large frequencies.
          \param minbw  The minimal bandwidth at larges frequencies (Hz).
          \param order  The bandwidth order .
          \return The computed bandwidth (Hz).
        */
        static inline Scalar bandwidth(
          const Scalar& center_frequency,
          const Scalar& earq,
          const Scalar& minbw,
          const std::size_t& order);
      };
    }
  }
}


template<class Scalar>
Scalar gammatone::policy::bandwidth::base<Scalar>::
bandwidth(const Scalar& center_frequency,
          const Scalar& earq,
          const Scalar& minbw,
          const std::size_t& order)
{
  // We use std::pow with the specialized implementation
  // pow(float,int). The legacy implementation is powf(float,float)
  return bw_correction(order) *
    std::pow((std::pow(center_frequency/earq, order) + std::pow(minbw, order)), 1.0/static_cast<Scalar>(order));
}

template<class Scalar>
Scalar gammatone::policy::bandwidth::base<Scalar>::
bw_correction(const std::size_t& order)
{
  return 1.0190;
}

#endif // GAMMATONE_POLICY_BANDWIDTH_BASE_HPP
