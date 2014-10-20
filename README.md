Contract Light {#mainpage}
==============

A library that offers in a light-weight manner Design by Contract constructs


Motivation
----------
  
Contract Light Overveiw
------------------


Documentation
-------------
  Online Documentation is available on [GitHub.io] (http://felixpetriconi.github.io/contract_light/index.html) as well.
  
  There is the begin of a tutorial in the [tutorial section](http://felixpetriconi.github.io/contract_light/md__t_u_t_o_r_i_a_l.html)

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
| Visual Studio 2012 x64 | All tests pass |
| Visual Studio 2013 x64 | All tests pass |
| Debian x64, Clang 3.4  | All tests pass |

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
  See issue list of [open enhancements] (https://github.com/FelixPetriconi/contract_light/issues?labels=enhancement&page=1&state=open)


