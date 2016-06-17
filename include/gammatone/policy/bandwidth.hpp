/*
  Copyright (C) 2015, 2016 Mathieu Bernard <mathieu_bernard@laposte.net>

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

#ifndef GAMMATONE_POLICY_BANDWIDTH_HPP
#define GAMMATONE_POLICY_BANDWIDTH_HPP

#include <gammatone/policy/bandwidth/slaney1988.hpp>
#include <gammatone/policy/bandwidth/glasberg1990.hpp>
#include <gammatone/policy/bandwidth/greenwood1990.hpp>

/*!
  \namespace gammatone::policy::bandwidth

  \brief Policies for filter bandwidth computation

  This namespace provides 3 sets of parameters for computing the
  bandwidth of a gammatone filter, given its center frequency
  \cite Slaney1993 .

  According to Slaney \cite Slaney1993 , a general form for the
  bandwidth \f$ b \f$ of a cochlear channel as a function of its
  center frequency \f$ f_c \f$ is given as : \f[ b(f_c) =
  ((\frac{f_c}{Q})^n + b_m^n)^{1/n},\f] where \f$ Q \f$ is the
  asymptotic filter quality at large frequencies (refered as earq
  in code and Slaney1993), \f$ b_m \f$ is the minimal bandwidth at
  low frequencies (refered as minbw) and \f$ n \f$ is the
  bandwidth order (refered as order).

  TODO Update this documentation
*/


#endif // GAMMATONE_POLICY_BANDWIDTH_HPP
