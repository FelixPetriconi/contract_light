cmake_minimum_required(VERSION 2.8)

project(cl)

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
ENDIF(WIN32)

# Visual Studio 2012 only supports up to 8 template parameters in
# std::tr1::tuple by default, but gtest requires 10
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" AND MSVC_VERSION EQUAL 1700)
  add_definitions(-D_VARIADIC_MAX=10)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")  
  set(CMAKE_CXX_FLAGS "-O3 -Wall -std=c++11")
endif()

add_subdirectory(tools/gtest-1.7.0)
add_subdirectory(source)
add_subdirectory(test)

