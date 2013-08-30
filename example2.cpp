// Copyright 2013 by Martin Moene
//
// hamlest example. hamlest is based on the C++ hamcrest variant by Jon Reid,
// see http://code.google.com/p/hamcrest/source/browse/trunk/hamcrest-c++?r=446
// Note: Hamcrest is being ported to GitHub https://github.com/hamcrest
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "hamlest.hpp"

#include <array>
#include <iostream>
#include <set>
#include <vector>

using namespace lest::match;

int a() { return 33; }
int b() { return 55; }
int c() { return 77; }

char const *hello() { return "hello"; }
char const *world() { return "world"; }

int x{11}, y{22};

std::array<int, 3> m{ 1, 2, 3, };
std::array<int, 3> n{ 3, 2, 1, };

std::vector<int> p{ 1, 2, 3, 4, };
std::vector<int> q{ 4, 3, 2, 1, };

std::set<int> s{ 1, 2, 3, };
std::set<int> t{ 2, 1, 0, };


const lest::test specification[] =
{
    "compare int (fail)", []
    {
        EXPECT( 33 == 22 );
    },

    "compare array<int> (pass)", []
    {
        EXPECT( m == m );
    },

    "compare array<int> (fail)", []
    {
        EXPECT( m == n );
    },

    "compare set<int> (pass)", []
    {
        EXPECT( s == s );
    },

    "compare set<int> (fail)", []
    {
        EXPECT( s == t );
    },

    "compare vector<int> (pass)", []
    {
        EXPECT( p == p );
    },

    "compare vector<int> (fail)", []
    {
        EXPECT( p == q );
    },

    "equal_to vector<int> (pass)", []
    {
        EXPECT_THAT( p, equal_to( p ) );
    },

    "equal_to vector<int> (fail)", []
    {
        EXPECT_THAT( p, equal_to( q ) );
    },

    "equal_to vector<int>, {...} (fail)", []
    {
//        EXPECT     ( p    ==      { 3, 3, 3, 3, }   );
//        EXPECT_THAT( p, equal_to( { 3, 3, 3, 3, } ) );
    },

    "close_to int (pass)", []
    {
        EXPECT_THAT( a() + 1, close_to( a(), 2 ) );
    },

    "close_to int (fail)", []
    {
        EXPECT_THAT( a() - 5, close_to( a(), 2 ) );
    },

    "equal_to int (pass)", []
    {
        EXPECT_THAT( a(), equal_to( a() ) );
    },

    "equal_to int (fail)", []
    {
        EXPECT_THAT( a(), equal_to( b() ) );
    },

    "not_equal_to int (pass)", []
    {
        EXPECT_THAT( a(), not_equal_to( b() ) );
    },

    "not_equal_to int (fail)", []
    {
        EXPECT_THAT( a(), not_equal_to( a() ) );
    },

    "less_than int (pass)", []
    {
        EXPECT_THAT( a(), less_than( b() ) );
    },

    "less_than int (fail)", []
    {
        EXPECT_THAT( a(), less_than( a() ) );
    },

    "less_equal int (pass)", []
    {
        EXPECT_THAT( a(), less_equal( b() ) );
    },

    "less_equal int (fail)", []
    {
        EXPECT_THAT( b(), less_equal( a() ) );
    },

    "greater_than int (pass)", []
    {
        EXPECT_THAT( b(), greater_than( a() ) );
    },

    "greater_than int (fail)", []
    {
        EXPECT_THAT( a(), greater_than( a() ) );
    },

    "greater_equal int (pass)", []
    {
        EXPECT_THAT( a(), greater_equal( a() ) );
    },

    "greater_equal int (fail)", []
    {
        EXPECT_THAT( a(), greater_equal( b() ) );
    },

    "equal_to string (pass)", []
    {
        EXPECT_THAT( hello(), equal_to( hello() ) );
    },

    "equal_to string (fail)", []
    {
        EXPECT_THAT( hello(), equal_to( world() ) );
    },

    "starts_with (pass)", []
    {
        EXPECT_THAT( hello(), starts_with( "hel" ) );
    },

    "starts_with (fail)", []
    {
        EXPECT_THAT( hello(), starts_with( "hex" ) );
    },

    "ends_with (pass)", []
    {
        EXPECT_THAT( hello(), ends_with( "lo" ) );
    },

    "ends_with (fail)", []
    {
        EXPECT_THAT( hello(), ends_with( "xo" ) );
    },

    "contains (pass)", []
    {
        EXPECT_THAT( hello(), contains( hello() ) );
    },

    "contains (fail)", []
    {
        EXPECT_THAT( hello(), contains( world() ) );
    },

    "contains (fail)", []
    {
        EXPECT_THAT( hello(), contains( "xx" ) );
    },

    "contains_regexp (should pass; fails with GNUC 4.8.1)", []
    {
        EXPECT_THAT( hello(), contains_regexp( "h.*o" ) );
    },

    "contains_regexp (fail)", []
    {
        EXPECT_THAT( hello(), contains_regexp( "x" ) );
    },

    "matches_regexp (pass)", []
    {
        EXPECT_THAT( hello(), matches_regexp( "h.*o" ) );
    },

    "matches_regexp (fail)", []
    {
        EXPECT_THAT( hello(), matches_regexp( ".*x" ) );
    },

    "anything int (pass)", []
    {
        EXPECT_THAT( a(), anything<int>( "anything goes" ) );
    },

    "anything int (fail)", []
    {
        EXPECT_THAT( a(), is_not( anything<int>( "anything goes" ) ) );
    },

    "anything char const * (pass)", []
    {
        EXPECT_THAT( world(), anything<char const*>( "anything goes" ) );
    },

    "anything char const * (fail)", []
    {
        EXPECT_THAT( world(), is_not( anything<char const*>( "anything goes" ) ) );
    },

    "anything std::string (pass)", []
    {
        EXPECT_THAT( world(), anything<std::string>( "anything goes" ) );
    },

    "anything std::string (fail)", []
    {
        EXPECT_THAT( world(), is_not( anything<std::string>( "anything goes" ) ) );
    },

    "same_instance (pass)", []
    {
        EXPECT_THAT( x, same_instance( x ) );
    },

    "same_instance (fail)", []
    {
        EXPECT_THAT( x, same_instance( y ) );
    },

    "is equal_to int (pass)", []
    {
        EXPECT_THAT( a(), is( equal_to( a() ) ) );
    },

    "is equal_to int (fail)", []
    {
        EXPECT_THAT( a(), is( equal_to( b() ) ) );
    },

    "is int (pass)", []
    {
        EXPECT_THAT( a(), is( a() ) );
    },

    "is int (fail)", []
    {
        EXPECT_THAT( a(), is( b() ) );
    },

    "is double (pass)", []
    {
        EXPECT_THAT( a(), is( 33.0) );
    },

    "is double (fail)", []
    {
        EXPECT_THAT( a(), is( 77.0) );
    },

    "is string (pass)", []
    {
        EXPECT_THAT( hello(), is( hello() ) );
    },

    "is string (fail)", []
    {
        EXPECT_THAT( hello(), is( world() ) );
    },

    "is_not equal_to int (pass)", []
    {
        EXPECT_THAT( a(), is_not( equal_to( b() ) ) );
    },

    "is_not equal_to int (fail)", []
    {
        EXPECT_THAT( a(), is_not( equal_to( a() ) ) );
    },

    "is_not int (pass)", []
    {
        EXPECT_THAT( a(), is_not( b() ) );
    },

    "is_not int (fail)", []
    {
        EXPECT_THAT( a(), is_not( a() ) );
    },

    "is_not double (pass)", []
    {
        EXPECT_THAT( a(), is_not( 77.0) );
    },

    "is_not double (fail)", []
    {
        EXPECT_THAT( a(), is_not( 33.0) );
    },

    "is_not string (pass)", []
    {
        EXPECT_THAT( hello(), is_not( world() ) );
    },

    "is_not string (fail)", []
    {
        EXPECT_THAT( hello(), is_not( hello() ) );
    },

    "all_of int (pass)", []
    {
        EXPECT_THAT( a(), all_of( equal_to(a()), equal_to(a()) ) );
    },

    "all_of int (fail)", []
    {
        EXPECT_THAT( a(), all_of( equal_to(a()), equal_to(b()) ) );
    },

    "all_of int sequence (pass)", []
    {
        EXPECT_THAT( a(), all_of( { a(), a() } ) );
    },

    "all_of int sequence (fail)", []
    {
        EXPECT_THAT( a(), all_of( { a(), b() } ) );
    },

    "all_of char sequence (pass)", []
    {
        EXPECT_THAT( 'a', all_of( { 'a', 'a' } ) );
    },

    "all_of char sequence (fail)", []
    {
        EXPECT_THAT( 'a', all_of( { 'a', 'b' } ) );
    },

    "all_of contains (pass)", []
    {
        EXPECT_THAT( hello(), all_of( starts_with("hel"), ends_with("lo"), contains("ll") ) );
    },

    "all_of contains (fail)", []
    {
        EXPECT_THAT( hello(), all_of( starts_with("hel"), ends_with("lo"), contains("xx") ) );
    },

    "any_of int (pass)", []
    {
        EXPECT_THAT( a(), any_of( equal_to(a()), equal_to(b()) ) );
    },

    "any_of int (fail)", []
    {
        EXPECT_THAT( a(), any_of( equal_to(b()), equal_to(c()) ) );
    },

    "any_of int sequence (pass)", []
    {
        EXPECT_THAT( a(), any_of( { a(), b() } ) );
    },

    "any_of int sequence (fail)", []
    {
        EXPECT_THAT( a(), any_of( { b(), b() } ) );
    },

    "any_of char sequence (pass)", []
    {
        EXPECT_THAT( 'a', any_of( { 'a', 'b' } ) );
    },

    "any_of char sequence (fail)", []
    {
        EXPECT_THAT( 'a', any_of( { 'b', 'c' } ) );
    },
};

int main()
{
    return lest::run( specification );
}

// g++ -Wall -Wextra -Weffc++ -std=c++11 -o example2.exe example2.cpp && example2.exe
