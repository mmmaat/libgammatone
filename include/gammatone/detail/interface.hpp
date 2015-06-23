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

#ifndef GAMMATONE_DETAIL_INTERFACE_HPP
#define GAMMATONE_DETAIL_INTERFACE_HPP

#include <memory> // for std::move
#include <algorithm>

namespace gammatone
{
  namespace detail
  {
    //! Generic interface for both gammatone filters and filterbanks
    /*!
      \class interface gammatone/detail/interface.hpp

      This abstract class provides a common generic interface for both
      gammatone filters and filterbanks (i.e. single and multi channels
      filters). Provide a definition of the scalar type, common
      accessors, a reset() method and a polymorphic compute() method.

      \tparam Scalar  Type of the scalar values (usually double).

      \tparam Output Type of accessors results. Output is either a
      Scalar for modeling a single channel filter or any container of
      Scalar for modeling a filterbank.
    */
    template<class Scalar,
             class Output>
    class interface
    {
    public:

      //! Type of scalar input values 
      using scalar_type = Scalar;

      //! Type of output values
      using output_type = Output;      

      //! Type of this class
      using type = interface<scalar_type,output_type>;
      
      //! Constructor
      interface(const scalar_type& sample_frequency);

      //! Copy constructor
      interface(const type& other);

      //! Move constructor
      interface(type&& other) noexcept;

      //! Assignment operator
      type& operator=(const type& other);

      //! Move operator
      type& operator=(type&& other);

      //! Destructor
      virtual ~interface();

      //! Accessor to the sample frequency
      /*!
        Accessor to the processing sample frequency (Hz).
        \return The sample frequency.
      */
      scalar_type sample_frequency() const;

      //! Accessor to the center frequency
      /*!
        Accessor to the center frequencies of each channel in the filterbank (in Hz).
        \return The center frequencies of each cochlear channel.
      */
      virtual output_type center_frequency() const = 0;

      //! Accessor to the bandwidth
      /*!
        Accessor to the bandwidth of each channel in the filterbank (in Hz).
        \return The bandwidth of each cochlear channel.
      */
      virtual output_type bandwidth() const = 0;

      //! Accessor to the gain
      /*!
        Accessor to the gain of each channel in the filterbank.
        \return The gain of each cochlear channel.

        \todo return it in dB ?
      */
      virtual output_type gain() const = 0;

      //! Clean internal buffers
      /*!
        Fill internal buffers with zeros and restore the filterbank
        state as a newly created one.
      */
      virtual void reset() = 0;
      
    private:

      //! Processing sample frequency (Hz)
      scalar_type m_sample_frequency;
    };
  }
}

template<class Scalar, class Output>
gammatone::detail::interface<Scalar,Output>::
interface(const scalar_type& sample_frequency)
  : m_sample_frequency(sample_frequency)
{}

template<class Scalar, class Output>
gammatone::detail::interface<Scalar,Output>::
interface(const type& other)
  : m_sample_frequency(other.m_sample_frequency)
{}

template<class Scalar, class Output>
gammatone::detail::interface<Scalar,Output>::
interface(type&& other) noexcept
: m_sample_frequency(std::move(other.m_sample_frequency))
{}

template<class Scalar, class Output>
gammatone::detail::interface<Scalar,Output>&
gammatone::detail::interface<Scalar,Output>::
operator=(const type& other)
{
  type tmp(other);
  std::swap( m_sample_frequency, tmp.m_sample_frequency );
  return *this;
}

template<class Scalar, class Output>
gammatone::detail::interface<Scalar,Output>&
gammatone::detail::interface<Scalar,Output>::
operator=(type&& other)
{
  m_sample_frequency = std::move(other.m_sample_frequency);
  return *this;
}

template<class Scalar, class Output>
gammatone::detail::interface<Scalar,Output>::
~interface()
{}

template<class Scalar, class Output>
typename gammatone::detail::interface<Scalar,Output>::scalar_type
gammatone::detail::interface<Scalar,Output>::
sample_frequency() const
{
  return m_sample_frequency;
}

#endif // GAMMATONE_DETAIL_INTERFACE_HPP
