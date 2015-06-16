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

#ifndef GAMMATONE_INTERFACE_FILTER_HPP
#define GAMMATONE_INTERFACE_FILTER_HPP

#include <gammatone/interface.hpp>

namespace gammatone
{
  //! Gammatone filter interface
  /*!
    \class interface_filter

    This interface is the base class of gammatone::filter

    \tparam Scalar  Type of the scalar values
  */
  template<class Scalar>
  class interface_filter : public interface<Scalar,Scalar>
  {
  private:

    //! detail::scalar must access to this->compute_internal()
    friend class detail::scalar<interface<Scalar,Scalar> >;

  public:

    //! Compute output from input data.
    /*!
      Sequentially computes a range of input values and returns the result.

      \param input The input signal to be processed.
      \return The output processed signal

      \todo document the polymorphism
    */
    template<class Input>
    inline Input compute(const Input& input);

    template<class InputIterator, class OutputIterator>
    void compute(const InputIterator& first,
                 const InputIterator& last,
                 const OutputIterator& output);

    void compute(const std::size_t& size,
                 const Scalar* input,
                 Scalar* output);
  };
}


template<class Scalar>
template<class Input>
Input gammatone::interface_filter<Scalar>::
compute(const Input& input)
{
  // Any non arithmetic type is considered as a container
  return std::conditional<std::is_arithmetic<Input>::value,
                          detail::scalar<interface_filter<Scalar> >,
                          detail::container<interface_filter<Scalar> > >
    ::type::template compute<Input,Input>(input,this);
}

template<class Scalar>
template<class InputIterator, class OutputIterator>
void gammatone::interface_filter<Scalar>::
compute(const InputIterator& first,
        const InputIterator& last,
        const OutputIterator& output)
{
  std::transform(first,last,output,[&](const auto& x){return this->compute_internal(x);});
}


template<class Scalar>
void gammatone::interface_filter<Scalar>::
compute(const std::size_t& size,
        const Scalar* input,
        Scalar* output)
{
  for(std::size_t i=0;i<size;++i)
    output[i] = this->compute_internal(input[i]);
}

#endif // GAMMATONE_INTERFACE_FILTER_HPP
