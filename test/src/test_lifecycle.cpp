#include <gammatone/filter/concrete.hpp>
#include <gammatone/filterbank/concrete.hpp>
#include <iostream>
using namespace std;

// this function has been tested with success as a protected member of
// filter::concrete to assert lifecycle in the concrete as well
void write(const std::string msg)
{
  cout << msg << endl;
}

class filter : public gammatone::filter::concrete<double>
{
  using concrete = gammatone::filter::concrete<double>;
  
public:
  filter(const double& fs, const double& fc)
    : concrete::concrete(fs,fc)
  { write("explicit"); }

  filter(const filter& other)
    : concrete::concrete(other)
  { write("copy ctor"); }

  filter(filter&& other)
    : concrete::concrete(std::move(other))
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
