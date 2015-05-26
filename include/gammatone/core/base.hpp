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

#ifndef GAMMATONE_CORE_BASE_HPP
#define GAMMATONE_CORE_BASE_HPP

#include <gammatone/policy/gain.hpp>
#include <boost/math/constants/constants.hpp>
#include <cmath>
#include <complex>

namespace gammatone
{
  namespace core
  {
    //! Abstract base class of all cores
    /*!
      \class base

      \tparam Scalar         Type of scalar values
      \tparam GainPolicy     Policy for gain computation, see policy::gain .
    */
    template<class Scalar,
             class GainPolicy>
    class base
    {
    public:
      //! Explicit constructor
      /*!
        Creates a core from explicit parameters.

        \param sample_frequency  The sample frequency (Hz).
        \param center_frequency  The core center frequency (Hz).
        \param bandwidth         The core bandwidth (Hz).
      */
      base(const Scalar& sample_frequency, const Scalar& center_frequency, const Scalar& bandwidth);

      //! Copy constructor
      /*!
        \param other The core to copy.
      */
      base(const base<Scalar, GainPolicy>& other);

      //! Assignment operator
      /*!
        \param other The core to copy.
        \return A reference to a new copied core.
      */
      base<Scalar, GainPolicy>& operator=(const base<Scalar, GainPolicy>& other);

      //! Destructor
      virtual ~base();

      //! Return the core internal gain
      inline Scalar gain() const;

      //! Set the core at its initial state
      virtual inline void reset() = 0;

      //! Compute an output from an input value
      virtual inline Scalar compute(const Scalar& input) = 0;

    protected:

      inline Scalar find_factor(const Scalar& sample_frequency,
                                const Scalar& center_frequency,
                                const Scalar& bandwidth);

      //! 2\pi / f_s
      const Scalar m_tau;

      //! Inverse of the filter gain
      const Scalar m_factor;
    };
  }
}


template<class Scalar, class GainPolicy>
gammatone::core::base<Scalar, GainPolicy>::
base(const Scalar& sample_frequency,
     const Scalar& center_frequency,
     const Scalar& bandwidth)
  : m_tau( 2.0*boost::math::constants::pi<Scalar>() / sample_frequency ),
    m_factor( find_factor(sample_frequency, center_frequency, bandwidth) )
{}


template<class Scalar, class GainPolicy>
gammatone::core::base<Scalar, GainPolicy>::base(const base<Scalar, GainPolicy>& other)
  : m_tau(other.m_tau),
    m_factor(other.m_factor)
{}

template<class Scalar, class GainPolicy>
gammatone::core::base<Scalar, GainPolicy>& gammatone::core::base<Scalar, GainPolicy>::
operator=(const base<Scalar, GainPolicy>& other)
{
  base<Scalar, GainPolicy> tmp(other);
  std::swap(m_tau,other.m_tau);
  std::swap(m_factor,other.m_factor);

  return *this;
}

template<class Scalar, class GainPolicy>
gammatone::core::base<Scalar, GainPolicy>::~base()
{}

template<class Scalar, class GainPolicy>
Scalar gammatone::core::base<Scalar, GainPolicy>::gain() const
{
  return 1.0 / m_factor;
}

template<class Scalar, class GainPolicy>
Scalar gammatone::core::base<Scalar, GainPolicy>::
find_factor(const Scalar& sample_frequency,
            const Scalar& center_frequency,
            const Scalar& bandwidth)
{
  const auto a = this->m_tau*bandwidth;
  const auto b = std::complex<Scalar>(0.0, this->m_tau*center_frequency);
  const auto g = 2.0*std::abs(std::exp(2.0*(a+b)) - (1.0 + std::exp(2.0*b))* std::exp(a) - 1.0);
  return 1.0 / GainPolicy::gain(g, sample_frequency, center_frequency, 4);
}

#endif // GAMMATONE_CORE_BASE_HPP
