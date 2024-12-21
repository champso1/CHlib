cmake_minimum_required(VERSION 3.20)
project(chlib_example2)

find_package(CHlib REQUIRED)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/example2.c)
target_link_libraries(main PRIVATE CHlib::CHlib)

