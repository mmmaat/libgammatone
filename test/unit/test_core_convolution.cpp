#include <gammatone/core/convolution.hpp>
#include <gammatone/policy/bandwidth.hpp>
#include <gtest/gtest.h>

#include <iostream>
using std::cout;
using std::endl;

using namespace gammatone::core;
typedef double T;

// class core_convolution_test : public ::testing::Test
// {
// protected:

// };

class my_convolution : public convolution<T>
{
public:

  my_convolution(const T& fs, const T& fc)
    : convolution<T>(fs,fc,gammatone::policy::bandwidth::glasberg1990<T>::bandwidth(fc))
  {}

  virtual ~my_convolution()
  {}

  const std::vector<T>& ir() const
  {
    return m_ir;
  }

};

TEST(core_convolution_test,ctor_works)
{
  my_convolution core(44100,1000);
  cout << "gain = " << core.gain() << endl
       << "size = " << core.ir().size() << endl;

  //  cout << core.m_time.size() << " " << core.m_ir.size() << endl;

}
