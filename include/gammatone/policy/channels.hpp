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

#ifndef GAMMATONE_POLICY_CHANNELS_HPP
#define GAMMATONE_POLICY_CHANNELS_HPP

#include <gammatone/policy/channels/fixed_size.hpp>
#include <gammatone/policy/channels/fixed_overlap.hpp>

/*!
  \namespace gammatone::policy::channels

  \brief Policy for frequency channels spacing

  Classes in this namespace are in charge of frequency sampling
  and attribution of a center frequency to each filter in a
  filterbank. This computation depends of the number of channels
  you want in the bank, or equivalently to an overlap factor
  between successive filters. An overlap factor close to zero
  indicate filters nearly completly overlapped, an overlap factor
  of 0.5 means that each frequency in the input signal is
  "sampled" by two cochlear channels, and an overlap factor of 1
  means that there is almost no overlap between channels. Those
  algorithms are implemented after \cite Slaney1993 .

  - The class channels::fixed_size let the user determines the
  number of frequency channels in the bank and setup the related
  overlap.

  - The class channels::fixed_overlap let the user determines the
  overlap between successive filters in the bank. Also estimates
  the related number of channels.

  - A channel spacing policy accept a policy::order as template
  parameter. The default is to use policy::order::increasing.

  - Use this policy as a template parameter of filterbank::concrete.
  ~~~

  Usage
  -----

  Here is a minimal example of the channels policy:

  \include channels.cpp
*/


#endif // GAMMATONE_POLICY_CHANNELS_HPP
