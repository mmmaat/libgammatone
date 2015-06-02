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

#ifndef GAMMATONE_FILTERBANK_INTERFACE_HPP
#define GAMMATONE_FILTERBANK_INTERFACE_HPP

#include <gammatone/detail/interface.hpp>
#include <vector>


namespace gammatone
{
  namespace filterbank
  {
    template<class Scalar,
             class Output = std::vector<Scalar> >
    class interface : public ::gammatone::detail::interface<Scalar,Output>
    {
      //! Must access to compute_internal()
      friend class detail::internal::scalar<interface<Scalar,Output> >;

    public:
      //! Compute output from input data.
      /*!
        Sequentially computes a range of input values and returns the result.

        \param input The input signal to be processed.
        \return The output processed signal

        \todo document the polymorphism
      */
      template<class InputType>
      inline std::vector<InputType> compute(const InputType& input)
      {
        // Any non arithmetic type is considered as a container
        return std::conditional<std::is_arithmetic<InputType>::value,
                                detail::internal::scalar<interface<Scalar,Output> >,
                                detail::internal::container<interface<Scalar,Output> > >
          ::type::template compute<InputType,std::vector<InputType> >(input,this);
      }


      template<class InputIterator, class OutputIterator>
      void compute(const InputIterator& first,
                   const InputIterator& last,
                   const OutputIterator& result)
      {
        std::transform(first,last,result,[&](const auto& x){return this->compute_internal(x);});
      }

      void compute(const std::size_t& xsize,
		   const std::size_t& ysize,
		   const Scalar* input,
		   Scalar* output)
      {
	for(std::size_t i=0;i<xsize;++i)
	  {
	    const auto out_i = this->compute_internal(input[i]);
	    for(std::size_t j=0;j<ysize;++j)
	      output[i*ysize+j] = out_i[j];
	  }
      }

    };
  }
}

#endif // GAMMATONE_FILTERBANK_INTERFACE_HPP
