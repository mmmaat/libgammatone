/*
  Copyright (C) 2015 Mathieu Bernard <mathieu_bernard@laposte.net>
  Copyright (C) 1999 Matt R. Flax <https://tracker.debian.org/pkg/libgtfb>

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

#ifndef GAMMATONE_CORE_SLANEY1993_IIR_HPP
#define GAMMATONE_CORE_SLANEY1993_IIR_HPP

#include <utility>
#include <array>

namespace gammatone
{
  namespace core
  {
    //! Implementation of a 2nd order IIR from libgtfb
    template<class Scalar>
    class slaney1993_iir
    {
    public:
      slaney1993_iir(const std::array<Scalar,3>& a, const std::array<Scalar,3>& b);

      slaney1993_iir(const slaney1993_iir<Scalar>& other);
      slaney1993_iir(slaney1993_iir<Scalar>&& other) noexcept;
      
      slaney1993_iir<Scalar>& operator=(const slaney1993_iir<Scalar>& other);
      slaney1993_iir<Scalar>& operator=(slaney1993_iir<Scalar>&& other);

      virtual ~slaney1993_iir();

      inline void reset();
      inline Scalar compute(const Scalar& input);
      inline Scalar compute(const Scalar& input, const Scalar& gain);

    private:
      std::array<Scalar,3> m_a;
      std::array<Scalar,3> m_b;

      Scalar m_z1, m_z2;
    };
  }
}

template<class Scalar>
gammatone::core::slaney1993_iir<Scalar>::
slaney1993_iir(const std::array<Scalar,3>& a, const std::array<Scalar,3>& b)
  : m_a( a ),
    m_b( b )
{
  reset();
}

template<class Scalar>
gammatone::core::slaney1993_iir<Scalar>::
slaney1993_iir(const slaney1993_iir<Scalar>& other)
  : m_a( other.m_a ),
    m_b( other.m_b ),
    m_z1( other.m_z1 ),
    m_z2( other.m_z2 )
{}


template<class Scalar>
gammatone::core::slaney1993_iir<Scalar>::
slaney1993_iir(slaney1993_iir<Scalar>&& other) noexcept
  : m_a( std::move(other.m_a )),
    m_b( std::move(other.m_b )),
    m_z1( std::move(other.m_z1 )),
    m_z2( std::move(other.m_z2 ))
{}

template<class Scalar>
gammatone::core::slaney1993_iir<Scalar>& gammatone::core::slaney1993_iir<Scalar>::
operator=(const slaney1993_iir<Scalar>& other)
{
  gammatone::core::slaney1993_iir<Scalar> tmp(other);

  std::swap(m_a, tmp.m_a);
  std::swap(m_b, tmp.m_b);
  std::swap(m_z1, other.m_z1);
  std::swap(m_z2, other.m_z2);

  return *this;
}


template<class Scalar>
gammatone::core::slaney1993_iir<Scalar>& gammatone::core::slaney1993_iir<Scalar>::
operator=(slaney1993_iir<Scalar>&& other)
{
  m_a = std::move(other.m_a);
  m_b = std::move(other.m_b);
  m_z1 = std::move(other.m_z1);
  m_z2 = std::move(other.m_z2);

  return *this;
}

template<class Scalar>
gammatone::core::slaney1993_iir<Scalar>::
~slaney1993_iir()
{}

template<class Scalar>
void gammatone::core::slaney1993_iir<Scalar>::
reset()
{
  m_z1 = m_z2 = 0.0;
}

template<class Scalar>
Scalar gammatone::core::slaney1993_iir<Scalar>::
compute(const Scalar& input)
{
  Scalar out = m_a[0]*input + m_z1;
  m_z1 = m_a[1]*input - m_b[1]*out + m_z2;
  m_z2 = m_a[2]*input - m_b[2]*out;
  return out;
}

template<class Scalar>
Scalar gammatone::core::slaney1993_iir<Scalar>::
compute(const Scalar& input, const Scalar& gain)
{
  return compute(input/gain);
}

#endif // GAMMATONE_CORE_SLANEY1993_IIR_HPP
