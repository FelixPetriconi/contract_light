project(contract_light)

include_directories("${PROJECT_SOURCE_DIR}/../include")

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
endif(WIN32)

set(SOURCE
	contract_light.cpp
)

set(HEADERS
  ../include/contract_light.hpp
  ../include/contract_light_helper.hpp  
)

add_library(contract_light ${SOURCE} ${HEADERS})