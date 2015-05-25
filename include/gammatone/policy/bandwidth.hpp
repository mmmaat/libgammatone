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

#ifndef GAMMATONE_POLICY_BANDWIDTH_HPP
#define GAMMATONE_POLICY_BANDWIDTH_HPP

#include <cstddef>
#include <cmath>

namespace gammatone
{
  namespace policy
  {
    //! Policies for filter bandwidth computation
    /*!
      \namespace gammatone::policy::bandwidth

      This namespace provides 3 sets of parameters for computing the
      bandwidth of a gammatone filter, given its center frequency
      \cite Slaney1993 .

      According to Slaney \cite Slaney1993 , a general form for the
      bandwidth \f$ b \f$ of a cochlear channel as a function of its
      center frequency \f$ f_c \f$ is given as : \f[ b(f_c) =
      ((\frac{f_c}{Q})^n + b_m^n)^{1/n},\f] where \f$ Q \f$ is the
      asymptotic filter quality at large frequencies (refered as earq
      in code and Slaney1993), \f$ b_m \f$ is the minimal bandwidth at
      low frequencies (refered as minbw) and \f$ n \f$ is the
      bandwidth order (refered as order).
    */
    namespace bandwidth
    {
      //! Base class of bandwidth policy classes
      template<class Scalar>
      class base
      {
      public:
        //! Bandwidth correction factor. \see Holdsworth1988 .
        static const Scalar bw_correction;

      protected:
        //! Returns the bandwidth of a filter from explicit parameters
        /*!
          \param center_frequency  The center frequency of the filter (Hz).
          \param earq   The asymptotic filter quality at large frequencies.
          \param minbw  The minimal bandwidth at larges frequencies (Hz).
          \param order  The bandwidth order .
          \return The computed bandwidth (Hz).
        */
        static inline Scalar bandwidth(const Scalar& center_frequency,
                                       const Scalar& earq,
                                       const Scalar& minbw,
                                       const std::size_t& order);
      };

      //! Bandwidth parameters from \cite Glasberg1990 .
      template<class Scalar>
      class glasberg1990 : public base<Scalar>
      {
      public:
        static inline Scalar bandwidth(const Scalar& center_frequency);
        static const Scalar earq;
        static const Scalar minbw;
        static const std::size_t order;
      };

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

      //! Bandwidth parameters from \cite Greenwood1990 .
      template<class Scalar>
      class greenwood1990 : public base<Scalar>
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
Scalar gammatone::policy::bandwidth::base<Scalar>::
bandwidth(const Scalar& center_frequency,
          const Scalar& earq,
          const Scalar& minbw,
          const std::size_t& order)
{
  return bw_correction*std::pow((std::pow(center_frequency/earq, order) + std::pow(minbw, order)),
                                1.0/static_cast<Scalar>(order));
}

template<class Scalar> const Scalar gammatone::policy::bandwidth::base<Scalar>::
bw_correction = 1.0190;



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



template<class Scalar>
Scalar gammatone::policy::bandwidth::greenwood1990<Scalar>::bandwidth(const Scalar& center_frequency)
{
  return gammatone::policy::bandwidth::base<Scalar>::bandwidth(center_frequency,earq,minbw,order);
}

template<class Scalar>
const Scalar gammatone::policy::bandwidth::greenwood1990<Scalar>::
earq = 7.23824;

template<class Scalar>
const Scalar gammatone::policy::bandwidth::greenwood1990<Scalar>::
minbw = 22.8509;

template<class Scalar>
const std::size_t gammatone::policy::bandwidth::greenwood1990<Scalar>::
order = 1;

#endif // GAMMATONE_POLICY_BANDWIDTH_HPP
