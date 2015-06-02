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

#ifndef GAMMATONE_FILTER_INTERFACE_HPP
#define GAMMATONE_FILTER_INTERFACE_HPP

#include <gammatone/detail/interface.hpp>

namespace gammatone
{
  namespace filter
  {
    //! Gammatone filter interface
    /*!
      \class interface

      This interface is the base class of the two implementations
      filter::concrete and filter::holded.

      \tparam Scalar  Type of the scalar values
    */
    template<class Scalar>
    class interface : public ::gammatone::detail::interface<Scalar,Scalar>
    {
      //! Must access to compute_internal()
      friend class detail::internal::scalar<interface<Scalar> >;

    public:
      //! Compute output from input data.
      /*!
        Sequentially computes a range of input values and returns the result.

        \param input The input signal to be processed.
        \return The output processed signal

        \todo document the polymorphism
      */
      template<class InputType>
      inline InputType compute(const InputType& input)
      {
        // Any non arithmetic type is considered as a container
        return std::conditional<std::is_arithmetic<InputType>::value,
                                detail::internal::scalar<interface<Scalar> >,
                                detail::internal::container<interface<Scalar> > >
          ::type::template compute<InputType,InputType>(input,this);
      }


      template<class InputIterator, class OutputIterator>
      void compute(const InputIterator& first,
                   const InputIterator& last,
                   const OutputIterator& output)
      {
        std::transform(first,last,output,[&](const auto& x){return this->compute_internal(x);});
      }

      void compute(const std::size_t& size,
		   const Scalar* input,
		   Scalar* output)
      {
	for(std::size_t i=0;i<size;++i)
	  output[i] = this->compute_internal(input[i]);
      }
      
    };
  }
}

#endif // GAMMATONE_FILTER_INTERFACE_HPP
