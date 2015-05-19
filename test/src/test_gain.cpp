#include <gammatone/gammatone.hpp>
#include <utils/range.hpp>
#include <utils/utils.hpp>
#include <gnuplot-iostream.h>
#include <fstream>
#include <string>
#include <sstream>

typedef double T;

// gain types
typedef gammatone::policy::gain::forall_0dB     gain1;
typedef gammatone::policy::gain::peroctave_6dB  gain2;
//typedef gammatone::policy::gain::old_cooke1993  gain3;
//typedef gammatone::policy::gain::off            gain4;

// core type
typedef gammatone::core::convolution<T,gain1>  core11;
typedef gammatone::core::convolution<T,gain2>  core12;
typedef gammatone::core::slaney1993<T,gain1>   core21;
typedef gammatone::core::slaney1993<T,gain2>   core22;
typedef gammatone::core::cooke1993<T, gain1>   core31;
typedef gammatone::core::cooke1993<T, gain2>   core32;

// filterbank types
typedef gammatone::filterbank<T,core11>   filterbank11;
typedef gammatone::filterbank<T,core12>   filterbank12;
typedef gammatone::filterbank<T,core21>   filterbank21;
typedef gammatone::filterbank<T,core22>   filterbank22;
typedef gammatone::filterbank<T,core31>   filterbank31;
typedef gammatone::filterbank<T,core32>   filterbank32;
// typedef gammatone::filterbank<T,gammatone::core::cooke1993<T, gain3> >   filterbank33;
// typedef gammatone::filterbank<T,gammatone::core::cooke1993<T, gain4> >   filterbank34;


// filter types
typedef gammatone::filter<T,core11>   filter11;
typedef gammatone::filter<T,core12>   filter12;
typedef gammatone::filter<T,core21>   filter21;
typedef gammatone::filter<T,core22>   filter22;
typedef gammatone::filter<T,core31>   filter31;
typedef gammatone::filter<T,core32>   filter32;



// filterbank parameters
const T fs = 44100;           // Hz
const T lf = 1000, hf = 5000; // Hz

// for plotting
template<class DataType>
void plot(std::vector<T> xaxis,
          std::vector<DataType> data,
          const std::string ylabel,
          const std::string xlabel = "frequency (Hz)")
{
  Gnuplot gp;
  gp << std::ifstream("/home/mathieu/dev/libgammatone/share/setup.gp").rdbuf() << std::endl
     << "set xlabel '"<<xlabel<<"'" << std::endl
     << "set ylabel '"<<ylabel<<"'" << std::endl
     << "set key top right" << std::endl
     << "plot ";

  std::stringstream cmd;
  unsigned color = 0;
  for(const auto& d:data)
    cmd << "'-' u 1:2 w l ls "<<11+color++<<" t '"<<std::get<1>(d)<<"', ";
  gp << cmd.str().substr(0,cmd.str().size()-2) << std::endl; // rm the last 2 characters (= ' ,')

  for(const auto& d:data)
    gp.send1d(std::make_pair(xaxis,std::get<0>(d)));
}

template<class Filter>
void check(const T& sample_frequency,
	   const T& frequency,
	   const std::string& name)
{
  Filter f1(sample_frequency,   frequency);
  Filter f2(sample_frequency, 2*frequency);
  std::cout << name << ": " << f1.gain() << " " << f2.gain() << std::endl;
}

int main()
{
  const auto fc = filterbank11(fs,lf,hf).center_frequency();
  typedef std::tuple<std::vector<T>,const std::string> DataType;

  //////////////////////////
  //   Gain
  //////////////////////////

  // // gain of each filterbank type
  // std::vector<DataType> data_base;
  // data_base.push_back(make_tuple(filterbank11(fs,lf,hf).gain(), "convolution 0dB"));
  // data_base.push_back(make_tuple(filterbank12(fs,lf,hf).gain(), "convolution 6dB"));
  // data_base.push_back(make_tuple(filterbank31(fs,lf,hf).gain(), "cooke 0dB"));
  // data_base.push_back(make_tuple(filterbank32(fs,lf,hf).gain(), "cooke 6dB"));
  // data_base.push_back(make_tuple(filterbank21(fs,lf,hf).gain(), "slaney 0dB"));
  // data_base.push_back(make_tuple(filterbank22(fs,lf,hf).gain(), "slaney 6dB"));

  // // direct gain
  // plot(fc, data_base, "gain");

  // // normalized gain
  // for(auto& data:data_base)
  //   utils::decibel(std::get<0>(data).begin(),std::get<0>(data).end());
  // plot(fc, data_base, "normalized gain (dB)");


  //////////////////////////
  //   Impulse responses
  //////////////////////////

  // 3000 Hz IR
  using ir = gammatone::impulse_response;
  const T f = 3000;
  const auto t = ir::time(fs,0.01);
  std::vector<DataType> ir_base;
  ir_base.push_back(make_tuple(ir::implemented(filter11(fs,f),t.begin(),t.end()), "convolution 0dB"));
  ir_base.push_back(make_tuple(ir::implemented(filter12(fs,f),t.begin(),t.end()), "convolution 6dB"));
  ir_base.push_back(make_tuple(ir::implemented(filter31(fs,f),t.begin(),t.end()), "cooke 0dB"));
  ir_base.push_back(make_tuple(ir::implemented(filter32(fs,f),t.begin(),t.end()), "cooke 6dB"));
  ir_base.push_back(make_tuple(ir::implemented(filter21(fs,f),t.begin(),t.end()), "slaney 0dB"));
  ir_base.push_back(make_tuple(ir::implemented(filter22(fs,f),t.begin(),t.end()), "slaney 6dB"));

  plot(t, ir_base, "amplitude","time (s)");

  // // normalized IR
  // for(auto& ir:ir_base)
  //   utils::normalize(std::get<0>(ir).begin(),std::get<0>(ir).end());
  // plot(t, ir_base, "normalized amplitude","time (s)");


  //////////////////////////
  //   Check octave
  //////////////////////////

  std::vector<T> frequencies({500,1000,2000,3000,5000});
  for(const auto& f : frequencies)
    {
      check<filter11>(fs,f,"convolution 0dB");
      check<filter12>(fs,f,"convolution 6dB");
      check<filter21>(fs,f,"slaney 0dB");
      check<filter22>(fs,f,"slaney 6dB");
      check<filter31>(fs,f,"cooke 0dB");
      check<filter32>(fs,f,"cooke 6dB");
    }

  return 0;
}
