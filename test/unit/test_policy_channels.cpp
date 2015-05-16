#include <gammatone/policy/channels.hpp>
#include <gammatone/filterbank.hpp>
//#include <utils/utils.hpp>
#include <utils/range.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iterator>

using namespace std;
typedef double T;

TEST(policy_channels_test, silly_case_works)
{
  vector<T> v;
  using namespace gammatone::policy::order;
  EXPECT_EQ(increasing::begin(v),increasing::end(v) );
  EXPECT_EQ(decreasing::begin(v),decreasing::end(v) );
}

TEST(policy_channels_test, iteration_works)
{
  const size_t n = 10;
  const auto v = utils::linspace<T>(1.0,2.0,n);

  using namespace gammatone::policy::order;
  auto incr_begin = increasing::begin(v);
  auto incr_end   = increasing::end(v);
  auto decr_begin = decreasing::begin(v);
  auto decr_end   = decreasing::end(v);

  EXPECT_EQ( (int)n, distance(incr_begin,incr_end) );
  EXPECT_EQ( (int)n, distance(decr_begin,decr_end) );
  
  for(size_t i=0;i<n;i++)
    {
      EXPECT_DOUBLE_EQ( *incr_begin++, *--decr_end );
      EXPECT_DOUBLE_EQ( *decr_begin++, *--incr_end );
    }
}

TEST(policy_channels_test, filterbank_works)
{
  namespace gpc = gammatone::policy::channels;
  namespace gpo = gammatone::policy::order;
  typedef gammatone::core::cooke1993<T> core;
  typedef gammatone::policy::bandwidth::glasberg1990<T> bandwidth;
  typedef gammatone::filterbank<T,core,bandwidth,gpc::fixed_size<T,gpo::increasing> > increasing_fb;
  typedef gammatone::filterbank<T,core,bandwidth,gpc::fixed_size<T,gpo::decreasing> > decreasing_fb;

  const T fs = 44100, lf = 500, hf = 8000, nbc = 10;
  increasing_fb fi(fs,lf,hf,nbc);
  decreasing_fb fd(fs,lf,hf,nbc);

  const auto cfi = fi.center_frequency();
  const auto cfd = fd.center_frequency();

  auto i = fi.begin();
  auto d = fd.end(); d--;
  for(; i!=fi.end(); i++, d--)
    EXPECT_DOUBLE_EQ(i->center_frequency(),d->center_frequency());
}
