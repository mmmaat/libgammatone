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

#ifndef GAMMATONE_POLICY_POLICY_HPP
#define GAMMATONE_POLICY_POLICY_HPP

#include <boost/noncopyable.hpp>

namespace gammatone
{
  namespace policy
  {
    //! Base class of all gammatone policies
    /*!  

      This base class simply make private constructor, destructor and
      copy members. Indeed policy classes just endow static methods.
     */
    class policy : private boost::noncopyable {};
  }
}

#endif // GAMMATONE_POLICY_POLICY_HPP
