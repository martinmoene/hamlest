// Copyright 2013 by Martin Moene
//
// hamlest example. hamlest is based on the C++ hamcrest variant by Jon Reid,
// see http://code.google.com/p/hamcrest/source/browse/trunk/hamcrest-c++?r=446
// Note: Hamcrest is being ported to GitHub https://github.com/hamcrest
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "hamlest.hpp"
#include <set>

using namespace lest::hamlest;

std::set<int> s{ 1, 2, 3, };
std::set<int> t{ 2, 1, 0, };

std::string hello() { return "hello C++11 world"; }

const lest::test specification[] =
{
    CASE("sets of int compare equal")
    {
        EXPECT( s == s );
        EXPECT( s == t );
    },

    CASE("sets of int match equal")
    {
        EXPECT_THAT( s, equal_to( s ) );
        EXPECT_THAT( s, equal_to( t ) );
    },

    CASE("string contains substring")
    {
        EXPECT_THAT( hello(), starts_with( "hello" ) );
        EXPECT_THAT( hello(), ends_with( "world" ) );
        EXPECT_THAT( hello(), contains( "C++11" ) );
        EXPECT_THAT( hello(), contains( "C++14" ) );
    },

    CASE("string matches at least one string")
    {
        EXPECT_THAT( "hello", any_of( { "hello", "C++", "world" } ) );
        EXPECT_THAT( "hello", any_of( { "foo"  , "C++", "bar"   } ) );
    },

    CASE("string satisfies all constraints")
    {
        EXPECT_THAT( hello(), all_of( starts_with("hello"), contains("C++11"), ends_with("world") ) );
        EXPECT_THAT( hello(), all_of( starts_with("hello"), contains("C++14"), ends_with("world") ) );
    },
};

int main()
{
    return lest::run( specification );
}

// g++ -Wall -Wextra -Weffc++ -std=c++11 -o example1.exe example1.cpp && example1.exe
