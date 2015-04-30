#include <gammatone/policy/channels.hpp>
#include <utils.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iterator>
using namespace std;

typedef double T;

TEST( policy_test, channels_works )
{
  const size_t n = 10;
  const auto v = utils::linspace<T>(1.0,2.0,n);

  using namespace gammatone::policy::channels;
  auto incr_begin = increasing::begin(v);
  auto incr_end   = increasing::end(v);
  auto decr_begin = decreasing::begin(v);
  auto decr_end   = decreasing::end(v);

  EXPECT_EQ( n, distance(incr_begin,incr_end) );
  EXPECT_EQ( n, distance(decr_begin,decr_end) );
  
  for(size_t i=0;i<n;i++)
    {
      EXPECT_DOUBLE_EQ( *incr_begin++, *--decr_end );
      EXPECT_DOUBLE_EQ( *decr_begin++, *--incr_end );
    }
}
