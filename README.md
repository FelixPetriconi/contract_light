Contract Light {#mainpage}
==============

A library that offers in a light-weight manner Design by Contract constructs


Motivation
----------
Currently C++ 11/14 does not support natively the concept of [Design by Contract](http://en.wikipedia.org/wiki/Design_by_contract). Several libraries are available that support Design by Contract, but all that I know of, use heavily preprocessor makros. So this is my attempt to support this feature with pure C++. 
  
Contract Light Overview
-----------------------
This library supports pre-condition, post-condition and invariants for classes and structs.
Simple example of a rectangular implementation.

~~~C++
class Rect : 
  // derive from this base, if an invariant is defined. If no invariant is defined
  // no need to do it
  public contract_light::Contract 
{
  int w_;
  int h_;
public:
  Rect() : w_(0), h_(0) {}
  
  ~Rect() {
    // checking that the invariants are fulfilled, even before destruction
    INVARIANT;
  }
  
  void setWidth(int newW) {
    // setting the pre condition of this method; the invariant is checked 
    // once at the end of the scope
    PRECONDITION [&] { return newW >= 0; };
    // setting the post condition; the invariant is checked only once after 
    // leaving the scope, regardless of the number of pre- and/or post-conditions
    POSTCONDITION [&, this] { return newW == w_; };
    w_ = newW;
    
    // There is no need to define here INVARIANT here. This is done automatically
    // whenever there is a bool invariant() const method is defined
  }
  
  int size() const {
    int result;
    POSTCONDITION [&] { return result == w_ * h_; }
    result = w_ * h_;
    
    return result;
  }
  
  bool invariant() const {
    return w_ >= 0 && h_ >= 0;
  }
};
~~~

Documentation
-------------
  |Keyword                      |Meaning                            |
  ----------------------------- |-----------------------------------
  | PRECONDITION | Specifies the following callable expression as precondition. This is executed at the point of definition. As well an available invariant is registered to be executed whenever the current scope is left. |
  | POSTCONDITION | Specifies the following callable expression as postcondition. This gets executed  in the moment of leaving the current scope. Whenever a precondition is defined before and an invariant is available, then the invariant is executed only once after the most recent postcondition. |
  | INVARIANT | Executes the defined invariant at that location |
  | setHandlerFailedPreCondition | Set a private handler function that gets called whenever a precondition is not fulfilled. This function may throw. |
  | setHandlerFailedPostCondition | Set a private handler function that gets called whenever a postcondition is not fulfilled. This function must not throw. |
  | setHandlerFailedInvariant | Set a private handler function that gets called whenever the invariant is not fulfilled. this function must no throw. |




Author 
------
  Felix Petriconi (felix at petriconi.net)
  

Contributions
-------------

  Comments, feedback or contributions are welcome!

  
License
-------
  Boost 1.0 License


Version
-------
  0.1

Prerequisites
-------------
  * C++ 11 (partly, as far as Visual Studio 2013 supports it)
  * CMake 2.8 or later
  * GoogleTest 1.7 (Is part of the repository, because it's CMakeFiles.txt needs some patches to compile with Visual Studio)


Platform
--------
| Compiler | Status |
-----------|---------
| Visual Studio 2013 x64 | All tests pass |
| clang 3.6 | All tests pass |

Installation Win
----------------
  * Clone into e.g. D:\misc\contract_light
  * Create a build folder, eg D:\misc\contract_light_build
  * Open a command prompt in that contract_light_build
  * Have CMake in the path
  * Execute cmake -G "Visual Studio 11 Win64" ..\contract_light_build
  * Open created solution in .\alb_build\AllocatorBuilder.sln
  * Compile and run all test

Installation Linux
------------------
  * Clone into e.g. ~/contract_light via "git clone http://github.com/FelixPetriconi/contract_light"
  * Call   mkdir contract_light_build && cd contract_light_build
  * Call   cmake -G "Unix Makefiles" ../contract_light 
  * Build and test with   make && ./test/contract_light_test
  
  
ToDo
----
  to be done 
