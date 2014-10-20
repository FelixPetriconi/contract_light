Contract Light {#mainpage}
==============

A library that offers in a light-weight manner Design by Contract constructs


Motivation
----------
Currently C++ 11/14 does not support natively the concept of [Design by Contract](http://en.wikipedia.org/wiki/Design_by_contract). Several libraries are available that support Design by Contract, but all that I know of use heavily preprocessor makros. So this is my attempt to support this feature with pure C++. 
  
Contract Light Overview
-----------------------
This library supports pre-condition, post-condition and invariants for classes and structs.

~~~
class Rect : public contract_light::Contract
{
  int w_;
  int h_;
public:
  Rect() : w_(0), h_(0) {}
  
  ~Rect() {
    INVARIANT;
  }
  
  void setWidth(int newW) {
    PRECONDITION [&] { return newW >= 0; };
    POSTCONDITION [&, this] { return newW == w_; };
    w_ = newW;
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
  to be done

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


Installation Win
----------------
  * Clone into e.g. D:\misc\contract_light
  * Create a build folder, eg D:\misc\contract_light_build
  * Open a command prompt in that contract_light_build
  * Have CMake in the path
  * Execute cmake -G "Visual Studio 11 Win64" ..\contract_light_build
  * Open created solution in .\alb_build\AllocatorBuilder.sln
  * Compile and run all test
  
ToDo
----
  to be done 


