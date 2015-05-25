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


#ifndef GAMMATONE_POLICY_CLIPPING_HPP
#define GAMMATONE_POLICY_CLIPPING_HPP

#include <complex>

namespace gammatone
{
  namespace policy
  {
    //! Policy for clipping of very small numbers
    /*!
      \namespace gammatone::policy::clipping

      This namespace provides a clipping policy allowing to clip very
      small numbers to zero. This prevents gradual overflow which can
      happen if the input signal contains many zeros.

      - The clipping policy is build on the classes clipping::on and
      clipping::off implementing a clip method

      ~~~
      using namespace gammatone;
      Scalar policy::clipping::on::clip(const Scalar& x)  // Clip very small x to 0
      Scalar policy::clipping::off::clip(const Scalar& x) // Do nothing
      ~~~

      - Use the policy as a template parameter of core::cooke1993

      ~~~
      typedef core::cooke1993<double, policy::clipping::on> clipped_core;
      ~~~

      \see This solution have been proposed by \cite Ma2006
      \todo Let the user change the small value -> no magic number.
      \todo Is it really usefull? Prove it on a test.
    */
    namespace clipping
    {
      //! Clipping enabled
      class on
      {
      public:
        //! Clip very small input to zero
        /*!
          \param x  The input value to clip
          \return 0 if |x| < 1e-200, x else
        */
	template<class Scalar>
        static inline Scalar clip(const Scalar& x);
      };

      //! Clipping disabled
      class off
      {
      public:
        //! Do nothing
        /*!
          \param x The input value
          \return x
        */
        template<class Scalar>
        static inline Scalar clip(const Scalar& x);
      };
    }
  }
}

template<class Scalar>
Scalar gammatone::policy::clipping::on::clip(const Scalar& x)
{
  static const typename Scalar::value_type small = 1e-200;
  if(std::abs(x)<small) return static_cast<Scalar>(0);
  else return x;
}

template<class Scalar>
Scalar gammatone::policy::clipping::off::clip(const Scalar& x)
{
  return x;
}



#endif // GAMMATONE_POLICY_CLIPPING_HPP
