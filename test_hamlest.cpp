// Copyright 2013 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "hamlest.hpp"

using lest::test;
using namespace lest::match;

int a() { return 33; }
int b() { return 55; }
int c() { return 77; }

char const *hello()       { return "hello"; }
char const *world()       { return "world"; }
char const *hello_world() { return "hello world"; }

const test specification[] =
{
    // object:
    
    "same_instance matches properly", []
    {
        int x{1}, y{2};
        EXPECT(  true == same_instance(x)(x) );
        EXPECT( false == same_instance(x)(y) );
    },

    // numeric:
    
    "close_to matches properly", []
    {
        EXPECT(  true == close_to(10, 2)( 8) );
        EXPECT(  true == close_to(10, 2)( 9) );
        EXPECT(  true == close_to(10, 2)(10) );
        EXPECT(  true == close_to(10, 2)(11) );
        EXPECT(  true == close_to(10, 2)(12) );

        EXPECT( false == close_to(10, 2)( 6) );
        EXPECT( false == close_to(10, 2)( 7) );
        EXPECT( false == close_to(10, 2)(13) );
        EXPECT( false == close_to(10, 2)(14) );
    },

    "equal_to matches properly", []
    {
        EXPECT(  true == equal_to(3)(3) );
        EXPECT( false == equal_to(3)(4) );
    },

    "not_equal_to matches properly", []
    {
        EXPECT(  true == not_equal_to(3)(4) );
        EXPECT( false == not_equal_to(3)(3) );
    },

    "less_than matches properly", []
    {
        EXPECT(  true == less_than(-3)(-4) );
        EXPECT(  true == less_than( 3)( 2) );
        
        EXPECT( false == less_than(-3)(-3) );
        EXPECT( false == less_than(-3)(-2) );
        EXPECT( false == less_than( 3)( 3) );
        EXPECT( false == less_than( 3)( 4) );
    },

    "less_equal matches properly", []
    {
        EXPECT(  true == less_equal(-3)(-4) );
        EXPECT(  true == less_equal(-3)(-3) );
        EXPECT(  true == less_equal( 3)( 2) );
        EXPECT(  true == less_equal( 3)( 3) );
        
        EXPECT( false == less_equal(-3)(-2) );
        EXPECT( false == less_equal( 3)( 4) );
    },

    "greater_than matches properly", []
    {
        EXPECT(  true == greater_than(-3)(-2) );
        EXPECT(  true == greater_than( 3)( 4) );
        
        EXPECT( false == greater_than(-3)(-3) );
        EXPECT( false == greater_than(-3)(-4) );
        EXPECT( false == greater_than( 3)( 3) );
        EXPECT( false == greater_than( 3)( 2) );
    },

    "greater_equal matches properly", []
    {
        EXPECT(  true == greater_equal(-3)(-3) );
        EXPECT(  true == greater_equal(-3)(-2) );
        EXPECT(  true == greater_equal( 3)( 3) );
        EXPECT(  true == greater_equal( 3)( 4) );
        
        EXPECT( false == greater_equal(-3)(-4) );
        EXPECT( false == greater_equal( 3)( 2) );
    },

    // textual:

    "starts_with matches properly", []
    {
        EXPECT(  true == starts_with( hello() )( hello_world() ) );
        EXPECT( false == starts_with( world() )( hello_world() ) );
    },

    "ends_with matches properly", []
    {
        EXPECT(  true == ends_with( world() )( hello_world() ) );
        EXPECT( false == ends_with( hello() )( hello_world() ) );
    },

    "contains matches properly", []
    {
        EXPECT(  true == contains( world() )( hello_world() ) );
        EXPECT(  true == contains( hello() )( hello_world() ) );

        EXPECT( false == contains( "hola" )( hello_world() ) );
    },

    "contains_regexp matches properly (fails for GNUC 4.8.1)", []
    {
        EXPECT(  true == contains_regexp( hello() )( hello_world() ) );
        EXPECT(  true == contains_regexp( "h.*o"  )( hello_world() ) );

        EXPECT( false == contains_regexp( "hola"  )( hello_world() ) );
    },

    "matches_regexp matches properly", []
    {
        EXPECT(  true == matches_regexp( hello_world() )( hello_world() ) );
        EXPECT(  true == matches_regexp( "h.*d"  )( hello_world() ) );

        EXPECT( false == matches_regexp( "hola" )( hello_world() ) );
    },

    // sequence:

    //using lest::contains;
    //using lest::contains_in_any_order;
    //using lest::has_items;

    // dictionary:

    //using lest::has_entries;
    //using lest::has_entry;
    //using lest::has_key;
    //using lest::has_value;

    // logical: 

    "anything matches properly", []
    {
        EXPECT(  true == anything<int        >( "[desciption]" )(  77  ) );
        EXPECT(  true == anything<char const*>( "[desciption]" )( "77" ) );
        EXPECT(  true == anything<std::string>( "[desciption]" )( "77" ) );
    },

    "is decorates properly", []
    {
        EXPECT(  true == is(77)(77) );
        EXPECT(  true == is( equal_to(77)(77) )(true) );
        EXPECT(  true == is( hello() )( hello() ) );
        EXPECT(  true == is( equal_to( hello() )( hello() ) )(true) );

        EXPECT( false == is(77)(33) );
        EXPECT( false == is( equal_to(77)(33) )(true) );
        EXPECT( false == is( hello() )( world() ) );
        EXPECT( false == is( equal_to( hello() )( world() ) )(true) );
    },

    "is_not decorates properly", []
    {
        EXPECT(  true == is_not(77)(33) );
        EXPECT(  true == is_not( equal_to(77)(33) )(true) );
        EXPECT(  true == is_not( hello() )( world() ) );
        EXPECT(  true == is_not( equal_to( hello() )( world() ) )(true) );

        EXPECT( false == is_not(77)(77) );
        EXPECT( false == is_not( equal_to(77)(77) )(true) );
        EXPECT( false == is_not( hello() )( hello() ) );
        EXPECT( false == is_not( equal_to( hello() )( hello() ) )(true) );
    },

    "all_of matches properly", []
    {
        EXPECT_THAT( a(), all_of( equal_to(a()), equal_to(a()) ) );
        EXPECT_THAT( a(), all_of( { a(), a() } ) );
        EXPECT_THAT( 'a', all_of( { 'a', 'a' } ) );

        test fail_1[] = {{ "F1", [] { EXPECT_THAT( a(), all_of( equal_to(a()), equal_to(b()) ) ); } }};
        test fail_2[] = {{ "F2", [] { EXPECT_THAT( a(), all_of( { a(), b() } ) ); } }};
        test fail_3[] = {{ "F3", [] { EXPECT_THAT( 'a', all_of( { 'a', 'b' } ) ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail_1, os ) );
        EXPECT( 1 == run( fail_2, os ) );
        EXPECT( 1 == run( fail_3, os ) );
    },

    "any_of matches properly", []
    {
        EXPECT_THAT( a(), any_of( equal_to(a()), equal_to(b()) ) );
        EXPECT_THAT( a(), any_of( { a(), b() } ) );
        EXPECT_THAT( 'a', any_of( { 'a', 'b' } ) );

        test fail_1[] = {{ "F1", [] { EXPECT_THAT( a(), any_of( equal_to(b()), equal_to(c()) ) ); } }};
        test fail_2[] = {{ "F2", [] { EXPECT_THAT( a(), any_of( { b(), b() } ) ); } }};
        test fail_3[] = {{ "F3", [] { EXPECT_THAT( 'a', any_of( { 'b', 'c' } ) ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail_1, os ) );
        EXPECT( 1 == run( fail_2, os ) );
        EXPECT( 1 == run( fail_3, os ) );
    },
};

int main()
{
    return lest::run( specification );
}

// cl -nologo -Wall -EHsc test_hamlest.cpp && test_hamlest
// g++ -Wall -Wextra -Weffc++ -std=c++11 -o test_hamlest.exe test_hamlest.cpp && test_hamlest

