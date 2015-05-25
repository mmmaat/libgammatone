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

#ifndef GAMMATONE_POLICY_POSTPROCESSING_HPP
#define GAMMATONE_POLICY_POSTPROCESSING_HPP

namespace gammatone
{
  namespace policy
  {
    //! Policies for gammatone filter postprocessing
    /*!
      \namespace gammatone::policy::postprocessing

      Postprocessing policies used in the compute() methods in filter
      and filterbank. A postprocessing policy must implement the
      'static Scalar process(const Scalar&)' method.

      \todo Implement postprocessing::peak
    */
    namespace postprocessing
    {
      //! Postprocessing disabled
      /*!
        \class off.
        \tparam Scalar  Type of scalar values.
      */
      template<class Scalar> class off
      {
      public:
        //! Do nothing
        static inline Scalar process(const Scalar& x);
      };

      //! Half-wave rectification
      /*!
        \class hwr
        \tparam Scalar  Type of scalar values
      */
      template<class Scalar> class hwr
      {
      public:
        //! Put the negative values to 0.
        static inline Scalar process(const Scalar& x);
      };
    }
  }
}

template<class Scalar>
Scalar gammatone::policy::postprocessing::off<Scalar>::process(const Scalar& x)
{
  return x;
}

template<class Scalar>
Scalar gammatone::policy::postprocessing::hwr<Scalar>::process(const Scalar& x)
{
  return std::max(static_cast<Scalar>(0),x);
}
  
#endif // GAMMATONE_POLICY_POSTPROCESSING_HPP
