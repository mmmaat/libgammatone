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

#ifndef GAMMATONE_FACTORY_HPP
#define GAMMATONE_FACTORY_HPP

#include <gammatone/filter.hpp>
#include <gammatone/filterbank.hpp>
#include <string>

namespace gammatone
{
  class factory
  {
  public:
    template<class Scalar>
    static gammatone::interface& filter(const Scalar& sample_frequency,
			  const Scalar& center_frequency,
			  const std::string& setup = "")
    {
      return gammatone::filter<Scalar>(sample_frequency,center_frequency);
    }
  };
}

#endif // GAMMATONE_FACTORY_HPP
