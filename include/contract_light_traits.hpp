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

#include <type_traits>

namespace contract_light
{
#ifdef HAS_INLINE_NAMESPACE
	inline
#endif
  namespace v_100 
  {
    namespace contract_detail
    {
      /**
       * Traits checks if the given type has a bool invariant() const method
       */
      template <typename T>
      class has_invariant
      {
        template<typename U, bool(U::*)() const> 
        struct SFINAE {};
        
        template<typename U> 
        static std::true_type try_method(SFINAE<U, &U::invariant>*);

        template<typename U>
        static std::false_type try_method(...);

        using result_type = decltype(try_method<T>(nullptr));
      public:
        static const bool value = result_type::value;
      };

      /**
       * Typed If, thanks to Walter E. Brown from CppCon 2014 
       */
      template <bool F, typename X, typename Y>
      using IF_t = typename std::conditional<F, X, Y>::type;

    }
  }
}
