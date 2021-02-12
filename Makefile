# Copyright 2013 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

CXXFLAGS   := -Wall -Wextra -Weffc++ -Wno-missing-braces -Wno-unused-parameter
CXXFLAGS03 := $(CXXFLAGS) -std=c++03
CXXFLAGS11 := $(CXXFLAGS) -std=c++11

all: test_hamlest example1 example2 

test_hamlest: test_hamlest.cpp lest.hpp hamlest.hpp
	$(CXX) $(CXXFLAGS11) -o test_hamlest test_hamlest.cpp
	./test_hamlest

example1: example1.cpp lest.hpp hamlest.hpp
	$(CXX) $(CXXFLAGS11) -o example1 example1.cpp
	-./example1

example2.exe: example2.cpp lest.hpp hamlest.hpp
	$(CXX) $(CXXFLAGS11) -o example2 example2.cpp
	-example2

clean:
	rm test_hamlest example1 example2

