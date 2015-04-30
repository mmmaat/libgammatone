#include <gammatone/filter.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/holdsworth.hpp>
#include <gammatone/impulse_response.hpp>
#include <gnuplot-iostream.h>
#include <iostream>

using namespace std;

typedef double T;
const T sample_frequency = 44100;
const T center_frequency = 1000;
const T duration = 0.001;

void test_slaney1993(const T& fs, const T& fc, const T& d);
// void test_holdsworth(const T& fs, const T& fc, const T& d);
// void test_holdsworth_rir(const T& fs, const T& fc, const T& d);
template<class T> void plot(const T& data, const string& name);



int main()
{
  test_slaney1993(sample_frequency, center_frequency, duration);
  //test_holdsworth_rir(sample_frequency, center_frequency, duration);
  
  return 0;
}




template<class T>
void plot(const T& data, const string& name)
{
  Gnuplot gp;
  gp << ifstream("setup.gp").rdbuf() << endl
     << "plot '-' u 1:2 w l ls 11 lw 1.5 t '"<<name<<"'"<< endl;
  gp.send1d(data);
}


void test_slaney1993(const T& fs, const T& fc, const T& d)
{
  gammatone::filter<T,gammatone::core::slaney1993<T> > f(fs, fc);
  plot(gammatone::impulse_response::implemented(f, d), "slaney1993");
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

