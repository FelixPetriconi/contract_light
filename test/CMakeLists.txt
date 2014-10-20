project(contract_light_test)

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
ENDIF(WIN32)

# Visual Studio 2012 only supports up to 8 template parameters in
# std::tr1::tuple by default, but gtest requires 10
if(MSVC AND MSVC_VERSION EQUAL 1700)
  add_definitions(-D_VARIADIC_MAX=10)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")  
  set(CMAKE_CXX_FLAGS "-O3 -Wall -std=c++11 -fcxx-exceptions")
endif()

include_directories("${PROJECT_SOURCE_DIR}/../include")
include_directories("${PROJECT_SOURCE_DIR}/../tools/gtest-1.7.0/include")

set(HEADERS
)

set(SOURCE
  contract_light_test.cpp
  main.cpp
)

add_executable(contract_light_test ${SOURCE} ${HEADERS})

add_dependencies(contract_light_test gtest)
add_dependencies(contract_light_test contract_light)
target_link_libraries(contract_light_test gtest contract_light)
