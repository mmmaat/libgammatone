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


template<class Core>
void test(const std::string& name)
{
  typedef gammatone::filter::holder<double>          holder;
  typedef gammatone::filter::concrete<double, Core>  concrete;

  holder   f1(44100,3000,name);
  concrete f2(44100,3000);

  std::vector<double> input(10);
  input[0] = 1.0;

  // compute1
  std::for_each(input.begin(),input.end(), [&](const auto& x)
   		{assert(f1.compute(x) == f2.compute(x));});

  // compute2
  std::vector<double> out1(input.size());
  f1.compute(input.begin(),input.end(),out1.begin());

  // compute3
  auto out2 = f2.compute(input);

  for(std::size_t i=0;i<input.size();i++)
    assert(out1[i] == out2[i]);
}

int main()
{
  using namespace gammatone::core;
  test<cooke1993<double>>("cooke1993");
  test<slaney1993<double>>("slaney1993");
  std::cout << "all asserts verified" << std::endl;
  return 0;
}
