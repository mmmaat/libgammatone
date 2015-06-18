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

#ifndef GAMMATONE_INTERFACE_HPP
#define GAMMATONE_INTERFACE_HPP

#include <memory>
#include <algorithm>
#include <type_traits>

namespace gammatone
{
  //! Generic interface for both gammatone filters and filterbanks
  /*!
    \class interface gammatone/interface.hpp

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

    //! The type of scalar values
    using scalar_type = Scalar;

    //! The type of the output values
    using output_type = Output;

    //! Accessor to the sample frequency
    /*!
      Accessor to the processing sample frequency (Hz).
      \return The sample frequency.
    */
    virtual scalar_type sample_frequency() const = 0;

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


    inline output_type compute(const scalar_type& input);

    //! Compute preallocated output from a range of scalar inputs.
    /*!
      Sequentially computes a range of input values and stores the
      result in an output range.

      \tparam InputIterator   Iterator on the input range.
      \tparam OutputIterator  Iterator on the output range.

      \param first  Iterator to the initial position of the input range.
      \param last   Iterator to the final position of the input range.
      \param result Iterator to the initial position of the output
      range. The range must include at least as many elements as
      [first,last).
    */
    template<class InputIterator,
             class OutputIterator>
    void compute(const InputIterator& first,
                 const InputIterator& last,
                 const OutputIterator& result);

    void compute(const std::size_t& size,
                 const scalar_type* input,
                 scalar_type* output);
    
    
  protected:

    //! Compute an output from a scalar input
    /*!

      This method is called by the compute() public methods.

      \param input  The scalar value to be processed.
      \return The computed output value.
    */
    virtual output_type compute_internal(const scalar_type& input) = 0;
  };
}




template<class Scalar, class Output>
Output gammatone::interface<Scalar,Output>::
compute(const Scalar& input)
{
  return compute_internal(input);
}

template<class Scalar, class Output>
template<class InputIterator, class OutputIterator>
void gammatone::interface<Scalar,Output>::
compute(const InputIterator& first,
        const InputIterator& last,
        const OutputIterator& output)
{
  std::transform(first,last,output,[&](const auto& x){return this->compute_internal(x);});
}


template<class Scalar, class Output>
void gammatone::interface<Scalar,Output>::
compute(const std::size_t& size,
        const Scalar* input,
        Scalar* output)
{
  for(std::size_t i=0;i<size;++i)
    output[i] = this->compute_internal(input[i]);
}


#endif // GAMMATONE_INTERFACE_HPP
