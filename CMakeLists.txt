cmake_minimum_required(VERSION 3.20)
project(CHlib VERSION 0.0.1)


# grab all the source files
file(GLOB_RECURSE sources   src/*.c include/*.h)

# we build a static library.
add_library(CHlib STATIC ${sources})

# we setup the include directories
# since public, meaning other projects that find this package,
# we want to specify that during this build, we use this versions directories,
# but during the building of other stuff, use the final installed versions
target_include_directories(CHlib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
# also supply the library directory
target_link_directories(CHlib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
  $<INSTALL_INTERFACE:lib>
)

# we need to link with libglfw3 and libm(ath)
target_link_libraries(CHlib glfw m)

# gives us CMAKE_INSTALL_<x>DIR
# that is set by CMAKE_INSTALL_PREFIX
include(GNUInstallDirs)

# installs the artifacts, and registers the target
# with an "export set"
# which essentially includes essential project info
install(TARGETS CHlib
  EXPORT CHlibTargets
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# installs the export set as well
install(EXPORT CHlibTargets
  FILE CHlibTargets.cmake
  NAMESPACE CHlib::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CHlib
)

# these commands set up and install Cmake config files
# so that other projects can find this library
# it uses the Cmake.config.in file
# which these commands fill with necessary info
# it also will include the exported targets
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/CHlibConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CHlib
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/CHlibConfig.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CHlib
)


# the first install command only set some metadata up
# it only installs binaries and such
# so we have to manually install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
