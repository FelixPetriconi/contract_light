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
#include "contract_light_helper.hpp"
#include "contract_light_traits.hpp"

namespace contract_light
{
  namespace v_100 {
    using PreConditionFailedFunction = void(*)(const char*, int);

    using PostConditionFailedFunction = void(*)(const char*, int);

    using InvariantFailedFunction = void(*)(const char*, int);

    void setHandlerFailedPreCondition(PreConditionFailedFunction) NOEXCEPT;

    void setHandlerFailedPostCondition(PostConditionFailedFunction) NOEXCEPT;

    void setHandlerFailedInvariant(InvariantFailedFunction) NOEXCEPT;


    class Contract
    {
      mutable int _invariantStack;
    public:
      Contract() : _invariantStack(0) {}

      void pushInvariantOnStack() const NOEXCEPT{
        ++_invariantStack;
      }

        void popInvariantFromStack() const NOEXCEPT{
        --_invariantStack;
      }

      bool stackEmpty() const NOEXCEPT {
        return _invariantStack == 0;
      }
    };


    namespace contract_detail
    {
      void handleFailedPreCondition(const char* filename, int lineNumber);

      void handleFailedPostCondition(const char* filename, int lineNumber);

      void handleFailedInvariant(const char* filename, int lineNumber) NOEXCEPT;


      template <typename T>
      struct ContractContext {
        using provider_type = T;
        const char* fileName;
        const int line;
        const T& provider;
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

      struct NoInvariantPolicy
      {
        template <typename C>
        static void pushInvariantOnStack(C&) NOEXCEPT {}

        template <typename C>
        static void checkInvariant(C&) NOEXCEPT {}
      };

      struct InvariantPolicy
      {
        template <typename Context>
        static void pushInvariantOnStack(Context& ctx) NOEXCEPT {
          ctx.provider.pushInvariantOnStack();
        }

        template <typename Context>
        static void checkInvariant(Context& ctx) NOEXCEPT {
          ctx.provider.popInvariantFromStack();
          if (ctx.provider.stackEmpty() && !ctx.provider.invariant()) {
            handleFailedInvariant(ctx.fileName, ctx.line);
          }
        }
      };


      template <typename Context, typename Op>
      class PreCondition
      {
      public:
        using Provider = typename Context::provider_type;
        using InvariantPolicy = typename std::conditional <
          has_invariant<Provider>::value,
          InvariantPolicy,
          NoInvariantPolicy
        > ::type;

        static_assert(!has_invariant<Provider>::value ||
          (has_invariant<Provider>::value && std::is_base_of<Contract, Provider>::value),
          "A class that uses invariants must have the base class Contract!");

        const Context _context;

      public:
        PreCondition(Context&& ctx, Op&& op)
          : _context(std::forward<Context>(ctx))
        {
          static_assert(std::is_same<bool, decltype(op())>::value,
            "Pre-Condition must be a callable object returning a boolean");

          if (!op()) {
            handleFailedPreCondition(_context.fileName, _context.line);
          }

          InvariantPolicy::pushInvariantOnStack(_context);
        }

        ~PreCondition()
        {
          InvariantPolicy::checkInvariant(_context);
        }
      };

      template <typename Context, typename Op>
      class PostCondition
      {
        using Provider = typename Context::provider_type;
        using InvariantPolicy = typename std::conditional <
          has_invariant<Provider>::value,
          InvariantPolicy,
          NoInvariantPolicy
        > ::type;

        static_assert(!has_invariant<Provider>::value ||
          (has_invariant<Provider>::value && std::is_base_of<Contract, Provider>::value),
          "A class that uses invariants must have the base class Contract!");

        const Context _context;
        Op _op;

      public:
        PostCondition(Context&& ctx, Op&& op)
          : _context(std::forward<Context>(ctx))
          , _op(std::forward<Op>(op))
        {
          static_assert(std::is_same<bool, decltype(op())>::value,
            "Post-Condition must be a callable object returning a boolean");

          InvariantPolicy::pushInvariantOnStack(_context);
        }

        ~PostCondition() {
          if (!_op()) {
            handleFailedPostCondition(_context.fileName, _context.line);
          }

          InvariantPolicy::checkInvariant(_context);
        }
      };


      template <typename Context>
      class Invariant
      {
        static_assert(has_invariant<typename Context::provider_type>::value,
          "An Invariant can only be used if the Provider class has a bool invariant() const method");

        const Context _context;
      public:
        Invariant(Context&& ctx)
          : _context(std::forward<Context>(ctx)) {

          InvariantPolicy::pushInvariantOnStack(_context);
        }

        ~Invariant() {
          InvariantPolicy::checkInvariant(_context);
        }
      };


      template <typename T, typename Op>
      PreCondition<PreConditionContext<T>, Op> operator+(PreConditionContext<T>&& ctx, Op&& op)
      {
        using Context = PreConditionContext < T > ;
        return PreCondition<Context, Op>(std::forward<Context>(ctx), std::forward<Op>(op));
      }


      template <typename T, typename Op>
      PostCondition<PostConditionContext<T>, Op> operator+(PostConditionContext<T>&& ctx, Op&& op)
      {
        using Context = PostConditionContext < T > ;
        return PostCondition<Context, Op>(std::forward<Context>(ctx), std::forward<Op>(op));
      }

      template <typename T>
      Invariant<ContractContext<T>> makeInvariant(ContractContext<T>&& ctx) {
        return Invariant<ContractContext<T>>(std::forward<ContractContext<T>>(ctx));
      }
    }
  }
  using namespace v_100;
}




#define PRECONDITION auto ANONYMOUS_VARIABLE(CONTRACT_STATE) =                \
      ::contract_light::contract_detail::PreConditionContext<std::remove_reference<decltype(*this)>::type>(*this, __FILE__, __LINE__) + 

#define POSTCONDITION auto ANONYMOUS_VARIABLE(CONTRACT_STATE) =               \
      ::contract_light::contract_detail::PostConditionContext<std::remove_reference<decltype(*this)>::type>(*this, __FILE__, __LINE__) + 

#define INVARIANT auto ANONYMOUS_VARIABLE(CONTRACT_STATE) =                   \
      ::contract_light::contract_detail::makeInvariant(::contract_light::contract_detail::ContractContext<std::remove_reference<decltype(*this)>::type>(*this, __FILE__, __LINE__));

