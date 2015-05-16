#include <filterbank_types.h>
#include <utils/utils.hpp>
using namespace gammatone;


typedef double T;

template<class Filterbank>
class filterbank_test : public testing::Test
{
protected:

  const T m_sample_frequency = 44100;
  const T m_low = 500, m_high = 8000;
};

TYPED_TEST_CASE(filterbank_test, filterbank_types);

TYPED_TEST( filterbank_test, accessors_works )
{
  using namespace gammatone;
  TypeParam m_filterbank( this->m_sample_frequency,this->m_low,this->m_high );

  EXPECT_EQ( this->m_sample_frequency, m_filterbank.sample_frequency() );

  size_t i=0;
  for(const auto& f:m_filterbank)
    {
      EXPECT_GT( f.bandwidth(), 0.0 );
      EXPECT_GT( f.gain(), 0.0 );

      EXPECT_EQ( m_filterbank.center_frequency()[i], f.center_frequency() );
      EXPECT_EQ( m_filterbank.bandwidth()[i], f.bandwidth() );
      EXPECT_EQ( m_filterbank.gain()[i], f.gain() );
      i++;
    }
}


TEST( filterbank_test, center_frequency_works )
{
  const T m_sample_frequency = 44100;
  //  const T m_nb_channels = 20;
  const T m_low = 500, m_high = 8000;

  using namespace gammatone;
  typedef core::cooke1993<T> core;
  typedef policy::bandwidth::glasberg1990<T> bandwidth;
  typedef filterbank<T,core,bandwidth,policy::channels::fixed_size<T,policy::order::increasing> > increasing;
  typedef filterbank<T,core,bandwidth,policy::channels::fixed_size<T,policy::order::decreasing> > decreasing;

  increasing fi(m_sample_frequency,m_low,m_high);
  EXPECT_LE( m_low, fi.begin()->center_frequency() );
  EXPECT_EQ( m_high, fi.rbegin()->center_frequency() );

  decreasing fd(m_sample_frequency,m_low,m_high);
  EXPECT_EQ( m_high, fd.begin()->center_frequency() );
  EXPECT_LE( m_low, fd.rbegin()->center_frequency() );

  auto it=fi.begin(); auto it2=fd.rbegin();
  while(it!=fi.end())
    EXPECT_EQ(it++->center_frequency(), it2++->center_frequency());

}
