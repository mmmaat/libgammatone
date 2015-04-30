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

#ifndef GAMMATONE_CORE_COOKE1993_HPP
#define GAMMATONE_CORE_COOKE1993_HPP

#include <gammatone/policy/clipping.hpp>
#include <boost/math/constants/constants.hpp> // for pi
#include <utility>
#include <cmath>
#include <array>
#include <complex>

namespace gammatone
{
  namespace core
  {
    //! Gammatone filter implementation as proposed by \cite Cooke1993 \cite Ma2006 .
    /*!
      \class cooke1993

      This gammatone filter implementation is based on \cite Cooke1993
      using the base-band impulse invariant transformation. For each
      sample \f$ t \f$ the signal is first multiplied by a complex
      exponential \f$ e^{-j2\pi ft} \f$ at the desired center frequency,
      then filtered with a base-band gammatone filter, and finally
      shifted back to the centre frequency region by multiplying the
      signal by \f$e^{j2\pi ft}\f$.

      \see More details and original implementation of this code in \cite Ma2006 .

      \tparam Scalar  Type of scalar values
      \tparam ClippingPolicy Policy for clipping small values, see policy::clipping .
    */
    template<class Scalar, class ClippingPolicy = policy::clipping::off>
    class cooke1993
    {
    public:
      //! Explicit constructor
      /*!
        Creates a core from explicit parameters.

        \param sample_frequency  The sample frequency (Hz).
        \param center_frequency  The core center frequency (Hz).
        \param bandwidth         The core bandwidth (Hz).
      */
      cooke1993(const Scalar& sample_frequency, const Scalar& center_frequency, const Scalar& bandwidth);

      //! Copy constructor
      /*!
        \param other The core to copy.
      */
      cooke1993(const cooke1993<Scalar, ClippingPolicy>& other);

      //! Assignment operator
      /*!
        \param other The core to copy.
        \return A reference to a new copied core.
      */
      cooke1993<Scalar, ClippingPolicy>& operator=(const cooke1993<Scalar, ClippingPolicy>& other);

      //! Destructor
      virtual ~cooke1993();

      //! Set the core at its initial state
      inline void reset();

      //! Return the core internal gain
      inline Scalar gain() const;

      //! Compute an output from an input value
      inline Scalar compute(const Scalar& input);

    private:
      //! 2*pi/sample_frequency()
      const Scalar m_tpt;

      //! m_tpt*bandwidth()
      const Scalar m_tptbw;

      //! Gain of the filter
      const Scalar m_gain;

      // Internal cooking
      std::complex<Scalar> c, u, q;
      std::array<Scalar,5> a;
      std::array<std::complex<Scalar>,5> p;
    };
  }
}


template<class Scalar, class ClippingPolicy>
gammatone::core::cooke1993<Scalar,ClippingPolicy>::
cooke1993(const Scalar& sample_frequency,
          const Scalar& center_frequency,
          const Scalar& bandwidth)
  : m_tpt( 2.0*boost::math::constants::pi<Scalar>() / sample_frequency ),
    m_tptbw( m_tpt * bandwidth ),
    m_gain( pow(m_tptbw,4) / 3.0 ),
    c( std::complex<Scalar>(cos(m_tpt*center_frequency), sin(m_tpt*center_frequency)))
{
  const Scalar a0 = exp (-m_tptbw);
  a[0] = 4.0*a0;
  a[1] = -6.0*pow(a0,2);
  a[2] = 4.0*pow(a0,3);
  a[3] = -pow(a0,4);
  a[4] = 4.0*pow(a0,2);

  reset();
}

template<class Scalar, class ClippingPolicy>
gammatone::core::cooke1993<Scalar,ClippingPolicy>::
cooke1993(const cooke1993<Scalar,ClippingPolicy>& other)
  : m_tpt(other.m_tpt),
    m_tptbw(other.m_tptbw),
    m_gain(other.m_gain),
    c(other.c),
    u(other.u),
    q(other.q),
    a(other.a),
    p(other.p)
{}

template<class Scalar, class ClippingPolicy>
gammatone::core::cooke1993<Scalar,ClippingPolicy>& gammatone::core::cooke1993<Scalar,ClippingPolicy>::
operator=(const cooke1993<Scalar,ClippingPolicy>& other)
{
  gammatone::core::cooke1993<Scalar,ClippingPolicy> tmp(other);

  std::swap(m_tpt, other.m_tpt);
  std::swap(m_tptbw, other.m_tptbw);
  std::swap(m_gain, other.m_gain);
  std::swap(c, other.c);
  std::swap(u, other.u);
  std::swap(q, other.q);
  std::swap(a, other.a);
  std::swap(p, other.p);

  return *this;
}

template<class Scalar, class ClippingPolicy>
gammatone::core::cooke1993<Scalar,ClippingPolicy>::
~cooke1993()
{}

template<class Scalar, class ClippingPolicy>
void gammatone::core::cooke1993<Scalar,ClippingPolicy>::
reset()
{
  p.fill(0.0);
  q = std::complex<Scalar>(1,0);
}

template<class Scalar, class ClippingPolicy>
Scalar gammatone::core::cooke1993<Scalar,ClippingPolicy>::
gain() const
{
  return m_gain;
}

template<class Scalar, class ClippingPolicy>
Scalar gammatone::core::cooke1993<Scalar,ClippingPolicy>::
compute(const Scalar& input)
{
  p[0] = ClippingPolicy::clip(q*input + a[0]*p[1] + a[1]*p[2] + a[2]*p[3] + a[3]*p[4]);

  u = p[0] + a[0]*p[1] + a[4]*p[2];
  p[4] = p[3]; p[3] = p[2]; p[2] = p[1]; p[1] = p[0];

  Scalar output = m_gain * ( u.real()*q.real() + u.imag()*q.imag() );
  std::complex<Scalar> tmp(c.real()*q.real() + c.imag()*q.imag(),
                           c.real()*q.imag() - c.imag()*q.real() );
  std::swap(q,tmp);

  return std::move(output);
}

#endif // GAMMATONE_CORE_COOKE1993_HPP
