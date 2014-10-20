///////////////////////////////////////////////////////////////////
//
// Copyright 2014 Felix Petriconi
//
// License: http://boost.org/LICENSE_1_0.txt, Boost License 1.0
//
// Authors: http://petriconi.net, Felix Petriconi
//
//////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "contract_light.hpp"

namespace
{
  class TestClassWithInvariant : public contract_light::Contract
  {
  public:
    int x;
    int y;
    mutable int invariantWasCalled;
    bool preConditionWasCalled;
    mutable int postConditionWasCalled;

    TestClassWithInvariant()
      : x(0)
      , y(0)
      , invariantWasCalled(0)
      , preConditionWasCalled(false)
      , postConditionWasCalled(0)
    {}

    ~TestClassWithInvariant() {
      INVARIANT;
    }

    void setX(int newX) {
      PRECONDITION[&, this]() -> bool { preConditionWasCalled = true;  return newX > 0; };
      POSTCONDITION[&, this]()-> bool { ++postConditionWasCalled;  return newX == x; };
      x = newX;
    }

    void doubleX(int newX) {
      POSTCONDITION[&, this]()-> bool { ++postConditionWasCalled;  return x == newX * 2; };

      x = newX * 3; // This is of course wrong and should be caught by the post condition
    }

    bool invariant() const {
      invariantWasCalled++;
      return y == 0;
    }
  };


  struct PreConditionFailedEx : public std::exception
  {};

  void myPreConditionFailedHandler(const char* , int ) {
    throw PreConditionFailedEx();
  }

  bool postConditionHasFailedFlag = false;
  void myPostConditionFailedHandler(const char*, int) {
    postConditionHasFailedFlag = true;
  }

  bool invariantHasFailedFlag = false;
  void myInvariantFailedHandler(const char* , int ) {
    invariantHasFailedFlag = true;
  }
}


class ContractTestOnClassWithInvariant : public ::testing::Test
{
protected:
  ContractTestOnClassWithInvariant() {
    contract_light::setHandlerFailedPreCondition(&myPreConditionFailedHandler);
    contract_light::setHandlerFailedPostCondition(&myPostConditionFailedHandler);
    contract_light::setHandlerFailedInvariant(&myInvariantFailedHandler);
    postConditionHasFailedFlag = false;
    invariantHasFailedFlag = false;
  }

  TestClassWithInvariant sut;
};

TEST_F(ContractTestOnClassWithInvariant, ThatWhenNoFunctionWasCalledNoConditionAndInvariantWasCalled)
{
  EXPECT_EQ(0, sut.invariantWasCalled);
  EXPECT_FALSE(sut.preConditionWasCalled);
  EXPECT_FALSE(sut.postConditionWasCalled);
}

TEST_F(ContractTestOnClassWithInvariant, ThatACallToAFullfilledPreconditionDoesNotFireAndThatTheInvariantWasCalled)
{
  EXPECT_NO_THROW(sut.setX(1));
  EXPECT_EQ(1, sut.invariantWasCalled);
  EXPECT_TRUE(sut.preConditionWasCalled);
  EXPECT_EQ(1, sut.postConditionWasCalled);
}

TEST_F(ContractTestOnClassWithInvariant, ThatACallToAFailingPreconditionFireTheSetExceptionAndThatTheInvariantWasCalledAndThatTheStateOfTheClassDoesNotChange)
{
  auto oldX = sut.x;
  auto oldY = sut.y;
  EXPECT_THROW(sut.setX(0), PreConditionFailedEx);
  
  EXPECT_EQ(0, sut.invariantWasCalled);
  EXPECT_TRUE(sut.preConditionWasCalled);
  EXPECT_FALSE(sut.postConditionWasCalled);

  EXPECT_EQ(oldX, sut.x);
  EXPECT_EQ(oldY, sut.y);
}


TEST_F(ContractTestOnClassWithInvariant, ThatACallToAFailingPreConditionFireTheSetExceptionAndThatTheInvariantWasNotCalledAndThatTheStateOfTheClassDoesNotChange)
{
  auto oldX = sut.x;
  auto oldY = sut.y;
  EXPECT_THROW(sut.setX(0), PreConditionFailedEx);

  EXPECT_EQ(0, sut.invariantWasCalled);
  EXPECT_TRUE(sut.preConditionWasCalled);
  EXPECT_FALSE(sut.postConditionWasCalled);

  EXPECT_EQ(oldX, sut.x);
  EXPECT_EQ(oldY, sut.y);
}

