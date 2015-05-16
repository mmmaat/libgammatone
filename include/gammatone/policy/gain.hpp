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

#ifndef GAMMATONE_POLICY_GAIN_HPP
#define GAMMATONE_POLICY_GAIN_HPP

namespace gammatone
{
  namespace policy
  {
    //! Policies for filter gain computation
    /*!
      \namespace gammatone::policy::gain

      This namespace provides a set of policies used to set up the
      gain of a gammatone filter.
    */
    namespace gain
    {
      //! Disable gain policy
      class off
      {
        template<class Scalar>
        static inline Scalar gain(){ return static_cast<Scalar>(1);}
      };

      //! Unitary gain for all channels
      class one
      {
        template<class Scalar>
        static inline Scalar gain(){ return static_cast<Scalar>(1);}
      };
    }
  }
}

#endif // GAMMATONE_POLICY_GAIN_HPP
