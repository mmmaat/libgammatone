#include <filter_types.h>
#include <utils/utils.hpp>
using namespace gammatone;


template<class Filter>
class filter_test : public testing::Test
{
protected:
  filter_test() : signal(utils::random(-1.0,1.0,10000))
  {
    std::for_each(cf.begin(),cf.end(),[&](const auto& x){filters.push_back(Filter(fs,x));});
  };

  const T fs = 44100;
  const std::vector<T> cf = {1,10,100,451,2351,6842,12504,15478,fs/2,fs};
  std::vector<Filter> filters;
  std::vector<T> signal;
};

TYPED_TEST_CASE(filter_test, filter_types);

TYPED_TEST( filter_test, copy_works )
{
  const auto& x = this->signal;
  for(auto& f:this->filters)
    {
      TypeParam f1(f);
      EXPECT_DOUBLE_EQ( f1.sample_frequency(), f.sample_frequency() );
      EXPECT_DOUBLE_EQ( f1.center_frequency(), f.center_frequency() );
      EXPECT_DOUBLE_EQ( f1.bandwidth(), f.bandwidth() );
      EXPECT_DOUBLE_EQ( f1.gain(), f.gain() );

      TypeParam f2 = f;
      EXPECT_DOUBLE_EQ( f2.sample_frequency(), f.sample_frequency() );
      EXPECT_DOUBLE_EQ( f2.center_frequency(), f.center_frequency() );
      EXPECT_DOUBLE_EQ( f2.bandwidth(), f.bandwidth() );
      EXPECT_DOUBLE_EQ( f2.gain(), f.gain() );

      auto out = f.compute(x);
      auto out1 = f1.compute(x);
      auto out2 = f2.compute(x);
      for(size_t i=0;i<x.size();i++)
        {
          EXPECT_DOUBLE_EQ(out[i],out1[i]);
          EXPECT_DOUBLE_EQ(out[i],out2[i]);
        }
    }
}

TYPED_TEST( filter_test, compute_works )
{
  const auto& x = this->signal;
  for(auto& f:this->filters)
    {
      f.reset();
      std::vector<T> c1 = f.compute(x);

      f.reset();
      std::vector<T> c2(x.size());
      f.compute(x.begin(),x.end(),c2.begin());

      f.reset();
      std::vector<T> c3(x.size());
      std::transform(x.begin(),x.end(),c3.begin(),[&](const T& xx){return f.compute(xx);});

      for(std::size_t i=0;i<x.size();i++)
        {
          EXPECT_DOUBLE_EQ(c1[i],c2[i]);
          EXPECT_DOUBLE_EQ(c1[i],c3[i]);
        }
    }
}

TYPED_TEST( filter_test, reset_works )
{
  const auto& x = this->signal;
  for(auto& f:this->filters)
    {
      auto c = f.compute(x);

      auto c1 = f.compute(x);
      std::vector<T> d1(x.size());
      std::transform(c.begin(),c.end(),c1.begin(),d1.begin(),
                     [&](const T& x,const T& y){return x-y;});
      bool b1 = std::all_of(d1.begin(),d1.end(),[](const T& x){return x==0;});
      EXPECT_EQ(false,b1);

      f.reset();
      auto c2 = f.compute(x);
      std::vector<T> d2(x.size());
      std::transform(c.begin(),c.end(),c2.begin(),d2.begin(),
                     [&](const T& x,const T& y){return x-y;});
      bool b2 = std::all_of(d2.begin(),d2.end(),[](const T& x){return x==0;});
      EXPECT_EQ(true,b2);
    }
}

TYPED_TEST( filter_test, accessors_works )
{
  size_t i=0;
  for(auto& f:this->filters)
    {
      EXPECT_EQ( this->fs, f.sample_frequency() );
      EXPECT_EQ( this->cf[i++], f.center_frequency() );
      EXPECT_GT( f.bandwidth(), 0.0 );
      EXPECT_GT( f.gain(), 0.0 );
    }
}