TEST_F(ContractTestOnClassWithInvariant, ThatACallToAFailingPostConditionFireTheSetExceptionAndThatTheInvariantWasCalled)
{
  EXPECT_NO_THROW(sut.doubleX(2));
  EXPECT_TRUE(postConditionHasFailedFlag);

  EXPECT_EQ(1, sut.invariantWasCalled);
  EXPECT_EQ(1, sut.postConditionWasCalled);
}

namespace
{

  class TestClassWithOutInvariant
  {
  public:
    int x;
    int y;

    bool preConditionWasCalled;
    bool postConditionWasCalled;

    TestClassWithOutInvariant()
      : x(42)
      , y(42)
      , preConditionWasCalled(false)
      , postConditionWasCalled(false)
    {}

    ~TestClassWithOutInvariant()
    {
      // INVARIANT; Placing here causes correctly a compile error
    }

    void setX(int newX) {
      PRECONDITION[&, this]() -> bool { preConditionWasCalled = true;  return newX > 0; };
      POSTCONDITION[&, this]()-> bool { postConditionWasCalled = true;  return newX == x; };
      x = newX;
    }

    void doubleX(int newX) {
      POSTCONDITION[&, this]()-> bool { postConditionWasCalled = true;  return x == newX * 2; };

      x = newX * 3; // This is of course wrong and should be caught by the post condition
    }

  };

}

class ContractTestOnClassWithOutInvariant : public ::testing::Test
{
protected:
  TestClassWithOutInvariant sut;
};


TEST_F(ContractTestOnClassWithOutInvariant, ThatWhenNoFunctionWasCalledNoConditionWasCalled)
{
  EXPECT_FALSE(sut.preConditionWasCalled);
  EXPECT_FALSE(sut.postConditionWasCalled);
}

TEST_F(ContractTestOnClassWithOutInvariant, ThatACallToAFullfilledPreconditionDoesNotFire)
{
  EXPECT_NO_THROW(sut.setX(1));
  EXPECT_TRUE(sut.preConditionWasCalled);
  EXPECT_TRUE(sut.postConditionWasCalled);
}

TEST_F(ContractTestOnClassWithOutInvariant, ThatACallToAFailingPreconditionFireTheSetException)
{
  auto oldX = sut.x;
  auto oldY = sut.y;
  EXPECT_THROW(sut.setX(0), PreConditionFailedEx);

  EXPECT_TRUE(sut.preConditionWasCalled);
  EXPECT_FALSE(sut.postConditionWasCalled);

  EXPECT_EQ(oldX, sut.x);
  EXPECT_EQ(oldY, sut.y);
}


TEST_F(ContractTestOnClassWithOutInvariant, ThatACallToAFailingPreConditionFireTheSetException)
{
  auto oldX = sut.x;
  auto oldY = sut.y;
  EXPECT_THROW(sut.setX(0), PreConditionFailedEx);

  EXPECT_TRUE(sut.preConditionWasCalled);
  EXPECT_FALSE(sut.postConditionWasCalled);

  EXPECT_EQ(oldX, sut.x);
  EXPECT_EQ(oldY, sut.y);
}

TEST_F(ContractTestOnClassWithOutInvariant, ThatACallToAFailingPostConditionFireTheSetException)
{
  EXPECT_NO_THROW(sut.doubleX(2));
  EXPECT_TRUE(postConditionHasFailedFlag);

  EXPECT_TRUE(sut.postConditionWasCalled);
}

namespace 
{
  class TestClassMultipleConditionsAndInvariant : public contract_light::Contract
  {
  public:
    TestClassMultipleConditionsAndInvariant() 
      : invariantCalled(0)
    {}

    void dummy1() {
      PRECONDITION[] { return true; };
      PRECONDITION[] { return true; };
      POSTCONDITION[] { return true; };
      POSTCONDITION[] { return true; };
      INVARIANT;
      INVARIANT;
    }

    bool invariant() const {
      ++invariantCalled;
      return true;
    }

    mutable int invariantCalled;
  };
}

TEST(ContractInvariantTest, ThatMultipleConditionsAndInvariantsResultOnlyInASingleInvariantCall)
{
  TestClassMultipleConditionsAndInvariant sut;
  sut.dummy1();
  EXPECT_EQ(1, sut.invariantCalled);

}