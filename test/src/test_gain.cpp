#include <gammatone/filterbank.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/convolution.hpp>
#include <utils/range.hpp>
#include <utils/utils.hpp>

#include <fstream>
#include <string>
#include <sstream>
#include <gnuplot-iostream.h>


// filterbank types
typedef double T;
typedef gammatone::filterbank<T,gammatone::core::convolution<T> > filterbank1;
typedef gammatone::filterbank<T,gammatone::core::slaney1993<T> >  filterbank2;

typedef gammatone::policy::gain::old_cooke1993  gain1;
typedef gammatone::policy::gain::forall_0dB     gain2;
typedef gammatone::policy::gain::peroctave_6dB  gain3;
typedef gammatone::policy::gain::off  gain4;
typedef gammatone::filterbank<T,gammatone::core::cooke1993<T, gain1> >   filterbank31;
typedef gammatone::filterbank<T,gammatone::core::cooke1993<T, gain2> >   filterbank32;
typedef gammatone::filterbank<T,gammatone::core::cooke1993<T, gain3> >   filterbank33;
typedef gammatone::filterbank<T,gammatone::core::cooke1993<T, gain4> >   filterbank34;

// filterbank parameters
const T fs = 44100;           // Hz
const T lf = 1000, hf = 5000; // Hz
const T duration = 0.01;      // s


// for plotting
const std::string gnuplot_setup = "/home/mathieu/dev/libgammatone/share/setup.gp";

template<class DataType> void plot(std::vector<T> fc, std::vector<DataType> data)
{
  Gnuplot gp;
  gp << std::ifstream(gnuplot_setup).rdbuf() << std::endl
     << "set xlabel 'frequency (Hz)'" << std::endl
     << "set ylabel 'normalized gain (dB)'" << std::endl
     << "set key top right" << std::endl
     << "plot ";

  std::stringstream cmd;
  unsigned color = 0;
  for(const auto& t:data)
    cmd << "'-' u 1:2 w l ls "<<11+color++<<" t '"<<std::get<1>(t)<<"', ";
  gp << cmd.str().substr(0,cmd.str().size()-2) << std::endl; // rm the last 2 characters (= ' ,')

  for(const auto& t:data)
    gp.send1d(std::make_pair(fc,std::get<0>(t)));
}

int main()
{
  // center frequencies
  const auto fc = filterbank1(fs,lf,hf).center_frequency();

  // gain of each filterbank type
  typedef std::tuple<std::vector<T>,const std::string> DataType;
  std::vector<DataType> data_base;
  data_base.push_back(make_tuple(filterbank1(fs,lf,hf).gain(), "convolution"));
  data_base.push_back(make_tuple(filterbank2(fs,lf,hf).gain(), "slaney"));
  //  data_base.push_back(make_tuple(filterbank31(fs,lf,hf).gain(), "cooke old"));
  data_base.push_back(make_tuple(filterbank32(fs,lf,hf).gain(), "cooke 0dB"));
  data_base.push_back(make_tuple(filterbank33(fs,lf,hf).gain(), "cooke 6dB"));
  //  data_base.push_back(make_tuple(filterbank34(fs,lf,hf).gain(), "cooke off"));

  for(auto& data:data_base)
    {
      auto& d = std::get<0>(data);
      utils::decibel(d.begin(),d.end());
    }
  
  plot(fc, data_base);

  return 0;
}
