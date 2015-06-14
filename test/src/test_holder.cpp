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

#include <gammatone/filter.hpp>
#include <gammatone/detail/utils.hpp>
#include <iostream>
#include <vector>
#include <exception>

using namsespace gammatone;

void check(bool b){  if(!b) throw std::exception(); }

template<template<class...> class Core>
void test(const std::string& name,
          const std::vector<double>& input)
{
  filter::holder<double>          f1(20000,3000,name);
  filter::concrete<double, Core>  f2(20000,3000);

  std::for_each(input.begin(),input.end(), [&](const auto& x)
                {check((f1.compute(x)) == f2.compute(x));});
}

int main()
{
  std::vector<double> input(10000);
  input[0] = 1.0;

  try
    {
      test<core::cooke1993>("cooke1993", input);
      test<core::slaney1993>("slaney1993", input);
    }
  catch(const std::exception e)
    {
      std::cout << "error detected !" << std::endl;
      return -1;
    }

  std::cout << "all asserts verified" << std::endl;
  return 0;
}
