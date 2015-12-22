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

#include <memory>  // for std::move
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
          accessors and a reset() method.

          The compute() method is not defined here but in child classes as
          the interface is different for filter and filterbank.

          \tparam Scalar Type of the scalar values (usually double, be
          scared of float16).

          \tparam Output Type of accessors results. Output is either a
          Scalar for filter or any container of Scalar for filterbank.
        */
        template<class Scalar,
                 class Output>
        class interface
        {
        public:
            //! Type of this class
            using type = interface<Scalar, Output>;
        
            //! Type of scalar input values 
            using scalar_type = Scalar;

            //! Type of output values
            // Scalar for filter, vector of Scalar for filterbank
            using output_type = Output;

            //! Constructor
            explicit interface(const scalar_type& sample_frequency)
                : m_sample_frequency(sample_frequency)
                {}


            //! Copy constructor
            interface(const type& other)
                : m_sample_frequency(other.m_sample_frequency)
                {}


            //! Move constructor
            interface(type&& other) noexcept
                : m_sample_frequency(std::move(other.m_sample_frequency))
                {}


            //! Assignment operator
            type& operator=(const type& other){
                type tmp(other);
                std::swap(m_sample_frequency, tmp.m_sample_frequency);
                return *this;
            }


            //! Move operator
            type& operator=(type&& other){
                m_sample_frequency = std::move(other.m_sample_frequency);
                return *this;
            }


            //! Destructor
            virtual ~interface(){}

            //! Accessor to the sample frequency
            /*!
              Accessor to the processing sample frequency (Hz).
              \return The sample frequency.
            */
            scalar_type sample_frequency() const{
                return m_sample_frequency;
            }

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

              TODO return it in dB ?
            */
            virtual output_type gain() const = 0;

            //! Clean internal buffers
            /*!
              Fill internal buffers with zeros and restore the filterbank
              state as a newly created one.
            */
            virtual void reset() = 0;


            // //! Allocate memory for storing an output of the given size
            // /*!  

            //   Because the compute functions declared in this class do
            //   not allocate any data, output is writed on preallocated
            //   data. This function simply create an output buffer and
            //   reserve for space in it.

            //   \param size  The number of output samples to store
            // */
            // virtual output_type reserve_output(const std::size_t& size = 1) = 0;
            
            //! Compute a scalar output from a scalar input
            /*!
              \param input   The scalar value to be processed
              \param output  The computed output value
            */
            virtual void compute(const scalar_type& input, output_type& output) = 0;

            
            //! Compute an input iterator range
            /*!
              Sequentially computes an input range of values and stores the
              result in an output range of values.
              
              \tparam InputIterator   Iterator on the input range.
              \tparam OutputIterator  Iterator on the output range.

              \param first  Iterator to the initial position of the input range.
              \param last   Iterator to the final position of the input range.
              \param result Iterator to the initial position of the output range.

              \attention This method does not allocate any data. The
              output range must include at least elements as
              std::distance(first, last).
            */
            template<class InputIterator, class OutputIterator>
            inline void compute_range(const InputIterator& first,
                                      const InputIterator& last,
                                      const OutputIterator& result){
                auto it = result;
                std::for_each(first, last,
                              [&](const scalar_type& x){compute(x, *it++);});
            }

            //! Compute scalar values from/to pointers
            /*!
          
              This is a legacy C style compute function.

              \param size    Number of scalars to read/write in input and
              output ranges.
              \param input   A pointer to the input range
              \param output  A pointer to the output range

              \attention This method does not allocate any data, input and
              output must point to some memory allocated for at least
              `size` scalars.
            */
            virtual void compute_ptr(const std::size_t& size,
                                     const scalar_type* input,
                                     scalar_type* output) = 0;
            
        private:
            //! Processing sample frequency (Hz)
            scalar_type m_sample_frequency;
        };
    }
}

#endif // GAMMATONE_DETAIL_INTERFACE_HPP
