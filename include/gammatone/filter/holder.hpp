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

#ifndef GAMMATONE_FILTER_HOLDER_HPP
#define GAMMATONE_FILTER_HOLDER_HPP

#include <gammatone/filter/interface.hpp>
#include <gammatone/filter/concrete.hpp>
#include <gammatone/detail/factory.hpp>
#include <string>

namespace gammatone
{
  namespace filter
  {
    //! Gammatone filter holder
    /*!
      \class holder

      This class models a single gammatone filter. Processing core,
      bandwidth computation and postprocessing are specified by string
      during construction. Internally this class holds a pointer to a
      filter::concrete.

      \tparam Scalar  Type of the scalars.
    */
    template<class Scalar>
    class holder : public gammatone::filter::interface<Scalar>
    {
    public:

      //! Creates a gammatone filter from explicit parameters.
      /*! 
	\param sample_frequency The input signal sample frequency (Hz).
        \param center_frequency The filter center frequency (Hz).
	\param params A parameters string specifying core, bandwidth
	and postprocessing policies.

	\todo Comment params !
      */
      holder(const Scalar& sample_frequency,
             const Scalar& center_frequency,
             const std::string params = "");

      //! Copy constructor
      holder(const holder<Scalar>& other);

      //! Assignment operator
      holder<Scalar>& operator=(const holder<Scalar>& other);

      //! Destructor
      virtual ~holder();

      // Methods inherited from filter::interface
      inline Scalar sample_frequency() const;
      inline Scalar center_frequency() const;
      inline Scalar bandwidth() const;
      inline Scalar gain() const;
      inline void reset();

    protected:

      // Method inherited from filter::interface
      inline Scalar compute_internal(const Scalar& input);
      
      //! A pointer to the concrete filter
      typename factory<Scalar>::shared_ptr p_filter;
    };
  }
}

template<class Scalar>
gammatone::filter::holder<Scalar>::
holder(const Scalar& sample_frequency,
       const Scalar& center_frequency,
       const std::string params)
  : p_filter(gammatone::factory<double>::filter(sample_frequency,center_frequency,params))
{}

template<class Scalar>
gammatone::filter::holder<Scalar>::
holder(const gammatone::filter::holder<Scalar>& other)
  : p_filter(other.p_filter)
{}

template<class Scalar>
gammatone::filter::holder<Scalar>& gammatone::filter::holder<Scalar>::
operator=(const gammatone::filter::holder<Scalar>& other)
{
  p_filter = other.p_filter;
}

template<class Scalar> gammatone::filter::holder<Scalar>::
~holder()
{}

template<class Scalar>
Scalar gammatone::filter::holder<Scalar>::
center_frequency() const
{
  return p_filter->center_frequency();
}

template<class Scalar>
Scalar gammatone::filter::holder<Scalar>::
sample_frequency() const
{
  return p_filter->sample_frequency();
}

template<class Scalar>
Scalar gammatone::filter::holder<Scalar>::
bandwidth() const
{
  return p_filter->bandwidth();
}

template<class Scalar>
Scalar gammatone::filter::holder<Scalar>::
gain() const
{
  return p_filter->gain();
}

template<class Scalar>
void gammatone::filter::holder<Scalar>::
reset()
{
  p_filter->reset();
}

template<class Scalar>
Scalar gammatone::filter::holder<Scalar>::
compute_internal(const Scalar& input)
{
  return p_filter->compute(input);
}

#endif // GAMMATONE_FILTER_HOLDER_HPP
