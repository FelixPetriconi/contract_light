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
      template <typename T>
      class has_invariant
      {
        template<typename U>
        static auto try_method(U* p) -> decltype(p->invariant(), std::true_type());

        template<typename U>
        static std::false_type try_method(...);

        using result_type = decltype(try_method<T>(nullptr));
      public:
        static const bool value = result_type::value;
      };

    }
  }
}
