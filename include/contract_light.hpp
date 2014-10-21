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
#include "contract_light_context.hpp"

#include <type_traits>
#include <utility>

namespace contract_light
{
#ifdef HAS_INLINE_NAMESPACE
  inline
#endif
  namespace v_100 {
    /**
     * Function signature to handle failed preconditions
     * @fileName The file where the precondition was defined that failed
     * @line The line number where the precondition was defined that failed
     */
    using PreConditionFailedFunction = void(*)(const char* fileName, int line);

    /**
     * Function signature to handle failed postconditions
     * @fileName The file where the precondition was defined that failed
     * @line The line number where the precondition was defined that failed
     */
    using PostConditionFailedFunction = void(*)(const char*, int);

    /**
     * Function signature to handle failed invariants
     * @fileName The file where the precondition was defined that failed
     * @line The line number where the precondition was defined that failed
     */
    using InvariantFailedFunction = void(*)(const char*, int);

    /**
      * Set an alternate pre condition failed handler. The default version
      * just prints the failure location to std::cout
      */
    void setHandlerFailedPreCondition(PreConditionFailedFunction) NOEXCEPT;

    /**
      * Set an alternate post condition failed handler. The default version
      * just prints the failure location to std::cout
      * The function itself must not throw!
      */
    void setHandlerFailedPostCondition(PostConditionFailedFunction) NOEXCEPT;

    /**
      * Set an alternate invariant failed handler. The default version
      * just prints the failure location to std::cout
      * The function itself must not throw!
      */
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

      void handleFailedPostCondition(const char* filename, int lineNumber) NOEXCEPT;

      void handleFailedInvariant(const char* filename, int lineNumber) NOEXCEPT;

      struct NoInvariantPolicy
      {
        template <typename C>
        static void pushInvariantOnStack(C&) NOEXCEPT{}

          template <typename C>
        static void checkInvariant(C&) NOEXCEPT{}
      };

      struct InvariantPolicy
      {
        template <typename Context>
        static void pushInvariantOnStack(Context& ctx) NOEXCEPT{
          ctx.provider.pushInvariantOnStack();
        }

          template <typename Context>
        static void checkInvariant(Context& ctx) NOEXCEPT{
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
#ifndef HAS_INLINE_NAMESPACE
  using namespace v_100;
#endif
}



/**
 * Defines a precondtion. Must be followed by a callable object.
 * Several ones can be defined within a single function
 * the current scope is left. As well the invariant is checked whenever one is defined.
 * E.g. PRECONDITION [this]{ return myMember_ > 42;};
  */
#define PRECONDITION auto ANONYMOUS_VARIABLE(CONTRACT_STATE) =                \
      ::contract_light::contract_detail::PreConditionContext<std::remove_reference<decltype(*this)>::type>(*this, __FILE__, __LINE__) + 

/**
 * Defines a postcondtion. Must be followed by a callable object.
 * Several ones can be defined within a single function. The check is executed whenever
 * the current scope is left. As well the invariant is checked whenever one is defined.
 * E.g. POSTCONDITION [this]{ return result > 42;};
  */
#define POSTCONDITION auto ANONYMOUS_VARIABLE(CONTRACT_STATE) =               \
      ::contract_light::contract_detail::PostConditionContext<std::remove_reference<decltype(*this)>::type>(*this, __FILE__, __LINE__) + 

/**
 * Defines that the invariant shall be called whenever the current scope is left
 * E.g. INVARIANT;
  */
#define INVARIANT auto ANONYMOUS_VARIABLE(CONTRACT_STATE) =                   \
      ::contract_light::contract_detail::makeInvariant(::contract_light::contract_detail::ContractContext<std::remove_reference<decltype(*this)>::type>(*this, __FILE__, __LINE__));

