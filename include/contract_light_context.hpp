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

#include "contract_light_helper.hpp"
#include "contract_light_traits.hpp"

namespace contract_light
{
#ifdef HAS_INLINE_NAMESPACE
	inline
#endif
  namespace v_100
  {
    namespace contract_detail
    {
      template <typename T>
      struct ContractContext 
      {
        using provider_type = T;
        const T& provider;
        const char* fileName;
        const int line;

        ContractContext(T& p, const char* fn, int ln) : provider(p), fileName(fn), line(ln) {}
      };


      template <typename T>
      struct PreConditionContext : public ContractContext < T >
      {
        PreConditionContext(T& p, const char* fn, int ln) : ContractContext<T>(p, fn, ln) {}
      };

      template <typename T>
      struct PostConditionContext : public ContractContext < T >
      {
        PostConditionContext(T& p, const char* fn, int ln) : ContractContext<T>(p, fn, ln) {}
      };
    }
  }
}