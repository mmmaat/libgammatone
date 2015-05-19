#include <gammatone/factory.hpp>

typedef double T;
const T fs = 44100;
const T fc = 2000;

int main()
{
  auto filter = gammatone::factory::filter<T>(fs,fc);
  
  return 0;
}
