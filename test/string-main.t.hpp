// Copyright 2021-2021 by Martin Moene
//
// https://github.com/martinmoene/string-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef TEST_STRING_LITE_H_INCLUDED
#define TEST_STRING_LITE_H_INCLUDED

#include string_STRING_HEADER

// Compiler warning suppression for usage of lest:

#ifdef __clang__
# pragma clang diagnostic ignored "-Wstring-conversion"
# pragma clang diagnostic ignored "-Wunused-parameter"
# pragma clang diagnostic ignored "-Wunused-template"
# pragma clang diagnostic ignored "-Wunused-function"
# pragma clang diagnostic ignored "-Wunused-member-function"
#elif defined __GNUC__
# pragma GCC   diagnostic ignored "-Wunused-parameter"
# pragma GCC   diagnostic ignored "-Wunused-function"
#endif

// Limit C++ Core Guidelines checking to GSL Lite:

#if string_COMPILER_MSVC_VER >= 1910
# include <CppCoreCheck/Warnings.h>
# pragma warning(disable: ALL_CPPCORECHECK_WARNINGS)
#endif

#include <iostream>

#if string_CPP17_OR_GREATER && defined(__has_include )
# if __has_include( <string_view> )
#  define string_HAVE_STD_STRING_VIEW  1
# else
#  define string_HAVE_STD_STRING_VIEW  0
# endif
#else
# define  string_HAVE_STD_STRING_VIEW  0
#endif

// String to use with iterators:

inline std::string & text()
{
    static std::string text_("abc123xyz");
    return text_;
}

inline std::string::const_iterator text_end()
{
    return text().end();
}

#if string_HAVE_STD_STRING_VIEW

inline std::string_view text_view()
{
    return { text().c_str(), text().length() };
}

inline std::string_view::const_iterator text_view_end()
{
    return text_view().end();
}

#endif // string_HAVE_STD_STRING_VIEW

// Provide CsString with C++17 and later:

#define string_HAS_CSSTRING string_CPP17_140

#if string_HAS_CSSTRING

#include "cs_string/cs_string.h"
#include "cs_string/cs_string_view.h"

// String to use with iterators:

inline CsString::CsString & cstext()
{
    static CsString::CsString text_("αβγ123χψω");
    return text_;
}

inline CsString::CsString::const_iterator cstext_end()
{
    return cstext().end();
}

inline CsString::CsStringView::const_iterator cstext_view_end()
{
    return CsString::CsStringView( cstext() ).end();
}

namespace nonstd {

// use oparator<< instead of to_string() overload;
// see  http://stackoverflow.com/a/10651752/437272

template< typename CharT, typename AllocT >
inline std::ostream & operator<< ( std::ostream & os, CsString::CsBasicString<CharT,AllocT> const & s )
{
    return os << s.constData();
}

template< typename CharT, typename AllocT >
inline std::ostream & operator<< ( std::ostream & os, typename CsString::CsBasicString<CharT,AllocT>::CsStringIterator pos )
{
    return os << "[CsStringIterator]";

    if ( pos == cstext_end() )
        return os << "[end]";

    os << "\"";
    for ( ; pos != cstext_end(); ++pos )
    {
        os << (*pos).unicode();
    }
    return os << "\"";
}

} // namespace nonstd

#endif // string_HAS_CSSTRING

namespace nonstd {

inline std::ostream & operator<< ( std::ostream & os, std::string::const_iterator pos )
{
    // return os << "[it]";

    if ( pos == text_end() )
        return os << "[end]";

    os << "\"";
    for ( ; pos != text_end(); ++pos )
    {
        os << *pos;
    }
    return os << "\"";
}

#if string_HAVE_STD_STRING_VIEW && _MSC_VER

inline std::ostream & operator<< ( std::ostream & os, std::string_view::const_iterator pos )
{
    // return os << "[it-sv]";

    if ( pos == text_view_end() )
        return os << "[end-sv]";

    os << "\"";
    for ( ; pos != text_view_end(); ++pos )
    {
        os << *pos;
    }
    return os << "\"";
}
#endif // string_HAVE_STD_STRING_VIEW

} // namespace nonstd

namespace lest {

using ::nonstd::operator<<;

} // namespace lest

#include "lest_cpp03.hpp"

extern lest::tests & specification();

#define CASE( name ) lest_CASE( specification(), name )

#endif // TEST_STRING_LITE_H_INCLUDED

// end of file
