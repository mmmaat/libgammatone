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

#ifndef GAMMATONE_INTERFACE_HPP
#define GAMMATONE_INTERFACE_HPP

#include <memory>
#include <algorithm>

namespace gammatone
{
  //! Generic interface for both gammatone filters and filterbanks
  /*!
    \class interface

    This abstract class provides a common generic interface for both
    gammatone filters and filterbanks(i.e. single and multi channels
    filters). Provide a definition of the scalar type, common
    accessors and a reset() method.

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
    typedef Scalar scalar_type;

    //! The type of the output values
    typedef Output output_type;

    // //! virtual destructor
    // virtual ~base() = 0;

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

    //! Compute a output state from a scalar input.
    /*!
      \param input  The scalar value to be processed by the filter.
      \return The computed output value
    */
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
    template<class InputIterator, class OutputIterator>
    void compute(const InputIterator& first,
                 const InputIterator& last,
                 const OutputIterator& result);

    //! Compute output from a range of scalar inputs.
    /*!
      Sequentially computes a range of input values and returns the result.

      \tparam Container  Type of the result container.

      \param input The input signal to be processed
      \return The output processed signal
    */
    template<class Container>
    Container compute(const Container& input);

  protected:
    virtual output_type compute_internal(const scalar_type& input) = 0;
  };
}

template<class Scalar,class Output>
Output gammatone::interface<Scalar,Output>::compute(const Scalar& input)
{
  return compute_internal(input);
}

template<class Scalar,class Output>
template<class InputIterator, class OutputIterator>
void gammatone::interface<Scalar,Output>::
compute(const InputIterator& first, const InputIterator& last, const OutputIterator& result)
{
  std::transform(first,last,result,[&](const auto& x){return this->compute_internal(x);});
}

template<class Scalar, class Output>
template<class Container>
Container gammatone::interface<Scalar,Output>::
compute(const Container& input)
{
  Container out(input.size());
  this->compute(input.begin(),input.end(),out.begin());
  return std::move(out);
}

#endif // GAMMATONE_INTERFACE_HPP
