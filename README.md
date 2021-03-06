****

# libgammatone: a generic C++ library for gammatone filterbank

- Implementation of *An efficient implementation of the
  Patterson-Holdsworth auditory filter bank*, Slaney, 1993.


- A lot of open source implementations have been proposed over years,
  mainly in Matlab or python, few in C or C++.  Among these,
  **libgammatone** aims to be **standard C++ implementation**,
  suitable for research and engineering purpose.

    - Documented and tested
    - Clear and extensible interface
    - Fast and efficient


- Here a minimal working exemple

``` c++
    #include <gammatone/filter.hpp>
    #include <vector>

    int main()
    {
        // Init a gammatone filter sampled at 44.1 kHz, centered at 1 kHz
        gammatone::filter<double> filter(44100, 1000);

        // An input signal (here 1000 times zero, silly but minimal)
        std::vector<double> input(1000, 0.0);

        // Init an output buffer to store the filter response
        std::vector<double> output(1000);

        // Compute the output signal from input
        filter.compute_range(input.begin(), input.end(), output.begin());

        return 0;
    }
```

# Installation

Retrieve the code from github:

```
    git clone https://github.com/mmmaat/libgammatone.git
```

- After cloning, you have the libgammatone headers in
`./libgammatone/include`.
- You must have this dircetory in your include path in order to
compile your code with libgammatone.
- You must compile with the `-std=c++11` flag (tested on GCC 4.9.2).

# Hello World !

Have a look to `doc/exemple/src/minimal.cpp`.

This minimal exemple initializes a gammatone filter and process an
empty input signal.
Compile it with:
```
g++ -std=c++11 -o minimal minimal.cpp -I/path/to/libgammatone/include
```

It work ? Great, you are now ready to read the [tutorial](@ref tutorial) !


# Build documentation and tests

If you want to build documentation and tests you need the following programs:

- [cmake](http://www.cmake.org) to configure and compile,
- [gnuplot](http://www.gnuplot.info/) and
[gnuplot-iostream](http://www.stahlke.org/dan/gnuplot-iostream/) to
plot some tests output,
- [doxygen](http://www.stack.nl/~dimitri/doxygen/index.html) to build the documentation.
- [boost](http://www.boost.org) for the unit test framework

On a Debian/Ubuntu system, install it with:

```
sudo apt-get install libboost-dev doxygen gnuplot libgnuplot-iostream-dev cmake
```

You should place the build tree away from the source tree. For exemple
in `./libgammatone` create a `build` directory from where you run cmake:

```
mkdir ./build
cd ./build
cmake ..
```

Then type `make help` to see all the available targets.
Here are some interesting ones...

```
make html       # Generates the HTML documentation. Main page is doc/html/index.html
make tests      # Generates standalone tests in bin
make unit       # Generates a unit test executable bin/unit
make unit-all   # Generates a bigger unit test executable bin/unit-all
```


# Licence

### Copyright (C) 2015, 2016 Mathieu Bernard

libgammatone is free software: you can redistribute it and/or modify
it under the terms of the **GNU General Public License** as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

libgammatone is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details. You should have received a
copy of the GNU General Public License along with libgammatone. If
not, see <http://www.gnu.org/licenses/>.
