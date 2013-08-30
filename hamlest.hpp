// Copyright 2013 by Martin Moene
//
// hamlest example. hamlest is based on the C++ hamcrest variant by Jon Reid,
// see http://code.google.com/p/hamcrest/source/browse/trunk/hamcrest-c++?r=446
// Note: Hamcrest is being ported to GitHub https://github.com/hamcrest
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEST_HAMLEST_H_INCLUDED
#define LEST_HAMLEST_H_INCLUDED

#include "lest.hpp"

#include <memory>
#include <regex>
#include <vector>

#ifndef lest_NO_SHORT_ASSERTION_NAMES
# define EXPECT_THAT  lest_EXPECT_THAT
#endif

#define lest_EXPECT_THAT( actual, match ) \
    try \
    { \
        if ( ! match( actual ) ) \
            throw lest::failure{ lest_LOCATION, \
                lest::matching( #actual, #match ), \
                lest::matching( lest::to_string(actual), match.to_string() ) }; \
    } \
    catch( lest::failure const & ) \
    { \
        throw ; \
    } \
    catch( std::exception const & e ) \
    { \
        throw lest::unexpected{ lest_LOCATION, lest::matching( #actual, #match ), lest::with_message( e.what() ) }; \
    } \
    catch(...) \
    { \
        throw lest::unexpected{ lest_LOCATION, lest::matching( #actual, #match ), "of unknown type" }; \
    }

namespace lest {

inline std::string matching( std::string actual, std::string match )
{
    return actual + " " + match;
}

template <typename T>
struct matcher;

template<typename T>
using Matcher = std::shared_ptr<matcher<T>>;

/**
 * matcher for values of type T.
 */
template <typename T>
struct matcher // : public std::unary_function<T, bool> // has non-virtual destructor
{
    using argument_type = T;
    
    virtual ~matcher() { }

    /// matcher copy
    virtual Matcher<T> copy() const = 0;

    /// true if matches
    virtual bool operator()( T const & actual ) const = 0;

    /// matcher description using expectation values.
    virtual std::string to_string() const = 0;
};

struct not_matcher_tag { };
struct matcher_tag { };

/**
 * matcher traits for argument type and matcher-value distinction.
 */
template <typename T>
struct matcher_traits
{
    using argument = T;
    using category = not_matcher_tag;
};

#define LEST_TAG_MATCHER(matcher_type)          \
    template <typename T>                       \
    struct matcher_traits< matcher_type<T> >    \
    {                                           \
        using argument = T;                     \
        using category = matcher_tag;           \
    };

/**
 * comparison( value )
 */
template <typename T>
struct comparison_matcher : matcher<T>
{
    comparison_matcher( std::string text, T const & expected ) : text( text ), expected( expected ) { }

    // accept copying of pointers as is, e.g. char *:
    comparison_matcher( comparison_matcher<T> const & ) = default;
    comparison_matcher<T>& operator=( comparison_matcher<T> const & ) = default;

    std::string to_string() const { return text + " " + lest::to_string( expected ); }

    const std::string text;
    const T expected;
};

#define LEST_COMPARISON_MATCHER( op, matcher_name, text ) \
    template <typename T> \
    struct matcher_name : comparison_matcher<T> \
    {   \
        matcher_name( T const & expected ) : comparison_matcher<T>( text, expected ) { } \
        \
        Matcher<T> copy() const { return Matcher<T>( new matcher_name( *this ) ); } \
        \
        bool operator()( T const & actual ) const { return actual op this->expected; } \
    };  \
        \
    LEST_TAG_MATCHER( matcher_name )

LEST_COMPARISON_MATCHER( ==, equal_to_matcher     , "is equal to" );
LEST_COMPARISON_MATCHER( !=, not_equal_to_matcher , "is not equal to" );

LEST_COMPARISON_MATCHER( < , less_than_matcher    , "is less than" );
LEST_COMPARISON_MATCHER( <=, less_equal_matcher   , "is less or equal" );

LEST_COMPARISON_MATCHER( > , greater_than_matcher , "is greater than" );
LEST_COMPARISON_MATCHER( >=, greater_equal_matcher, "is greater or equal" );

/**
 * close_to( expected, delta ): match number within delta of expected value.
 */
template <typename T>
struct close_to_matcher : matcher<T>
{
    explicit close_to_matcher( T const & expected, T const & delta ) : expected(expected), delta(delta) { }

    Matcher<T> copy() const { return Matcher<T>( new close_to_matcher(*this) ); }

    bool operator()( T const & actual ) const { return std::abs( expected - actual ) <= delta; }

    std::string to_string() const { return "is within " + lest::to_string( expected ) + " +/- " + lest::to_string( delta ); }

    const T expected;
    const T delta;
};

/**
 * anything( [description] ): match anything of type T.
 */
template <typename T>
struct anything : matcher<T>
{
    explicit anything( std::string description = "[anything]" ) : description( description ) { }

    Matcher<T> copy() const { return Matcher<T>( new anything(*this) ); }

    bool operator()( T const & /*actual*/ ) const { return true; }

    std::string to_string() const { return lest::to_string( description ); }

    const std::string description;
};

/**
 * same_instance( item )
 */
template <typename T>
struct same_instance_matcher : matcher<T>
{
    same_instance_matcher( T const & instance ) : instance( instance ) { }

    Matcher<T> copy() const { return Matcher<T>( new same_instance_matcher(*this) ); }

    bool operator()( T const & actual ) const { return &actual == &instance; }

    std::string to_string() const { return "same (" + lest::to_string( instance ) + ")"; }

    const T & instance;
};

/**
 * starts_with( text )
 */
struct starts_with_matcher : public matcher<std::string>
{
    explicit starts_with_matcher( std::string const & text ) : text( text ) { }

    Matcher<std::string> copy() const { return Matcher<std::string>( new starts_with_matcher( *this ) ); }

    bool operator()( std::string const & actual ) const { return 0 == actual.find( text ); }

    std::string to_string() const { return "starts with " + lest::to_string( text ); }

    const std::string text;
};

/**
 * ends_with( text )
 */
struct ends_with_matcher : public matcher<std::string>
{
    explicit ends_with_matcher( std::string const & text ) : text(text) { }

    Matcher<std::string> copy() const { return Matcher<std::string>( new ends_with_matcher( *this ) ); }

    bool operator()( std::string const & actual ) const { return actual.find( text ) == actual.size() - text.size(); }

    std::string to_string() const { return "ends with " + lest::to_string( text ); }

    const std::string text;
};

/**
 * contains( text ).
 */
struct contains_matcher : public matcher<std::string>
{
    explicit contains_matcher( std::string const & text ) : text( text ) { }

    Matcher<std::string> copy() const { return Matcher<std::string>( new contains_matcher( *this ) ); }

    bool operator()(std::string const & actual) const { return std::string::npos != actual.find( text ); }

    std::string to_string() const { return "contains " + lest::to_string( text ); }

    const std::string text;
};

/**
 * contains_regexp( expr )
 */
struct contains_regexp_matcher : public matcher<std::string>
{
    explicit contains_regexp_matcher( std::string const & expr ) : expr(expr) { }

    Matcher<std::string> copy() const { return Matcher<std::string>( new contains_regexp_matcher( *this ) ); }

    bool operator()( std::string const & actual ) const { return std::regex_search( actual, std::regex(expr) ); }

    std::string to_string() const { return "contains " + lest::to_string( expr ); }

    const std::string expr;
};

/**
 * matches_regexp( expr )
 */
struct match_regexp_matcher : public matcher<std::string>
{
    explicit match_regexp_matcher( std::string const & expr ) : expr(expr) { }

    Matcher<std::string> copy() const { return Matcher<std::string>( new match_regexp_matcher( *this ) ); }

    bool operator()( std::string const & actual ) const { return std::regex_match( actual, std::regex(expr) ); }

    std::string to_string() const { return "matches " + lest::to_string( expr ); }

    const std::string expr;
};

/**
 * is( matcher ), is( value )
 */
template <typename T>
struct is_matcher : matcher<T>
{
    explicit is_matcher( matcher<T> const & match ) : match( match.copy() ) { }

    Matcher<T> copy() const { return match; }

    bool operator()( T const & actual ) const { return (*match)( actual ); }

    std::string to_string() const { return match->to_string(); }

    const Matcher<T> match;
};

/**
 * is_not( matcher ), is_not( value )
 */
template <typename T>
struct is_not_matcher : matcher<T>
{
    explicit is_not_matcher( matcher<T> const & match ) : match( match.copy() ) { }

    Matcher<T> copy() const { return match; }

    bool operator()( T const & actual ) const { return ! (*match)( actual ); }

    std::string to_string() const { return "not (" + match->to_string() + ")"; }

    const Matcher<T> match;
};

/**
 * base class for logic matchers, any_of(), all_of().
 */
template <typename T>
struct logical_matcher : matcher<T>
{
protected:
    logical_matcher() { }

    template <typename M, typename... Ms>
    logical_matcher( M const & m, Ms const &... ms) : matchers()
    {
        combine( m, ms... );
    }

    template <typename U>
    logical_matcher( std::initializer_list<U> const & list ) : matchers()
    {
        for ( auto const & x : list )
        {
            push_back( equal_to_matcher<T>( x ) );
        }
    }

    bool any_of( T const & expected ) const
    {
        return std::any_of(
            matchers.begin(), matchers.end(),
            [expected]( Matcher<T> match ) { return (*match)(expected); }
        );
    }

    bool all_of( T const & expected ) const
    {
        return std::all_of(
            matchers.begin(), matchers.end(),
            [expected]( Matcher<T> match ) { return (*match)(expected); }
        );
    }

    std::string to_string_op( std::string op ) const
    {
        const std::string padded_op = " " + op + " ";

        auto first = matchers.begin();
        auto  last = matchers.end();

        std::string result = "(" + (*first)->to_string();

        ++first;
        while (first != last)
        {
            result += padded_op + (*first)->to_string();
            ++first;
        }

        return result + ")";
    }

private:
    void combine() {}

    template <typename M, typename... Ms>
    void combine( M const & m, Ms const &... ms)
    {
        push_back( m );
        combine( ms... );
    }

    template <typename M>
    void push_back( M const & m )
    {
        matchers.push_back( Matcher<T>( m.copy() ) );
    }

    std::vector<Matcher<T>> matchers;
};

/**
 * any_of( {...} ), any_of( matcher... ).
 */
template <typename T>
struct any_of_matcher : public logical_matcher<T>
{
    template <typename M, typename... Ms>
    any_of_matcher( M const & m, Ms const &... ms ) : logical_matcher<T>( m, ms... ) { }

    any_of_matcher( std::initializer_list<T> const & list ) : logical_matcher<T>( list ) { }

    Matcher<T> copy() const { return Matcher<T>( new any_of_matcher( *this ) ); }

    bool operator()( T const & actual ) const { return this->any_of( actual ); }

    std::string to_string() const { return this->to_string_op( "or" ); }
};

/**
 * all_of( {...} ), all_of( matcher... ).
 */
template <typename T>
struct all_of_matcher : public logical_matcher<T>
{
    template <typename M, typename... Ms>
    all_of_matcher( M const & m, Ms const &... ms ) : logical_matcher<T>( m, ms... ) { }

    all_of_matcher( std::initializer_list<T> const & list ) : logical_matcher<T>( list ) { }

    Matcher<T> copy() const { return Matcher<T>( new all_of_matcher( *this ) ); }

    bool operator()( T const & actual ) const { return this->all_of( actual ); }

    std::string to_string() const { return this->to_string_op( "and" ); }
};

LEST_TAG_MATCHER( close_to_matcher );
LEST_TAG_MATCHER( anything );
LEST_TAG_MATCHER( same_instance_matcher );
LEST_TAG_MATCHER( is_matcher );
LEST_TAG_MATCHER( is_not_matcher );
LEST_TAG_MATCHER( any_of_matcher );
LEST_TAG_MATCHER( all_of_matcher );

// matcher funtions to resolve types:

template <typename T>
close_to_matcher<T> close_to( T const & expected, T const & delta )
{
    return close_to_matcher<T>( expected, delta );
}

template <typename T>
equal_to_matcher<T> equal_to( T const & expected )
{
    return equal_to_matcher<T>( expected );
}

template <typename T>
equal_to_matcher<const T*> equal_to( const T expected[] )
{
    return equal_to_matcher<const T*>( expected );
}

template <typename T>
not_equal_to_matcher<T> not_equal_to( T const & expected )
{
    return not_equal_to_matcher<T>( expected );
}

template <typename T>
not_equal_to_matcher<const T*> not_equal_to( const T expected[] )
{
    return not_equal_to_matcher<const T*>( expected );
}

template <typename T>
less_than_matcher<T> less_than( T const & expected )
{
    return less_than_matcher<T>( expected );
}

template <typename T>
less_than_matcher<const T*> less_than( const T expected[] )
{
    return less_than_matcher<const T*>( expected );
}

template <typename T>
less_equal_matcher<T> less_equal( T const & expected )
{
    return less_equal_matcher<T>( expected );
}

template <typename T>
less_equal_matcher<const T*> less_equal( const T expected[] )
{
    return less_equal_matcher<const T*>( expected );
}

template <typename T>
greater_than_matcher<T> greater_than( T const & expected )
{
    return greater_than_matcher<T>( expected );
}

template <typename T>
greater_than_matcher<const T*> greater_than( const T expected[] )
{
    return greater_than_matcher<const T*>( expected );
}

template <typename T>
greater_equal_matcher<T> greater_equal( T const & expected )
{
    return greater_equal_matcher<T>( expected );
}

template <typename T>
greater_equal_matcher<const T*> greater_equal( const T expected[] )
{
    return greater_equal_matcher<const T*>( expected );
}

template <typename T>
same_instance_matcher<T> same_instance( T const & instance )
{
    return same_instance_matcher<T>( instance );
}

template <typename T>
starts_with_matcher starts_with( T const & text )
{
    return starts_with_matcher( text );
}

template <typename T>
ends_with_matcher ends_with( T const & text )
{
    return ends_with_matcher( text );
}

template <typename T>
contains_matcher contains( T const & text )
{
    return contains_matcher( text );
}

template <typename T>
contains_regexp_matcher contains_regexp( T const & expr )
{
    return contains_regexp_matcher( expr );
}

template <typename T>
match_regexp_matcher matches_regexp( T const & expr )
{
    return match_regexp_matcher( expr );
}

template <typename T>
is_matcher<typename T::argument_type> is_implementation( T const & expected, matcher_tag )
{
    return is_matcher<typename T::argument_type>( expected );
}

template <typename T>
is_matcher<T> is_implementation( T const & expected, not_matcher_tag )
{
    return is_matcher<T>( equal_to( expected ) );
}

template <typename T>
is_matcher<typename matcher_traits<T>::argument> is( T const & expected )
{
    return is_implementation( expected, typename matcher_traits<T>::category() );
}

template <typename T>
is_matcher<const T*> is( const T expected[] )
{
    return is_implementation( expected, not_matcher_tag() );
}

template <typename T>
is_not_matcher<typename T::argument_type> is_not_implementation( T const & expected, matcher_tag )
{
    return is_not_matcher<typename T::argument_type>( expected );
}

template <typename T>
is_not_matcher<T> is_not_implementation( T const & expected, not_matcher_tag )
{
    return is_not_matcher<T>( equal_to( expected ) );
}

template <typename T>
is_not_matcher<typename matcher_traits<T>::argument> is_not( T const & expected )
{
    return is_not_implementation( expected, typename matcher_traits<T>::category() );
}

template <typename T>
is_not_matcher<const T*> is_not( const T expected[] )
{
    return is_not_implementation( expected, not_matcher_tag() );
}

template <typename T>
any_of_matcher<T> any_of( std::initializer_list<T> const & list )
{
    return any_of_matcher<T>( list );
}

template <typename M, typename... Ms>
any_of_matcher<typename M::argument_type> any_of( M const & m, Ms const &... ms )
{
    return any_of_matcher<typename M::argument_type>( m, ms... );
}

template <typename T>
all_of_matcher<T> all_of( std::initializer_list<T> const & list )
{
    return all_of_matcher<T>( list );
}

template <typename M, typename... Ms>
all_of_matcher<typename M::argument_type> all_of( M const & m, Ms const &... ms )
{
    return all_of_matcher<typename M::argument_type>( m, ms... );
}


namespace match {

// object:

using lest::same_instance;

// numeric:

using lest::close_to;

using lest::equal_to;
using lest::not_equal_to;

using lest::less_than;
using lest::less_equal;

using lest::greater_than;
using lest::greater_equal;

// textual:

using lest::starts_with;
using lest::ends_with;
using lest::contains;
using lest::contains_regexp;    // fails with g++ 4.8.1
using lest::matches_regexp;

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

using lest::anything;

using lest::is;
using lest::is_not;

using lest::all_of;
using lest::any_of;

} // namespace match

} // namespace lest

#undef LEST_TAG_MATCHER
#undef LEST_COMPARISON_MATCHER

#endif // LEST_HAMLEST_H_INCLUDED
