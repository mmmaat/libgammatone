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


#ifndef GAMMATONE_INTERFACE_HPP
#define GAMMATONE_INTERFACE_HPP

//! Root namespace of the libgammatone library
/*!
  \namespace gammatone

  The gammatone namespace is the root namespace of libgammatone. The
  main classes of this namespace are gammatone::filter and
  gammatone::filterbank, wich define respectively single and multi
  channels gammatone filters. In order to be instanciated, a filter
  (or filterbank) must be associated to one of the proposed processing
  cores. A gammatone::core contains a concrete implementation of the
  gammatone filter. Moreover, a set of gammatone::policy classes
  allows the static customization of the filters.
*/
namespace gammatone
{
  //! Generic interface for both gammatone filters and filterbanks
  /*!
    \class interface

    This abstract class provides a common generic interface for both
    gammatone filters and filterbanks(i.e. single and multi channels
    filters). Provide a definition of the scalar type, common
    accessors and a reset() method.

    \tparam Scalar  Type of the scalar values (usually double).

    \tparam Output Type of accessors results. Output is either a
    Scalar for modeling a single channel filter or any container of
    Scalar for modeling a filterbank.
  */
  template<class Scalar, class Output>
  class interface
  {
  public:
    //! The type of scalar values
    typedef Scalar scalar_type;

    //! Accessor to the sample frequency
    /*!
      Accessor to the processing sample frequency (Hz).
      \return The sample frequency.
    */
    virtual Scalar sample_frequency() const = 0;

    //! Accessor to the center frequency
    /*!
      Accessor to the center frequencies of each channel in the filterbank (in Hz).
      \return The center frequencies of each cochlear channel.
    */
    virtual Output center_frequency() const = 0;

    //! Accessor to the bandwidth
    /*!
      Accessor to the bandwidth of each channel in the filterbank (in Hz).
      \return The bandwidth of each cochlear channel.
    */
    virtual Output bandwidth() const = 0;

    //! Accessor to the gain
    /*!
      Accessor to the gain of each channel in the filterbank.
      \return The gain of each cochlear channel.

      \todo return it in dB ?
    */
    virtual Output gain() const = 0;

    //! Clean internal buffers
    /*!
      Fill internal buffers with zeros and restore the filterbank
      state as a newly created one.
    */
    virtual void reset() = 0;
  };
}

#endif // GAMMATONE_INTERFACE_HPP
