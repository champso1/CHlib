cmake_minimum_required(VERSION 3.20)
project(example)


find_package(CHlib REQUIRED)

# grab the name of the example to build from the user
set(BUILD_EXAMPLE "None" CACHE STRING "The name of the example file to build")

if(${BUILD_EXAMPLE} STREQUAL "None")
  set(BUILD_EXAMPLE "example1" CACHE STRING "The name of the example file to build" FORCE)
endif()

message("** Building ${BUILD_EXAMPLE}.c ...")

add_executable(example ${CMAKE_CURRENT_SOURCE_DIR}/${BUILD_EXAMPLE}.c)
target_link_libraries(example PRIVATE CHlib::CHlib)

