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

#ifndef GAMMATONE_CORE_HOLDSWORTH_HPP
#define GAMMATONE_CORE_HOLDSWORTH_HPP

#include <gammatone/core/holdsworth_rir.hpp>

namespace gammatone
{
  namespace core
  {
    //! Gammatone filter implementation from \cite Holdsworth1988 .
    template<class Scalar>
    class holdsworth
    {
    public:
      void reset();

      template<class Filter>
      void init(const Filter& filter);

      template<class Filter>
      inline Scalar compute(const Filter& filter, const Scalar& input);

      Scalar gain() const {return m_gain;}

    private:
      std::array<holdsworth_rir<Scalar>,4> m_filter;
      Scalar m_gain;
    };
  }
}

template<class Scalar> template<class Filter>
void gammatone::core::holdsworth<Scalar>::init(const Filter& filter)
{
  m_gain = 1;
  for(auto& f : m_filter) f.init(filter.sample_frequency(),filter.center_frequency(),filter.bandwidth());
}

template<class Scalar>
void gammatone::core::holdsworth<Scalar>::reset()
{
  for(auto& f:m_filter) f.reset();
}

template<class Scalar> template<class Filter>
Scalar gammatone::core::holdsworth<Scalar>::compute(const Filter& filter, const Scalar& input)
{
  return std::move(m_filter[3].compute(m_filter[2].compute(m_filter[1].compute(m_filter[0].compute(input)))));
}

#endif // GAMMATONE_CORE_HOLDSWORTH_HPP
