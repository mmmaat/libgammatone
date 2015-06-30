Install  {#install}
=======

[TOC]

This section explains step by step how to have a working version of
libgammatone on your computer.


\section _deps Dependancies


Minimal dependancies to get libgammatone works are:
- [git](http://www.git-scm.com) intalled on your machine to clone the source from github,
- The [boost](http://www.boost.org) library (tested on version 1.55),
- A C++ compiler supporting the -std=c++14 option (tested on GCC 4.9.2).

On a Debian/Ubuntu system just type:
	
	sudo apt-get install git libboost-dev


\section _down Download


Retrieve the code from github:

	git clone https://github.com/mmmaat/libgammatone.git


\section _minimal Hello World !

This minimal exemple initializes a gammatone filter and
process an empty input signal.

\include minimal.cpp

This file is doc/exemple/src/minimal.cpp. Go to the libgammatone root directory:

	cd libgammatone

From there compile the minimal exemple using:

	g++ -std=c++14 -o minimal doc/exemple/src/minimal.cpp -Iinclude

It work ? Great, you are now ready to read the [tutorial](@ref tutorial) !


\section _build Build documentation and tests


If you want also to build documentation and tests you need the following:
- [cmake](http://www.cmake.org) to configure and compile,
- [gnuplot](http://www.gnuplot.info/) and
  [gnuplot-iostream](http://www.stahlke.org/dan/gnuplot-iostream/) to
  plot some tests output,
- [doxygen](http://www.stack.nl/~dimitri/doxygen/index.html) to build the documentation.

Install it with:

	sudo apt-get install cmake doxygen gnuplot libgnuplot-iostream-dev

You need cmake to configure the build and generate the Makefiles. Then
type `make help` to see all the available targets.

	cmake .


This command generates the HTML documentation. Main page is doc/html/index.html.

	make html


Compiles tests in test/bin that generally plot something on screen
using gnuplot. See sources for detail.

	make tests

Generates a unit test executable in test/bin/unit.

	make unit

Generates a bigger unit test executable in test/bin/unit-all.

	make unit-all


