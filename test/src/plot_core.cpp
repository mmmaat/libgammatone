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

#include <gammatone/gammatone.hpp>
#include <test_setup.hpp>
#include <iostream>

using namespace std;

typedef double T;
const T sample_frequency = 44100;
const T center_frequency = 1000;
const T duration = 0.1;

void test_slaney1993(const T& fs, const T& fc, const T& d);
void test_convolution(const T& fs, const T& fc, const T& d);

// void test_holdsworth(const T& fs, const T& fc, const T& d);
// void test_holdsworth_rir(const T& fs, const T& fc, const T& d);

template<class T> void plot(const T& data, const string& name);





int main()
{
  test_convolution(sample_frequency,center_frequency,duration);
  //    test_slaney1993(sample_frequency, center_frequency, duration);
  //test_holdsworth_rir(sample_frequency, center_frequency, duration);

  return 0;
}




void test_slaney1993(const T& fs, const T& fc, const T& d)
{
  gammatone::filter::concrete<T,gammatone::core::slaney1993<T> > f(fs, fc);
  plot(gammatone::impulse_response::implemented(f, d), "slaney1993");
}



class my_convolution : public gammatone::core::convolution<T>
{
public:
  my_convolution(const T& fs, const T& fc)
    : convolution<T>( fs,fc,gammatone::policy::bandwidth::glasberg1990<T>::bandwidth(fc) ),
    m_time( gammatone::impulse_response::time(fs,(T)(m_ir.size()-1)/fs) )
  {}

  virtual ~my_convolution()
  {}

  const std::vector<T>& ir() const
  { return m_ir; }

  const std::vector<T>& time() const
  { return m_time; }
  
private:
  vector<T> m_time;
};


void test_convolution(const T& fs, const T& fc, const T& d)
{
  my_convolution core(fs,fc);
  cout << "gain = " << core.gain() << endl
       << "size = " << core.ir().size() << endl
       << "time = " << core.time().size() << endl;
  
  plot(make_pair(core.time(), core.ir()), "convolution the");

  gammatone::filter::concrete<T,gammatone::core::convolution<T> > f(fs, fc);
  plot(make_pair(gammatone::impulse_response::time(fs,d),
		 gammatone::impulse_response::implemented(f, d))
       , "convolution imp");
}



template<class T>
void plot(const T& data, const string& name)
{
  Gnuplot gp;
  gp << ifstream(gpsetup).rdbuf() << endl
     << "plot '-' u 1:2 w l ls 11 lw 1.5 t '"<<name<<"'"<< endl;
  gp.send1d(data);
}




// void test_holdsworth(const T& fs, const T& fc, const T& d)
// {
//   gammatone::filter<T,gammatone::core::holdsworth<T> > f(fs,d);
//   plot(gammatone::impulse_response::implemented(f, d), "holdsworth");
// }



// template<class T>
// class holdsworth_rir_test : public gammatone::core::holdsworth_rir<T>
// {
// public:
//   holdsworth_rir_test(const T& sample_frequency, const T& center_frequency)
//   {
//     fs = sample_frequency;
//     gammatone::filter<T,gammatone::core::holdsworth<T> > tmp(sample_frequency,center_frequency);
//     this->init(sample_frequency,center_frequency,tmp.bandwidth());
//   }

//   T sample_frequency() const{ return fs;}

//   T compute(const T& x)
//   {
//     const auto y = this->gammatone::core::holdsworth_rir<T>::compute(x);
//     //    cout << y << "    [period,t] = " << this->period << "   " << this->t << endl;
//     // cout << "z = [ " << this->z.back() << " " << this->z.front() << "]" << endl;
//     // cout << "w = [ " << this->w.back() << " " << this->w.front() << "]" << endl;

//     return y;
//   }

//   void debug()
//   {
//     cout << "[alpha,omega] = " << this->alpha << "   " << this->omega << endl
//          << "   [period,t] = " << this->period << "   " << this->t << endl;
//     //cout << "[zw] size = " << this->w.size() << " " << this->z.size() << endl;
//     cout << "z = [ " << this->z.back() << " " << this->z.front() << "]" << endl;
//     cout << "w = [ " << this->w.back() << " " << this->w.front() << "]" << endl;
//   }

// private:
//   T fs;
// };

// void test_holdsworth_rir(const T& fs, const T& fc, const T& d)
// {
//   holdsworth_rir_test<T> f(fs,fc);
//   f.debug();
//   auto ir = gammatone::impulse_response::implemented(f, d);
//   cout << endl << endl;
//   f.debug();

//   //plot(ir, "holdsworth rir");
// }
