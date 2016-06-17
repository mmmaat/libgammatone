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

#ifndef GAMMATONE_CORE_CONVOLUTION_HPP
#define GAMMATONE_CORE_CONVOLUTION_HPP

#include <gammatone/filter.hpp>
#include <gammatone/detail/impulse_response.hpp>
#include <gammatone/detail/utils.hpp>
#include <algorithm>
#include <numeric>
#include <memory>
#include <vector>
#include <deque>

namespace gammatone
{
  namespace core
  {
    //! Gammatone filter implementation by convolution from theorical impulse response
    /*!
      \class convolution

      \tparam Scalar  Type of scalar values
    */
    template<class Scalar,
	     class GainPolicy = policy::gain::forall_0dB,
	     class ClippingPolicy = policy::clipping::off>
    class convolution : public base<Scalar,GainPolicy>
    {
      using this_type = convolution<Scalar,GainPolicy,ClippingPolicy>;

    public:

      convolution(const Scalar& sample_frequency,
		  const Scalar& center_frequency,
		  const Scalar& bandwidth);

      convolution(const this_type& other);
      convolution(this_type&& other) noexcept;

      this_type& operator=(const this_type& other);
      this_type& operator=(this_type&& other);

      virtual ~convolution();

      inline void reset();

        inline void compute(const Scalar& input, Scalar& output);

    private:
      //! Find impulse response cutoff at a given dB
      void cutoff(const Scalar db = -30);

      //! The underlying impulse response
      std::vector<Scalar> m_ir;

      //! History of the input signal
      std::deque<Scalar> m_input;

      //! Convolution core doesn't use find_factor for gain computation
      inline Scalar find_factor(const Scalar&, const Scalar&, const Scalar&){}
    };
  }
}



template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::convolution<Scalar, GainPolicy, ClippingPolicy>::
convolution(const Scalar& sample_frequency,
            const Scalar& center_frequency,
            const Scalar& bandwidth)
  : base<Scalar, GainPolicy>(sample_frequency,center_frequency,bandwidth),
  //! \todo remove -60db magic number !
  m_ir(gammatone::detail::impulse_response::theorical_attenuate(center_frequency,bandwidth,sample_frequency,-60.0))
{
  reset();
}


template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::convolution<Scalar, GainPolicy, ClippingPolicy>::
convolution(const convolution<Scalar, GainPolicy,ClippingPolicy>& other)
  : base<Scalar, GainPolicy>(other),
  m_ir( other.m_ir ),
  m_input( other.m_input )
{}

template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::convolution<Scalar, GainPolicy, ClippingPolicy>::
convolution(convolution<Scalar, GainPolicy,ClippingPolicy>&& other) noexcept
  : base<Scalar, GainPolicy>(std::move(other)),
  m_ir( std::move(other.m_ir) ),
  m_input( std::move(other.m_input) )
{}


template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::convolution<Scalar, GainPolicy, ClippingPolicy>&
gammatone::core::convolution<Scalar, GainPolicy, ClippingPolicy>::
operator=(const convolution<Scalar, GainPolicy,ClippingPolicy>& other)
{
  convolution<Scalar, GainPolicy,ClippingPolicy> tmp(other);
  base<Scalar, GainPolicy>::operator=(tmp);
  std::swap(m_ir, tmp.m_ir);
  std::swap(m_input, tmp.m_input);

  return *this;
}

template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::convolution<Scalar, GainPolicy, ClippingPolicy>&
gammatone::core::convolution<Scalar, GainPolicy, ClippingPolicy>::
operator=(convolution<Scalar, GainPolicy,ClippingPolicy>&& other)
{
  base<Scalar, GainPolicy>::operator=(other);
  m_ir = std::move(other.m_ir);
  m_input = std::move(other.m_input);

  return *this;
}

template<class Scalar, class GainPolicy, class ClippingPolicy>
gammatone::core::convolution<Scalar, GainPolicy, ClippingPolicy>::
~convolution()
{}


template<class Scalar, class GainPolicy, class ClippingPolicy>
void
gammatone::core::convolution<Scalar, GainPolicy, ClippingPolicy>::
reset()
{
  m_input.assign(m_ir.size(), 0.0);
}


template<class Scalar, class GainPolicy, class ClippingPolicy>
void
gammatone::core::convolution<Scalar, GainPolicy, ClippingPolicy>::
compute(const Scalar& input, Scalar& output)
{
  m_input.pop_front();
  m_input.push_back(input);
  output = std::inner_product(m_input.begin(), m_input.end(), m_ir.rbegin(), 0.0);
}

#endif // GAMMATONE_CORE_CONVOLUTION_HPP
