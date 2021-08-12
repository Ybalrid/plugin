cmake_minimum_required(VERSION 3.14)
project(yba_plugin)
set(CMAKE_CXX_STANDARD 17)

# The following is for compiling the test plugin and test program in the `sample` sub-directory

## Plugin:
file(GLOB test_src CONFIGURE_DEPENDS 
  ${CMAKE_SOURCE_DIR}/sample/test/* 
  ${CMAKE_SOURCE_DIR}/include/plugin/* 
  ${CMAKE_SOURCE_DIR}/sample/test_interface/*)
add_library(test SHARED ${test_src})
target_include_directories(test PRIVATE 
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/sample/test_interface)

## Host program using the plugin:
file(GLOB test_host_src CONFIGURE_DEPENDS 
  ${CMAKE_SOURCE_DIR}/sample/test_host/* 
  ${CMAKE_SOURCE_DIR}/include/plugin/* 
  ${CMAKE_SOURCE_DIR}/sample/test_interface/*)
add_executable(test_host ${test_host_src})
add_dependencies(test_host test)
target_include_directories(test_host PRIVATE 
  ${CMAKE_SOURCE_DIR}/include 
  ${CMAKE_SOURCE_DIR}/sample/test_interface)
if(UNIX)
  target_link_libraries(test_host PRIVATE dl) # -ldl required on UNIX platoforms
endif()
