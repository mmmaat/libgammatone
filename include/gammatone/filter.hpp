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

#ifndef GAMMATONE_FILTER_HPP
#define GAMMATONE_FILTER_HPP

#include <gammatone/detail/interface.hpp>
#include <gammatone/core/cooke1993.hpp>  // default core
#include <gammatone/policy/gain.hpp>
#include <gammatone/policy/bandwidth.hpp>
#include <gammatone/policy/clipping.hpp>

namespace gammatone
{
    //! Gammatone filter implementation
    /*!
      \class filter gammatone/filter.hpp

      This class models a single gammatone filter. Processing core as
      well as bandwidth and clipping policies are templates.

      \tparam Scalar                Type of the scalars.
      \tparam Core                  Type of the processing core.
      \tparam BandwidthPolicy       Policy for computing filter bandwidth.
      \tparam ClippingPolicy        Policy for clipping.
    */
    template
    <
        class Scalar,
        template<class...> class Core         = core::cooke1993,
        template<class> class BandwidthPolicy = policy::bandwidth::glasberg1990,
        class ClippingPolicy                  = policy::clipping::off
        >
    class filter : public detail::interface<Scalar,Scalar>
    {
    public:

        //! Type of this filter
        using type = filter<Scalar, Core, BandwidthPolicy, ClippingPolicy>;

        //! Type of the inherited interface
        using base = detail::interface<Scalar, Scalar>;

        //! Type of the filter core
        using core = Core<Scalar, policy::gain::forall_0dB, ClippingPolicy>;

        //! Type of the scalars
        using scalar_type = typename base::scalar_type;

        //! Type of the output is also scalar
        using output_type = typename base::output_type;


        //! Creates a gammatone filter from explicit parameters.
        /*!
          \param sample_frequency The input signal sample frequency (Hz).
          \param center_frequency The filter center frequency (Hz).
        */
        filter(const Scalar& sample_frequency,
               const Scalar& center_frequency)
            : base(sample_frequency),
              m_center_frequency(center_frequency),
              m_bandwidth(BandwidthPolicy<Scalar>::bandwidth(center_frequency)),
              m_core(base::sample_frequency(), m_center_frequency, m_bandwidth)
            {}

        //! Copy constructor
        filter(const type& other)
            : base(other),
              m_center_frequency(other.m_center_frequency),
              m_bandwidth(other.m_bandwidth),
              m_core(other.m_core)
            {}


        //! Move constructor
        filter(type&& other) noexcept
            : base(std::move(other)),
              m_center_frequency(std::move(other.m_center_frequency)),
              m_bandwidth(std::move(other.m_bandwidth)),
              m_core(std::move(other.m_core))
            {}


        //! Assignment operator
        type& operator=(const type& other){
            base::operator=(other);

            type tmp(other);
            std::swap(m_core, tmp.m_core);
            std::swap(m_center_frequency, tmp.m_center_frequency);
            std::swap(m_bandwidth, tmp.m_bandwidth);

            return *this;
        }


        //! Move operator
        type& operator=(type&& other){
            base::operator=(std::move(other));

            m_center_frequency = std::move(other.m_center_frequency);
            m_bandwidth = std::move(other.m_bandwidth);
            m_core = std::move(other.m_core);

            return *this;
        }


        //! Destructor
        virtual ~filter(){}

        //! Inherited from interface
        inline Scalar center_frequency() const{
            return m_center_frequency;
        }

        inline Scalar bandwidth() const{
            return m_bandwidth;
        }

        inline Scalar gain() const{
            return m_core.gain();
        }

        inline void reset(){
            m_core.reset();
        }


        //! Compute a scalar output from a scalar input
        /*!
          \param input   The scalar value to be processed
          \param output  The computed output value
        */
        inline void compute(const scalar_type& input, output_type& output){
            m_core.compute(input, output);
        }

        inline void compute_ptr(const std::size_t& size,
                                const Scalar* input,
                                Scalar* output){
            for(std::size_t i=0; i < size; ++i){
                compute(input[i], output[i]);
            }
        }


    private:
        //! Filter center frequency (Hz)
        Scalar m_center_frequency;

        //! Filter bandwidth (Hz)
        Scalar m_bandwidth;

        //! The underlying processing core
        core m_core;
    };
}

#endif // GAMMATONE_FILTER_HPP
