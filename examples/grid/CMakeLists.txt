# make the executable, link with candia, and use the original (not installed) include dir
add_executable(grid grid.c)
target_include_directories(grid PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_directories(grid PRIVATE c:/msys64/ucrt64/lib)
target_link_libraries(grid PRIVATE CHlib glfw3)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})