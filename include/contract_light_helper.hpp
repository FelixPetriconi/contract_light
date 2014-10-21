///////////////////////////////////////////////////////////////////
//
// Copyright 2014 Felix Petriconi
//
// License: http://boost.org/LICENSE_1_0.txt, Boost License 1.0
//
// Authors: http://petriconi.net, Felix Petriconi
//
//////////////////////////////////////////////////////////////////
#pragma once

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)
#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(str) \
CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) \
CONCATENATE(str, __LINE__)
#endif

#ifdef _MSC_VER 
#if _MSC_VER < 1900
#define NOEXCEPT throw()
#endif
#if _MSC_VER > 1900
#define HAS_INLINE_NAMESPACE
#endif
#else
#define NOEXCEPT noexcept
#define HAS_INLINE_NAMESPACE
#endif

