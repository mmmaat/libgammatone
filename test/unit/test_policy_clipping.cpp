#include <gammatone/policy/clipping.hpp>
#include <vector>
#include <algorithm>
#include <gtest/gtest.h>

typedef double T;
using namespace gammatone::policy::clipping;


TEST(policy_clipping, complex_works)
{
  std::vector<std::complex<T> > r = {-1e-201,1e-200-1, 0, 1e-250,1e-100,1,1e100};
  std::vector<std::complex<T> > a = {      0,1e-200-1, 0,      0,1e-100,1,1e100};

  std::vector<std::complex<T> > c1(r.size()), c2(r.size());
  std::transform(r.begin(),r.end(),a.begin(),c1.begin(),
                 [&](auto& x,auto& y){return on::clip(x);});

  std::transform(r.begin(),r.end(),c2.begin(),
                 [&](auto& x){return off::clip(x);});

  for(std::size_t i=0;i<r.size();i++)
    {
      EXPECT_DOUBLE_EQ( c1[i].real(), a[i].real() );
      EXPECT_DOUBLE_EQ( c1[i].imag(), a[i].imag() );

      EXPECT_DOUBLE_EQ( c2[i].real(), r[i].real() );
      EXPECT_DOUBLE_EQ( c2[i].imag(), r[i].imag() );

    }
}
