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

#ifndef GAMMATONE_CORE_COOKE1993_HPP
#define GAMMATONE_CORE_COOKE1993_HPP

#include <gammatone/core/base.hpp>
#include <gammatone/policy/clipping.hpp>
#include <array>


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

      \tparam Scalar         Type of scalar values
      \tparam GainPolicy     Policy for gain computation, see policy::gain .
      \tparam ClippingPolicy Policy for clipping small values, see policy::clipping .
    */
    template
    <
      class Scalar,
      class GainPolicy = policy::gain::forall_0dB,
      class ClippingPolicy = policy::clipping::off
      >
    class cooke1993 : public base<Scalar,GainPolicy>
    {
    public:
      cooke1993(const Scalar& sample_frequency, const Scalar& center_frequency, const Scalar& bandwidth);
      cooke1993(const cooke1993<Scalar, GainPolicy, ClippingPolicy>& other);
      virtual ~cooke1993();

      cooke1993<Scalar, GainPolicy, ClippingPolicy>& operator=
      (const cooke1993<Scalar, GainPolicy, ClippingPolicy>& other);

      inline void reset();
      inline Scalar compute(const Scalar& input);

    private:

      // Filter coefficients

      //! \f$ c = e^{2i\pi f_c/f_s} \f$
      const std::complex<Scalar> c;
      std::complex<Scalar> u, q;
      std::array<Scalar,5> a;
      std::array<std::complex<Scalar>,5> p;
    };
  }
}


template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::cooke1993<Scalar,GainPolicy,ClippingPolicy>::
cooke1993(const Scalar& sample_frequency,
          const Scalar& center_frequency,
          const Scalar& bandwidth)
  : base<Scalar,GainPolicy>(sample_frequency, center_frequency, bandwidth),
  c( std::complex<Scalar>(cos(base<Scalar,GainPolicy>::m_tau*center_frequency),
                          sin(base<Scalar,GainPolicy>::m_tau*center_frequency)))
{
  const Scalar a0 = exp (-this->m_tau*bandwidth);
  a[0] = 4.0*a0;
  a[1] = -6.0*pow(a0,2);
  a[2] = 4.0*pow(a0,3);
  a[3] = -pow(a0,4);
  a[4] = 4.0*pow(a0,2);

  reset();
}

template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::cooke1993<Scalar,GainPolicy,ClippingPolicy>::
cooke1993(const cooke1993<Scalar,GainPolicy,ClippingPolicy>& other)
  : base<Scalar,GainPolicy>(other),
  c(other.c),
  u(other.u),
  q(other.q),
  a(other.a),
  p(other.p)
{}

template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::cooke1993<Scalar,GainPolicy,ClippingPolicy>&
gammatone::core::cooke1993<Scalar,GainPolicy,ClippingPolicy>::
operator=(const cooke1993<Scalar,GainPolicy,ClippingPolicy>& other)
{
  cooke1993<Scalar,GainPolicy,ClippingPolicy> tmp(other);
  base<Scalar,GainPolicy>::operator=(tmp);
  std::swap(c, other.c);
  std::swap(u, other.u);
  std::swap(q, other.q);
  std::swap(a, other.a);
  std::swap(p, other.p);

  return *this;
}

template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::cooke1993<Scalar,GainPolicy,ClippingPolicy>::
~cooke1993()
{}


template<class Scalar, class GainPolicy, class ClippingPolicy>
void gammatone::core::cooke1993<Scalar,GainPolicy,ClippingPolicy>::
reset()
{
  p.fill(0.0);
  q = std::complex<Scalar>(1,0);
}


template<class Scalar, class GainPolicy, class ClippingPolicy>
Scalar gammatone::core::cooke1993<Scalar,GainPolicy,ClippingPolicy>::
compute(const Scalar& input)
{
  // update p and u
  p[0] = ClippingPolicy::clip(q*input + a[0]*p[1] + a[1]*p[2] + a[2]*p[3] + a[3]*p[4]);
  u = p[0] + a[0]*p[1] + a[4]*p[2];
  p[4] = p[3]; p[3] = p[2]; p[2] = p[1]; p[1] = p[0];

  // compute result
  Scalar output = this->m_factor * ( u.real()*q.real() + u.imag()*q.imag() );

  // update q
  std::complex<Scalar> tmp(c.real()*q.real() + c.imag()*q.imag(),
                           c.real()*q.imag() - c.imag()*q.real() );
  std::swap(q,tmp);

  return std::move(output);
}

#endif // GAMMATONE_CORE_COOKE1993_HPP
