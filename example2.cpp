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

using namespace lest::hamlest;

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
    CASE("compare int (fail)")
    {
        EXPECT( 33 == 22 );
    },

    CASE("compare array<int> (pass)")
    {
        EXPECT( m == m );
    },

    CASE("compare array<int> (fail)")
    {
        EXPECT( m == n );
    },

    CASE("compare set<int> (pass)")
    {
        EXPECT( s == s );
    },

    CASE("compare set<int> (fail)")
    {
        EXPECT( s == t );
    },

    CASE("compare vector<int> (pass)")
    {
        EXPECT( p == p );
    },

    CASE("compare vector<int> (fail)")
    {
        EXPECT( p == q );
    },

    CASE("equal_to vector<int> (pass)")
    {
        EXPECT_THAT( p, equal_to( p ) );
    },

    CASE("equal_to vector<int> (fail)")
    {
        EXPECT_THAT( p, equal_to( q ) );
    },

    CASE("equal_to vector<int>, {...} (fail)")
    {
//        EXPECT     ( p    ==      { 3, 3, 3, 3, }   );
//        EXPECT_THAT( p, equal_to( { 3, 3, 3, 3, } ) );
    },

    CASE("close_to int (pass)")
    {
        EXPECT_THAT( a() + 1, close_to( a(), 2 ) );
    },

    CASE("close_to int (fail)")
    {
        EXPECT_THAT( a() - 5, close_to( a(), 2 ) );
    },

    CASE("equal_to int (pass)")
    {
        EXPECT_THAT( a(), equal_to( a() ) );
    },

    CASE("equal_to int (fail)")
    {
        EXPECT_THAT( a(), equal_to( b() ) );
    },

    CASE("not_equal_to int (pass)")
    {
        EXPECT_THAT( a(), not_equal_to( b() ) );
    },

    CASE("not_equal_to int (fail)")
    {
        EXPECT_THAT( a(), not_equal_to( a() ) );
    },

    CASE("less_than int (pass)")
    {
        EXPECT_THAT( a(), less_than( b() ) );
    },

    CASE("less_than int (fail)")
    {
        EXPECT_THAT( a(), less_than( a() ) );
    },

    CASE("less_equal int (pass)")
    {
        EXPECT_THAT( a(), less_equal( b() ) );
    },

    CASE("less_equal int (fail)")
    {
        EXPECT_THAT( b(), less_equal( a() ) );
    },

    CASE("greater_than int (pass)")
    {
        EXPECT_THAT( b(), greater_than( a() ) );
    },

    CASE("greater_than int (fail)")
    {
        EXPECT_THAT( a(), greater_than( a() ) );
    },

    CASE("greater_equal int (pass)")
    {
        EXPECT_THAT( a(), greater_equal( a() ) );
    },

    CASE("greater_equal int (fail)")
    {
        EXPECT_THAT( a(), greater_equal( b() ) );
    },

    CASE("equal_to string (pass)")
    {
        EXPECT_THAT( hello(), equal_to( hello() ) );
    },

    CASE("equal_to string (fail)")
    {
        EXPECT_THAT( hello(), equal_to( world() ) );
    },

    CASE("starts_with (pass)")
    {
        EXPECT_THAT( hello(), starts_with( "hel" ) );
    },

    CASE("starts_with (fail)")
    {
        EXPECT_THAT( hello(), starts_with( "hex" ) );
    },

    CASE("ends_with (pass)")
    {
        EXPECT_THAT( hello(), ends_with( "lo" ) );
    },

    CASE("ends_with (fail)")
    {
        EXPECT_THAT( hello(), ends_with( "xo" ) );
    },

    CASE("contains (pass)")
    {
        EXPECT_THAT( hello(), contains( hello() ) );
    },

    CASE("contains (fail)")
    {
        EXPECT_THAT( hello(), contains( world() ) );
    },

    CASE("contains (fail)")
    {
        EXPECT_THAT( hello(), contains( "xx" ) );
    },

    CASE("contains_regexp (should pass; fails with GNUC 4.8.1)")
    {
        EXPECT_THAT( hello(), contains_regexp( "h.*o" ) );
    },

    CASE("contains_regexp (fail)")
    {
        EXPECT_THAT( hello(), contains_regexp( "x" ) );
    },

    CASE("matches_regexp (pass)")
    {
        EXPECT_THAT( hello(), matches_regexp( "h.*o" ) );
    },

    CASE("matches_regexp (fail)")
    {
        EXPECT_THAT( hello(), matches_regexp( ".*x" ) );
    },

    CASE("anything int (pass)")
    {
        EXPECT_THAT( a(), anything<int>( "anything goes" ) );
    },

    CASE("anything int (fail)")
    {
        EXPECT_THAT( a(), is_not( anything<int>( "anything goes" ) ) );
    },

    CASE("anything char const * (pass)")
    {
        EXPECT_THAT( world(), anything<char const*>( "anything goes" ) );
    },

    CASE("anything char const * (fail)")
    {
        EXPECT_THAT( world(), is_not( anything<char const*>( "anything goes" ) ) );
    },

    CASE("anything std::string (pass)")
    {
        EXPECT_THAT( world(), anything<std::string>( "anything goes" ) );
    },

    CASE("anything std::string (fail)")
    {
        EXPECT_THAT( world(), is_not( anything<std::string>( "anything goes" ) ) );
    },

    CASE("same_instance (pass)")
    {
        EXPECT_THAT( x, same_instance( x ) );
    },

    CASE("same_instance (fail)")
    {
        EXPECT_THAT( x, same_instance( y ) );
    },

    CASE("is equal_to int (pass)")
    {
        EXPECT_THAT( a(), is( equal_to( a() ) ) );
    },

    CASE("is equal_to int (fail)")
    {
        EXPECT_THAT( a(), is( equal_to( b() ) ) );
    },

    CASE("is int (pass)")
    {
        EXPECT_THAT( a(), is( a() ) );
    },

    CASE("is int (fail)")
    {
        EXPECT_THAT( a(), is( b() ) );
    },

    CASE("is double (pass)")
    {
        EXPECT_THAT( a(), is( 33.0) );
    },

    CASE("is double (fail)")
    {
        EXPECT_THAT( a(), is( 77.0) );
    },

    CASE("is string (pass)")
    {
        EXPECT_THAT( hello(), is( hello() ) );
    },

    CASE("is string (fail)")
    {
        EXPECT_THAT( hello(), is( world() ) );
    },

    CASE("is_not equal_to int (pass)")
    {
        EXPECT_THAT( a(), is_not( equal_to( b() ) ) );
    },

    CASE("is_not equal_to int (fail)")
    {
        EXPECT_THAT( a(), is_not( equal_to( a() ) ) );
    },

    CASE("is_not int (pass)")
    {
        EXPECT_THAT( a(), is_not( b() ) );
    },

    CASE("is_not int (fail)")
    {
        EXPECT_THAT( a(), is_not( a() ) );
    },

    CASE("is_not double (pass)")
    {
        EXPECT_THAT( a(), is_not( 77.0) );
    },

    CASE("is_not double (fail)")
    {
        EXPECT_THAT( a(), is_not( 33.0) );
    },

    CASE("is_not string (pass)")
    {
        EXPECT_THAT( hello(), is_not( world() ) );
    },

    CASE("is_not string (fail)")
    {
        EXPECT_THAT( hello(), is_not( hello() ) );
    },

    CASE("all_of int (pass)")
    {
        EXPECT_THAT( a(), all_of( equal_to(a()), equal_to(a()) ) );
    },

    CASE("all_of int (fail)")
    {
        EXPECT_THAT( a(), all_of( equal_to(a()), equal_to(b()) ) );
    },

    CASE("all_of int sequence (pass)")
    {
        EXPECT_THAT( a(), all_of( { a(), a() } ) );
    },

    CASE("all_of int sequence (fail)")
    {
        EXPECT_THAT( a(), all_of( { a(), b() } ) );
    },

    CASE("all_of char sequence (pass)")
    {
        EXPECT_THAT( 'a', all_of( { 'a', 'a' } ) );
    },

    CASE("all_of char sequence (fail)")
    {
        EXPECT_THAT( 'a', all_of( { 'a', 'b' } ) );
    },

    CASE("all_of contains (pass)")
    {
        EXPECT_THAT( hello(), all_of( starts_with("hel"), ends_with("lo"), contains("ll") ) );
    },

    CASE("all_of contains (fail)")
    {
        EXPECT_THAT( hello(), all_of( starts_with("hel"), ends_with("lo"), contains("xx") ) );
    },

    CASE("any_of int (pass)")
    {
        EXPECT_THAT( a(), any_of( equal_to(a()), equal_to(b()) ) );
    },

    CASE("any_of int (fail)")
    {
        EXPECT_THAT( a(), any_of( equal_to(b()), equal_to(c()) ) );
    },

    CASE("any_of int sequence (pass)")
    {
        EXPECT_THAT( a(), any_of( { a(), b() } ) );
    },

    CASE("any_of int sequence (fail)")
    {
        EXPECT_THAT( a(), any_of( { b(), b() } ) );
    },

    CASE("any_of char sequence (pass)")
    {
        EXPECT_THAT( 'a', any_of( { 'a', 'b' } ) );
    },

    CASE("any_of char sequence (fail)")
    {
        EXPECT_THAT( 'a', any_of( { 'b', 'c' } ) );
    },
};

int main()
{
    return lest::run( specification );
}

// g++ -Wall -Wextra -Weffc++ -std=c++11 -o example2.exe example2.cpp && example2.exe
