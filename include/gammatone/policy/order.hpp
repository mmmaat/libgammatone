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

#ifndef GAMMATONE_POLICY_ORDER_HPP
#define GAMMATONE_POLICY_ORDER_HPP

namespace gammatone
{
  namespace policy
  {
    //! Policy for frequency channels ordering
    /*!
      \namespace gammatone::policy::order

      - This namespace is built on the classes order::increasing and
      order::decreasing, allowing to sort channels of a filterbank by
      increasing or decreasing frequencies.

      - The cochlea decomposes incoming sounds from high to low
      frequencies. This results in channels sorted by decreasing
      frequencies.

      - Nevertheless it can be more intuitive to have channels sorted
      in increasing frequencies.

      - Use this policy as template parameter of a policy::channels.

      ~~~
      // Create two types of filterbank differing only by their ordering policy
      using namespace gammatone;

      typedef policy::channels::fixed_overlap<policy::channels::increasing> increasing_policy;
      typedef policy::channels::fixed_overlap<policy::channels::decreasing> decreasing_policy;

      typedef filterbank<double, core::cooke1993<double>, increasing_policy> increasing_filterbank;
      typedef filterbank<double, core::cooke1993<double>, decreasing_policy> decreasing_filterbank;
      ~~~
    */
    namespace order
    {
      //! Sort frequency channels by increasing order
      /*!
        \tparam Container The type container on which iterate. Must
        define a bidirectionnal iterator.
      */
      class increasing
      {
      public:
        //! Returns an iterator on the first (lowest) frequency channel
        /*!
          \param container   The container on which iterate
          \return An iterator on the first frequency channel
        */
        template<class Container>
        static inline typename Container::reverse_iterator begin(Container& container);

        //! Returns an iterator on the last (highest) frequency channel
        /*!
          \param container   The container on which iterate
          \return An iterator after the last frequency channel
        */
        template<class Container>
        static inline typename Container::reverse_iterator end(Container& container);

        //! Returns a const iterator on the first (lowest) frequency channel
        /*!
          \param container   The const container on which iterate
          \return A const iterator on the first frequency channel
        */
        template<class Container>
        static inline typename Container::const_reverse_iterator begin(const Container& container);

        //! Returns a const iterator on the last (highest) frequency channel
        /*!
          \param container   The const container on which iterate
          \return A const iterator after the last frequency channel
        */
        template<class Container>
        static inline typename Container::const_reverse_iterator end(const Container& container);

      };

      //! Sort frequency channels by decreasing order
      /*!
        \tparam Container  The type container on which iterate. Must
        define a forward iterator.
      */
      class decreasing
      {
      public:
        //! Returns an iterator on the first (highest) frequency channel
        /*!
          \param container   The container on which iterate
          \return An iterator on the first frequency channel
        */
        template<class Container>
        static inline typename Container::iterator begin(Container& container);

        //! Returns an iterator on the last (lowest) frequency channel
        /*!
          \param container   The container on which iterate
          \return An iterator after the last frequency channel
        */
        template<class Container>
        static inline typename Container::iterator end(Container& container);

        //! Returns a const iterator on the first (highest) frequency channel
        /*!
          \param container   The const container on which iterate
          \return A const iterator on the first frequency channel
        */
        template<class Container>
        static inline typename Container::const_iterator begin(const Container& container);

        //! Returns a const iterator on the last (lowest) frequency channel
        /*!
          \param container   The const container on which iterate
          \return A const iterator after the last frequency channel
        */
        template<class Container>
        static inline typename Container::const_iterator end(const Container& container);

      };
    }
  }
}

/*
  non const version
*/

template<class Container>
typename Container::reverse_iterator gammatone::policy::order::increasing::begin(Container& container)
{
  return container.rbegin();
}

template<class Container>
typename Container::reverse_iterator gammatone::policy::order::increasing::end(Container& container)
{
  return container.rend();
}

template<class Container>
typename Container::iterator gammatone::policy::order::decreasing::begin(Container& container)
{
  return container.begin();
}

template<class Container>
typename Container::iterator gammatone::policy::order::decreasing::end(Container& container)
{
  return container.end();
}


/*
  const version
*/


template<class Container>
typename Container::const_reverse_iterator gammatone::policy::order::increasing::begin(const Container& container)
{
  return container.rbegin();
}

template<class Container>
typename Container::const_reverse_iterator gammatone::policy::order::increasing::end(const Container& container)
{
  return container.rend();
}

template<class Container>
typename Container::const_iterator gammatone::policy::order::decreasing::begin(const Container& container)
{
  return container.begin();
}

template<class Container>
typename Container::const_iterator gammatone::policy::order::decreasing::end(const Container& container)
{
  return container.end();
}



#endif // GAMMATONE_POLICY_ORDER_HPP
