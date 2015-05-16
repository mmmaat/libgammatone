#include <gammatone/policy/channels.hpp>
#include <gammatone/filterbank.hpp>
//#include <utils/utils.hpp>
#include <utils/range.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

using namespace std;
typedef double T;

TEST( policy_bandwidth_test, silly_test )
{
  using namespace gammatone::policy::bandwidth;
  EXPECT_EQ(glasberg1990<T>::order, greenwood1990<T>::order);
  EXPECT_EQ(2, (int)slaney1988<T>::order);

  typedef glasberg1990<T> g;
  EXPECT_EQ(g::minbw*g::bw_correction, g::bandwidth(0));
}

TEST( policy_bandwidth_test, glasberg1990_works )
{
  using namespace gammatone::policy::bandwidth;

  // test on a lot of frequencies from 1Hz to 100kHz...  
  auto f = utils::linspace<T>(1,100000,10000);
  typedef glasberg1990<T> g;
  
  for_each(f.begin(),f.end(), [&](const auto& x)
           {ASSERT_NEAR(g::bandwidth(x),g::bw_correction*24.7*(4.37*x/1000 + 1),1e-2);});
  for_each(f.begin(),f.end(), [&](const auto& x)
           {ASSERT_DOUBLE_EQ(g::bandwidth(x),g::bw_correction*24.7*(4.3700009346995*x/1000 + 1));});
}
