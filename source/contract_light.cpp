///////////////////////////////////////////////////////////////////
//
// Copyright 2014 Felix Petriconi
//
// License: http://boost.org/LICENSE_1_0.txt, Boost License 1.0
//
// Authors: http://petriconi.net, Felix Petriconi
//
//////////////////////////////////////////////////////////////////

#include "contract_light.hpp"
#include <iostream>

namespace {
  void defaultHandlerFailedPrecondition(const char* filename, int lineNumber) {
    std::cout << "PreCondition failed in " << filename << ":" << lineNumber;
  }

  void defaultHandlerFailedPostcondition(const char* filename, int lineNumber) {
    std::cout << "PostCondition failed in " << filename << ":" << lineNumber;
  }

  void defaultHandlerFailedInvariant(const char* filename, int lineNumber) {
    std::cout << "Invariant failed in " << filename << ":" << lineNumber;
  }

  contract_light::PreConditionFailedFunction preConditionFailed = &defaultHandlerFailedPrecondition;
  contract_light::PostConditionFailedFunction postConditionFailed = &defaultHandlerFailedPostcondition;
  contract_light::InvariantFailedFunction invariantFailed = &defaultHandlerFailedInvariant;

}


namespace contract_light {
  namespace v_100 {
    void setHandlerFailedPreCondition(PreConditionFailedFunction h) NOEXCEPT {
      if (h != nullptr) {
        preConditionFailed = h;
      }
    }

    void setHandlerFailedPostCondition(PostConditionFailedFunction h) NOEXCEPT {
      if (h != nullptr) {
        postConditionFailed = h;
      }
    }

    void setHandlerFailedInvariant(InvariantFailedFunction h) NOEXCEPT {
      if (h != nullptr) {
        invariantFailed = h;
      }
    }

    namespace contract_detail {
      void handleFailedPreCondition(const char* filename, int lineNumber) {
        preConditionFailed(filename, lineNumber);
      }

      void handleFailedPostCondition(const char* filename, int lineNumber) {
        postConditionFailed(filename, lineNumber);
      }

      void handleFailedInvariant(const char* filename, int lineNumber)  NOEXCEPT {
        invariantFailed(filename, lineNumber);
      }
    }

  }
}
 


