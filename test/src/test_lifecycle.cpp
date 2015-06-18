#include <gammatone/filter.hpp>
#include <gammatone/filterbank.hpp>
#include <iostream>
using namespace std;

// this function has been tested with success as a protected member of
// filter::concrete to assert lifecycle in the concrete as well
void write(const std::string msg)
{
  cout << msg << endl;
}

class filter : public gammatone::filter<double>
{
  using concrete = gammatone::filter<double>;
  
public:
  filter(const double& fs, const double& fc)
    : concrete::filter(fs,fc)
  { write("explicit"); }

  filter(const filter& other)
    : concrete::filter(other)
  { write("copy ctor"); }

  filter(filter&& other)
    : concrete::filter(std::move(other))
  { write("move ctor"); }

  virtual ~filter()
  { write("dtor");}
    
};

int main()
{
  const double fs = 20000, fc = 3000;
  
  filter f(fs,fc);
  filter f2(f);
  filter f3(std::move(f));
  
  return 0;
}
