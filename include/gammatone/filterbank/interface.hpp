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

#ifndef GAMMATONE_FILTERBANK_INTERFACE_HPP
#define GAMMATONE_FILTERBANK_INTERFACE_HPP

#include <gammatone/interface.hpp>
#include <vector>

namespace gammatone
{
  namespace filterbank
  {
    template<class Scalar>
    class interface : public ::gammatone::interface<Scalar, std::vector<Scalar> >
    {};
  }
}

#endif // GAMMATONE_FILTERBANK_INTERFACE_HPP
