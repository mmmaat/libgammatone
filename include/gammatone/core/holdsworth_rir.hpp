#ifndef GAMMATONE_CORE_HOLDSWORTH_RIR_HPP
#define GAMMATONE_CORE_HOLDSWORTH_RIR_HPP

#include <complex>
#include <queue>

#include <iostream>

namespace gammatone
{
  namespace core
  {
    template<class T>
    class holdsworth_rir
    {
    public:
      typedef T scalar_type;
      
      void init(const T& sample_frequency, const T& center_frequency, const T& bandwidth)
      {
        alpha = find_alpha(sample_frequency,bandwidth);
        omega = find_omega(sample_frequency,center_frequency);
	period = 1.0 / sample_frequency;
        reset();
      }

      virtual ~holdsworth_rir(){}

      void reset()
      {
        t = 0;
	while(!z.empty()) z.pop(); z.push(0); z.push(0);
	while(!w.empty()) w.pop(); w.push(0); w.push(0);
      }

      virtual inline T compute(const T& x)
      {
	std::cout << std::exp(-omega*t) << "  " << x << "  " << std::exp(-omega*t) * x << std::endl;
	
        z.push( std::exp(-omega*t) * x ); z.pop();
        w.push( w.front() + alpha*w.front()*z.front()); w.pop();
	auto y = std::real( std::exp(omega*t) * w.back() );
	t += period;
	return y;
      }

    protected:
      T period, t;
      T alpha;
      std::complex<T> omega;
      std::queue<std::complex<T> > z, w;

      inline T find_alpha(const T& fs, const T& bw)
      {
        return 1 - std::exp(-2*boost::math::constants::pi<T>()*bw/fs);
      }

      inline std::complex<T> find_omega(const T& fs, const T& fc)
      {
	const auto theta = 2*boost::math::constants::pi<T>()*fc/fs;
        return std::complex<T>(std::cos(theta),std::sin(theta));
      }
    };
  }
}

#endif // GAMMATONE_CORE_HOLDSWORTH_RIR_HPP
