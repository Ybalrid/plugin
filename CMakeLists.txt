cmake_minimum_required(VERSION 3.14)
project(yba_plugin)
set(CMAKE_CXX_STANDARD 17)

file(GLOB test_src CONFIGURE_DEPENDS 
  ${CMAKE_SOURCE_DIR}/test/* 
  ${CMAKE_SOURCE_DIR}/include/plugin/* 
  ${CMAKE_SOURCE_DIR}/test_interface/*)
add_library(test SHARED ${test_src})
target_include_directories(test PRIVATE 
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/test_interface)

file(GLOB test_host_src CONFIGURE_DEPENDS 
  ${CMAKE_SOURCE_DIR}/test_host/* 
  ${CMAKE_SOURCE_DIR}/include/plugin/* 
  ${CMAKE_SOURCE_DIR}/test_interface/*)
add_executable(test_host ${test_host_src})
target_include_directories(test_host PRIVATE 
  ${CMAKE_SOURCE_DIR}/include 
  ${CMAKE_SOURCE_DIR}/test_interface)

# ensure that the DLL is built when building the executable only:
add_dependencies(test_host test)

if(UNIX)
	target_link_libraries(test_host PRIVATE dl) # link to the dynamic loader
endif()
