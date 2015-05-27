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

#ifndef GAMMATONE_CORE_SLANEY1993_HPP
#define GAMMATONE_CORE_SLANEY1993_HPP

#include <gammatone/core/base.hpp>
#include <gammatone/core/slaney1993_iir.hpp>
#include <gammatone/policy/clipping.hpp>
#include <array>

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
      \tparam GainPolicy     Policy for gain computation, see policy::gain .

      \todo Describe the implementation in doc.
    */
    template<class Scalar,
             class GainPolicy = policy::gain::forall_0dB,
	     class ClippingPolicy = policy::clipping::off>
    class slaney1993 : public base<Scalar,GainPolicy>
    {
    public:
      slaney1993(const Scalar& sample_frequency, const Scalar& center_frequency, const Scalar& bandwidth);
      slaney1993(const slaney1993<Scalar,GainPolicy,ClippingPolicy>& other);
      virtual ~slaney1993();

      slaney1993<Scalar,GainPolicy,ClippingPolicy>&
      operator=(const slaney1993<Scalar,GainPolicy,ClippingPolicy>& other);

      inline void reset();
      inline Scalar compute(const Scalar& input);

    private:

      inline std::array<slaney1993_iir<Scalar>,4> find_filters(const Scalar& sample_frequency,
                                                               const Scalar& center_frequency,
							       const Scalar& bandwidth);

      std::array<slaney1993_iir<Scalar>,4> m_filter;
    };
  }
}

template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::slaney1993<Scalar,GainPolicy,ClippingPolicy>::
slaney1993(const Scalar& sample_frequency,
           const Scalar& center_frequency,
           const Scalar& bandwidth)
  : base<Scalar,GainPolicy>(sample_frequency, center_frequency, bandwidth),
  m_filter(find_filters(sample_frequency, center_frequency, bandwidth))
{
  reset();
}

template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::slaney1993<Scalar,GainPolicy,ClippingPolicy>::
slaney1993(const slaney1993<Scalar,GainPolicy,ClippingPolicy>& other)
  : base<Scalar,GainPolicy>(other),
    m_filter({other.m_filter[0],other.m_filter[1],other.m_filter[2],other.m_filter[3]})
{}

template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::slaney1993<Scalar,GainPolicy,ClippingPolicy>&
gammatone::core::slaney1993<Scalar,GainPolicy,ClippingPolicy>::
operator=(const slaney1993<Scalar,GainPolicy,ClippingPolicy>& other)
{
  gammatone::core::slaney1993<Scalar,GainPolicy,ClippingPolicy> tmp(other);
  
  base<Scalar,GainPolicy>::operator=(tmp);
  std::swap(m_filter, tmp.m_filter);

  return *this;
}

template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::slaney1993<Scalar,GainPolicy,ClippingPolicy>::~slaney1993()
{}

// template<class Scalar, class GainPolicy, class ClippingPolicy>
// Scalar gammatone::core::slaney1993<Scalar,GainPolicy>::
// find_gain(const Scalar& sample_frequency,
//           const Scalar& center_frequency,
//           const Scalar& tpt,
//           const Scalar& tptbw)
// {
//   //  const Scalar period = 1.0/sample_frequency;
//   const Scalar a = tpt * center_frequency;
//   //  const Scalar b = pow(2.0,1.5);
//   const std::complex<Scalar> e(0.0, a);

//   //! \todo Factorize this code
//   return /*abs((-2.0*exp(e) * period + 2.0*exp(-tptbw + e) * period * (cos(a) - sqrt(3 - b) * sin(a))) *
//            (-2.0*exp(2.0*e)*period + 2.0*exp(-tptbw + e) * period * (cos(a) + sqrt(3 - b) * sin(a))) *
//            (-2.0*exp(2.0*e)*period + 2.0*exp(-tptbw + e) * period * (cos(a) - sqrt(3 + b) * sin(a))) *
//            (-2.0*exp(2.0*e)*period + 2.0*exp(-tptbw + e) * period * (cos(a) + sqrt(3 + b) * sin(a)))) /*/
//     1.0/GainPolicy::gain(abs(2.0*(exp(2.0*(tptbw+e)) - (1.0 + exp(2.0*e))*exp(tptbw) -1.0)),
//                          sample_frequency, center_frequency,4);
// }

template<class Scalar, class GainPolicy, class ClippingPolicy>
void gammatone::core::slaney1993<Scalar,GainPolicy,ClippingPolicy>::reset()
{
  for(auto& f:m_filter) f.reset();
}

template<class Scalar, class GainPolicy, class ClippingPolicy>
Scalar gammatone::core::slaney1993<Scalar,GainPolicy,ClippingPolicy>::compute(const Scalar& input)
{
  return m_filter[3].compute(m_filter[2].compute(m_filter[1].compute(m_filter[0].compute(input,this->m_factor))));
}


template<class Scalar, class GainPolicy, class ClippingPolicy>
std::array<gammatone::core::slaney1993_iir<Scalar>,4>
gammatone::core::slaney1993<Scalar,GainPolicy,ClippingPolicy>::
find_filters(const Scalar& sample_frequency,
             const Scalar& center_frequency,
	     const Scalar& bandwidth)
{
  // tempory variables
  const Scalar tptbw = this->m_tau * bandwidth;
  const Scalar a = this->m_tau * center_frequency;
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
  typedef slaney1993_iir<Scalar> iir;
  const std::array<Scalar,3> a0 = {A0,A1[0],A2};
  const std::array<Scalar,3> a1 = {A0,A1[1],A2};
  const std::array<Scalar,3> a2 = {A0,A1[2],A2};
  const std::array<Scalar,3> a3 = {A0,A1[3],A2};
  std::array<iir,4> filter = {iir(a0,B),iir(a1,B),iir(a2,B),iir(a3,B)};

  return std::move(filter);
}

#endif // GAMMATONE_CORE_SLANEY1993_HPP
