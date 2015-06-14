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

#ifndef GAMMATONE_FACTORY_HPP
#define GAMMATONE_FACTORY_HPP

// #include <gammatone/filter/factory.hpp>
// #include <gammatone/filterbank/factory.hpp>
// #include <gammatone/detail/singleton.hpp>

// namespace gammatone
// {
//   //! The bridge between concrete and holder implementation
//   /*!  
//     The factory creates a concrete filter or filterbank and returns a
//     pointer to it. The precise concrete type (*i.e.* template
//     arguments) is parsed from a parameter string. The pointer is then
//     encapsulated in a holder.

//     \tparam Scalar  Type of scalar values in the created filters

//     \todo Comment params string !
//   */
//   template<class Scalar>
//   class factory : boost::noncopyable
//   {
//   public:

//     //! Shared pointer to concrete filter
//     //    typedef typename filter::factory<Scalar>::filter_ptr  filter_ptr;
//     using filter_ptr = typename filter::factory<Scalar>::shared_ptr;

//     //! Shared pointer to concrete filterbank
//     using filterbank_ptr = typename filterbank::factory<Scalar>::shared_ptr;
    
    
//     //! Creates a concrete filter from a parameter string
//     /*!
//       \param sample_frequency  The processing sample frequency (Hz)
//       \param center_frequency  The filter center frequency (Hz)
//       \param params            Parameters string

//       \return A shared pointer to the created filter
//      */
//     // static filter_ptr filter(const Scalar& sample_frequency,
//     //                          const Scalar& center_frequency,
//     //                          const std::string& params);

//     //! Creates a concrete filterbank from a parameter string
//     /*!
//       \param sample_frequency  The processing sample frequency (Hz)
//       \param low_frequency     Lowest center frequency (Hz)
//       \param high_frequency    Highest center frequency (Hz)
//       \param params            Parameters string

//       \return A shared pointer to the created filterbank
//     */
//     static filterbank_ptr filterbank(const Scalar& sample_frequency,
//                                      const Scalar& low_frequency,
//                                      const Scalar& high_frequency,
//                                      const std::string& params);
//   };
// }

// template<class Scalar>
// typename gammatone::factory<Scalar>::filter_ptr gammatone::factory<Scalar>::
// filter(const Scalar& sample_frequency,
//        const Scalar& center_frequency,
//        const std::string& params)
// {
//   // delegation to the singleton filter factory
//   typedef typename detail::singleton<filter::factory<Scalar> > f;
//   return f::instance().create(sample_frequency,center_frequency,params);
// }

// template<class Scalar>
// typename gammatone::factory<Scalar>::filterbank_ptr
// gammatone::factory<Scalar>::
// filterbank(const Scalar& sample_frequency,
//            const Scalar& low_frequency,
//            const Scalar& high_frequency,
//            const std::string& params)
// {
//   // delegation to the singleton filterbank factory
//   typedef typename detail::singleton<filterbank::factory<Scalar> > f;
//   return f::instance().create(sample_frequency,low_frequency,high_frequency,params);
// }

#endif // GAMMATONE_FACTORY_HPP
