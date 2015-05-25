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

#include <iostream>

namespace gammatone
{
  namespace core
  {
    //! Gammatone filter implementation by convolution from theorical impulse response
    /*!
      \class convolution

      \tparam Scalar  Type of scalar values
    */
    template<class Scalar>
    class convolution
    {
    public:
      //! Explicit constructor
      /*!
        Creates a core from explicit parameters.

        \param sample_frequency  The sample frequency (Hz).
        \param center_frequency  The core center frequency (Hz).
        \param bandwidth         The core bandwidth (Hz).
      */
      convolution(const Scalar& sample_frequency, const Scalar& center_frequency, const Scalar& bandwidth);

      //! Copy constructor
      /*!
        \param other The core to copy.
      */
      convolution(const convolution<Scalar>& other);

      //! Assignment operator
      /*!
        \param other The core to copy.
        \return A reference to a new copied core.
      */
      convolution<Scalar>& operator=(const convolution<Scalar>& other);

      //! Destructor
      virtual ~convolution();

      //! Set the core at its initial state
      inline void reset();

      //! Return the core internal gain
      inline Scalar gain() const;

      //! Compute an output from an input value
      inline Scalar compute(const Scalar& input);

    protected:
      //! Find impulse response cutoff at a given dB
      void cutoff(const Scalar db = -30);

      //! The underlying impulse response
      std::vector<Scalar> m_ir;

      //! History of the input signal
      std::deque<Scalar> m_input;

      //! The filter gain
      Scalar m_gain;
    };
  }
}



template<class Scalar>
gammatone::core::convolution<Scalar>::
convolution(const Scalar& sample_frequency,
            const Scalar& center_frequency,
            const Scalar& bandwidth)
{
  // compute 1s theorical impulse response
  //! \todo remove -60db magic number !
  m_ir = gammatone::impulse_response::theorical_attenuate(center_frequency,bandwidth,sample_frequency,-60.0);
  
  // find gain as absmax
  m_gain = detail::absmax(m_ir.begin(), m_ir.end());

  // allocate input buffer
  reset();
}


template<class Scalar>
gammatone::core::convolution<Scalar>::convolution(const convolution<Scalar>& other)
  : m_ir( other.m_ir ),
    m_input( other.m_input ),
    m_gain( other.m_gain )
{}


template<class Scalar>
gammatone::core::convolution<Scalar>& gammatone::core::convolution<Scalar>::
operator=(const convolution<Scalar>& other)
{
  convolution<Scalar> tmp(other);

  std::swap(m_ir, tmp.m_ir);
  std::swap(m_input, tmp.m_input);
  std::swap(m_gain, tmp.m_gain);

  return *this;
}


template<class Scalar>
gammatone::core::convolution<Scalar>::~convolution()
{}


template<class Scalar>
void gammatone::core::convolution<Scalar>::reset()
{
  m_input.assign(m_ir.size(), 0.0);
}


template<class Scalar>
Scalar gammatone::core::convolution<Scalar>::gain() const
{
  return m_gain;
}


template<class Scalar>
Scalar gammatone::core::convolution<Scalar>::compute(const Scalar& input)
{
  m_input.pop_front();
  m_input.push_back(input);

  return std::inner_product(m_input.begin(), m_input.end(), m_ir.rbegin(), 0.0);
}

#endif // GAMMATONE_CORE_CONVOLUTION_HPP
