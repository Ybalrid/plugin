cmake_minimum_required(VERSION 3.14)
project(yplugs)
set(CMAKE_CXX_STANDARD 17)

file(GLOB test_src CONFIGURE_DEPENDS ./test/* ./plugin/* ./test_interface/*)
add_library(test SHARED ${test_src})
target_include_directories(test PRIVATE ./)

file(GLOB test_host_src CONFIGURE_DEPENDS ./test_host/* ./plugin/* ./test_interface/*)
add_executable(test_host ${test_host_src})
target_include_directories(test_host PRIVATE ./)
if(UNIX)
	target_link_libraries(test_host PRIVATE dl) # link to the dynamic loader
endif()
