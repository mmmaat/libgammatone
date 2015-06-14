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

#ifndef GAMMATONE_FILTERBANK_HOLDER_HPP
#define GAMMATONE_FILTERBANK_HOLDER_HPP

#include <gammatone/filterbank/interface.hpp>
#include <gammatone/filterbank/concrete.hpp>
#include <gammatone/filterbank/factory.hpp>
#include <string>

namespace gammatone
{
  namespace filterbank
  {
    //! Gammatone filterbank holder
    /*!
      \class holder

      This class models a gammatone filterbank. Processing core,
      computation of center frequencies and bandwidth, as well as
      postprocessing are specified by string during construction.
      Internally this class holds a pointer to a filterbank::concrete.

      \tparam Scalar  Type of the scalars.
    */
    template<class Scalar>
    class holder : public ::gammatone::filterbank::interface<Scalar>
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

      // Methods inherited from filterbank::interface
      inline Scalar sample_frequency() const;
      inline Scalar center_frequency() const;
      inline Scalar bandwidth() const;
      inline Scalar gain() const;
      inline void reset();

    protected:

      // Method inherited from filterbank::interface
      inline Scalar compute_internal(const Scalar& input);
      
      //! A pointer to the concrete filter
      typename factory<Scalar>::filterbank_ptr p_filterbank;
    };
  }
}


template<class Scalar>
gammatone::filterbank::holder<Scalar>::
holder(const Scalar& sample_frequency,
       const Scalar& center_frequency,
       const std::string params)
  : p_filterbank(gammatone::detail::singleton<gammatone::filter::factory<Scalar> >::instance().
		 create(sample_frequency,center_frequency,params))
{}

template<class Scalar>
gammatone::filterbank::holder<Scalar>::
holder(const gammatone::filterbank::holder<Scalar>& other)
  : p_filterbank(other.p_filterbank)
{}

template<class Scalar>
gammatone::filterbank::holder<Scalar>& gammatone::filterbank::holder<Scalar>::
operator=(const gammatone::filterbank::holder<Scalar>& other)
{
  p_filterbank = other.p_filterbank;
}

template<class Scalar> gammatone::filterbank::holder<Scalar>::
~holder()
{}

template<class Scalar>
Scalar gammatone::filterbank::holder<Scalar>::
center_frequency() const
{
  return p_filterbank->center_frequency();
}

template<class Scalar>
Scalar gammatone::filterbank::holder<Scalar>::
sample_frequency() const
{
  return p_filterbank->sample_frequency();
}

template<class Scalar>
Scalar gammatone::filterbank::holder<Scalar>::
bandwidth() const
{
  return p_filterbank->bandwidth();
}

template<class Scalar>
Scalar gammatone::filterbank::holder<Scalar>::
gain() const
{
  return p_filterbank->gain();
}

template<class Scalar>
void gammatone::filterbank::holder<Scalar>::
reset()
{
  p_filterbank->reset();
}

template<class Scalar>
Scalar gammatone::filterbank::holder<Scalar>::
compute_internal(const Scalar& input)
{
  return p_filterbank->compute(input);
}


#endif // GAMMATONE_FILTERBANK_HOLDER_HPP
