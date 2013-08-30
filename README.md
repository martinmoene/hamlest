hamlest &ndash; matchers for lest
=================================

hamlest provides matchers for the tiny C++11 test helper lest [1]. hamlest's design is inspired on the C++ version of hamcrest by Jon Reid [2,3] and on ideas found in PyHamcrest [4].

Let writing tests become irresistibly easy and attractive.

**Contents**  
- [Example usage](#example-usage)
- [Compile and run](#compile-and-run)
- [Synopsis](#synopsis)
- [Reported to work with](#reported-to-work-with)
- [Notes and References](#notes-and-references)


Example usage
-------------

```Cpp
#include "hamlest.hpp"
#include <set>

using namespace lest::match;

std::set<int> s{ 1, 2, 3, };
std::set<int> t{ 2, 1, 0, };

std::string hello() { return "hello C++11 world"; }

const lest::test specification[] =
{
    "sets of int compare equal", []
    {
        EXPECT( s == s );
        EXPECT( s == t );
    },

    "sets of int match equal", []
    {
        EXPECT_THAT( s, equal_to( s ) );
        EXPECT_THAT( s, equal_to( t ) );
    },

    "string contains substring", []
    {
        EXPECT_THAT( hello(), starts_with( "hello" ) );
        EXPECT_THAT( hello(), ends_with( "world" ) );
        EXPECT_THAT( hello(), contains( "C++11" ) );
        EXPECT_THAT( hello(), contains( "C++14" ) );
    },

    "string matches at least one string", []
    {
        EXPECT_THAT( "hello", any_of( { "hello", "C++", "world" } ) );
        EXPECT_THAT( "hello", any_of( { "foo"  , "C++", "bar"   } ) );
    },

    "string satisfies all constraints", []
    {
        EXPECT_THAT( hello(), all_of( starts_with("hello"), contains("C++11"), ends_with("world") ) );
        EXPECT_THAT( hello(), all_of( starts_with("hello"), contains("C++14"), ends_with("world") ) );
    },
};

int main()
{
    return lest::run( specification );
}
```


Compile and run
---------------
For ease of use, the decompose version of lest is included in this repository.

```
prompt>g++ -Wall -Wextra -Weffc++ -std=c++11 -o example1.exe example1.cpp && example1.exe
example1.cpp:25: failed: sets of int compare equal: s == t for { 1, 2, 3, } == { 0, 1, 2, }
example1.cpp:31: failed: sets of int match equal: s equal_to( t ) for { 1, 2, 3, } is equal to { 0, 1, 2, }
example1.cpp:39: failed: string contains substring: hello() contains( "C++14" ) for "hello C++11 world" contains "C++14"
example1.cpp:45: failed: string matches at least one string: "hello" any_of( { "foo" , "C++", "bar" } ) \
                         for "hello" (is_equal to "foo" or is equal to "C++" or is equal to "bar")
example1.cpp:51: failed: string satisfies all constraints: hello() all_of( starts_with("hello"), contains("C++14"), ends_with("world") ) \
                         for "hello C++11 world" (starts with "hello" and contains "C++14" and ends with "world")
5 out of 5 tests failed.
```


Synopsis
--------

### Assertions Macros
**EXPECT_THAT(** _expr_, _matcher_ **)**  
Evaluate the expression and report failure. If an exception is thrown it is caught, reported and counted as a failure.

If an assertion fails, the remainder of the test that assertion is part of is skipped.

Note that EXPECT_THAT() is a shortened aliases for lest_EXPECT_THAT().

### Other Macros
**lest_NO_SHORT_ASSERTION_NAMES**  
Define this to omit the shortened alias macros for the lest_EXPECT... macros.

### Namespaces
namespace **lest**  
Types and functions are located in namespace lest.

namespace **lest::match**  
Matcher functions to pull in.

### Matchers

#### Object
**same_instance** - match same object  

#### Numeric
**close_to** -  match number within a range delta  
**equal_to** - match equal  
**not_equal_to** - match not equal  
**less_than** - match less than  
**less_equal** - match less than or equal to  
**greater_than** - match greater than  
**greater_equal** - match greater than or equal to  

#### Textual
**starts_with** - match the start of a string  
**ends_with** - match the end of a string  
**contains** -  match part of a string  
**contains_regexp** - match part of string (excluded from test: fails with Clang 3.2 and GNUC 4.8.1)  
**matches_regexp** - match whole string  

#### Sequence
No matchers yet.

#### Logical
**anything** - match anything; takes optional description  
**is** -  decorate matcher to improve readability, or match a value  
**is_not** - negate the matcher  
**all_of** - _and_ all matchers, or match all values in initialiser list  
**any_of** - _or_ all matchers, or match any value in initialiser list  


Reported to work with
---------------------

- g++ 4.8.1
- clang 3.2
- (for lest: Visual Studio 2013 preview)


Notes and References
--------------------

[1] [lest &ndash; lest errors escape testing](https://github.com/martinmoene/lest).

[2] [hamcrest](http://code.google.com/p/hamcrest/) on Google Code.  

[3] [hamcrest C++ by Jon Reid](http://code.google.com/p/hamcrest/source/browse/trunk/hamcrest-c%2B%2B/?r=446) on Google Code.  

[4] [PyHamcrest](https://github.com/hamcrest/PyHamcrest) on GitHub

[![Build Status](https://travis-ci.org/martinmoene/hamlest.png?branch=master)](https://travis-ci.org/martinmoene/hamlest)
