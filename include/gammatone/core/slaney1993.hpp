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

#ifndef GAMMATONE_CORE_SLANEY1993_HPP
#define GAMMATONE_CORE_SLANEY1993_HPP

#include <gammatone/core/slaney1993_iir.hpp>
#include <cmath>
#include <array>
#include <complex>

namespace gammatone
{
  namespace core
  {
    //! Gammatone filter implementation from \cite Slaney1993 .
    /*!
      \class slaney1993

      Gammatone filter implementation from libgtfb, after \cite Slaney1993 .
      \see Original code comes from libgtfb in the Debian packages. https://tracker.debian.org/pkg/libgtfb

      \tparam Scalar  Type of scalar values.
      \todo Describe the implementation in doc.
    */
    template<class Scalar>
    class slaney1993
    {
    public:
      //! Explicit constructor
      /*!
        Creates a core from explicit parameters.

        \param sample_frequency  The sample frequency (Hz).
        \param center_frequency  The core center frequency (Hz).
        \param bandwidth         The core bandwidth (Hz).
      */
      slaney1993(const Scalar& sample_frequency, const Scalar& center_frequency, const Scalar& bandwidth);

      //! Copy constructor
      slaney1993(const slaney1993<Scalar>& other);

      //! Assignment operator
      slaney1993<Scalar>& operator=(const slaney1993<Scalar>& other);

      //! Destructor
      ~slaney1993();

      //! Set the core at its initial state
      void reset();

      //! Return the core internal gain
      Scalar gain() const;

      inline Scalar compute(const Scalar& input);

    private:
      static Scalar find_gain(const Scalar& sample_frequency,
                              const Scalar& center_frequency,
                              const Scalar& tpt,
                              const Scalar& tptbw);

      static std::array<slaney1993_iir<Scalar>,4> find_filters(const Scalar& sample_frequency,
                                                         const Scalar& center_frequency,
                                                         const Scalar& tpt,
                                                         const Scalar& tptbw);
      //! 2*pi/sample_frequency()
      const Scalar m_tpt;

      //! m_tpt*bandwidth()
      const Scalar m_tptbw;

      //! Gain of the filter
      const Scalar m_gain;

      std::array<slaney1993_iir<Scalar>,4> m_filter;
      //std::array<Scalar,4> m_filter;
    };
  }
}

template<class Scalar>
gammatone::core::slaney1993<Scalar>::slaney1993(const Scalar& sample_frequency,
                                    const Scalar& center_frequency,
                                    const Scalar& bandwidth)
  : m_tpt( 2.0*boost::math::constants::pi<Scalar>() / sample_frequency ),
    m_tptbw( m_tpt * bandwidth ),
    m_gain( find_gain(sample_frequency, center_frequency, m_tpt, m_tptbw) ),
    m_filter( find_filters(sample_frequency, center_frequency, m_tpt, m_tptbw) )
{}

template<class Scalar>
std::array<gammatone::core::slaney1993_iir<Scalar>,4> gammatone::core::slaney1993<Scalar>::
find_filters(const Scalar& sample_frequency,
             const Scalar& center_frequency,
             const Scalar& tpt,
             const Scalar& tptbw)
{
  // tempory variables
  const Scalar a = tpt * center_frequency;
  const Scalar b = pow(2.0,1.5);
  const Scalar c = sin(a)  / (exp(tptbw)*sample_frequency);
  const Scalar d = cos(a)  / (exp(tptbw)*sample_frequency);

  // filters coefficients
  const Scalar A0 = 1.0 / sample_frequency;
  const std::array<Scalar,4> A1 =
    {-sqrt(3.0 + b)*c - d,
     sqrt( 3.0 + b)*c - d,
     -sqrt(3.0 - b)*c - d,
     sqrt( 3.0 - b)*c - d};
  const Scalar A2 = 0.0;

  const Scalar B0 = 1.0;
  const Scalar B1 = -2.0*cos(a)/exp(tptbw);
  const Scalar B2 = exp(-2.0*tptbw);
  const std::array<Scalar,3> B = {B0,B1,B2};

  // filters initialisation
  typedef gammatone::core::slaney1993_iir<Scalar> iir;
  std::array<Scalar,3> a0 = {A0,A1[0],A2};
  std::array<Scalar,3> a1 = {A0,A1[1],A2};
  std::array<Scalar,3> a2 = {A0,A1[2],A2};
  std::array<Scalar,3> a3 = {A0,A1[3],A2};
  std::array<slaney1993_iir<Scalar>,4> filter = {iir(a0,B),iir(a1,B),iir(a2,B),iir(a3,B)};

  return std::move(filter);
}


template<class Scalar>
gammatone::core::slaney1993<Scalar>::slaney1993(const slaney1993<Scalar>& other)
  : m_tpt( other.m_tpt ),
    m_tptbw( other.m_tptbw ),
    m_gain( other.m_gain ),
    m_filter({other.m_filter[0],other.m_filter[1],other.m_filter[2],other.m_filter[3]})
{}

template<class Scalar>
gammatone::core::slaney1993<Scalar>& gammatone::core::slaney1993<Scalar>::operator=(const slaney1993<Scalar>& other)
{
  gammatone::core::slaney1993<Scalar> tmp(other);

  std::swap( m_tpt, tmp.m_tpt );
  std::swap( m_tptbw, tmp.m_tptbw );
  std::swap( m_gain, tmp.m_gain );
  std::swap( m_filter, tmp.m_filter );

  return *this;
}

template<class Scalar>
gammatone::core::slaney1993<Scalar>::~slaney1993()
{}

template<class Scalar>
Scalar gammatone::core::slaney1993<Scalar>::find_gain(const Scalar& sample_frequency,
                                                const Scalar& center_frequency,
                                                const Scalar& tpt,
                                                const Scalar& tptbw)
{
  const Scalar period = 1.0/sample_frequency;
  const Scalar a = tpt * center_frequency;
  const Scalar b = pow(2.0,1.5);
  const std::complex<Scalar> e(0.0, a);

  //! \todo Factorize this code
  return abs((-2.0*exp(e) * period + 2.0*exp(-tptbw + e) * period * (cos(a) - sqrt(3 - b) * sin(a))) *
             (-2.0*exp(2.0*e)*period + 2.0*exp(-tptbw + e) * period * (cos(a) + sqrt(3 - b) * sin(a))) *
             (-2.0*exp(2.0*e)*period + 2.0*exp(-tptbw + e) * period * (cos(a) - sqrt(3 + b) * sin(a))) *
             (-2.0*exp(2.0*e)*period + 2.0*exp(-tptbw + e) * period * (cos(a) + sqrt(3 + b) * sin(a))) /
             pow(2.0*(exp(2.0*(tptbw+e)) - (1.0 + exp(2.0*e))*exp(tptbw) -1.0), 4));
}

template<class Scalar>
void gammatone::core::slaney1993<Scalar>::reset()
{
  for(auto& f:m_filter) f.reset();
}

template<class Scalar>
Scalar gammatone::core::slaney1993<Scalar>::gain() const
{
  return 1.0 / m_gain;
}


template<class Scalar>
Scalar gammatone::core::slaney1993<Scalar>::compute(const Scalar& input)
{
  return m_filter[3].compute(m_filter[2].compute(m_filter[1].compute(m_filter[0].compute(input,m_gain))));
}

#endif // GAMMATONE_CORE_SLANEY1993_HPP
