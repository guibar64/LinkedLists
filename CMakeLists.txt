cmake_minimum_required(VERSION 3.16)
project(Linked_lists C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif(NOT CMAKE_BUILD_TYPE)

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall  -Werror -Wno-error=unused-variable  -Wno-error=unused-function")

if(COVERAGE)
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

add_library(linked_lists linked_list.c doubly_linked_list.c)

add_executable(test_linked_lists test_linked_lists.c)
target_link_libraries(test_linked_lists linked_lists cunit)

add_executable(test_doubly_linked_lists test_doubly_linked_lists.c)
target_link_libraries(test_doubly_linked_lists linked_lists cunit)

include(CTest)

add_test(NAME test_linked_lists  COMMAND test_linked_lists)
add_test(NAME test_doubly_linked_lists  COMMAND test_doubly_linked_lists)
