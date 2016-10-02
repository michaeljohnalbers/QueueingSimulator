# Author: Michael Albers
# Description: Top level CMake file for all of Queueing Simulator

cmake_minimum_required(VERSION 3.5)
project(QueueingSimulator VERSION 0.01 LANGUAGES CXX)

include(ExternalProject)

###################################
# General Settings
###################################

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
      "Choose the type of build, options are: Debug Release
RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Generate version header file.
configure_file (
  "${PROJECT_SOURCE_DIR}/Common/inc/QSConfig.h.in"
  "${PROJECT_BINARY_DIR}/Common/inc/QSConfig.h")
include_directories("${PROJECT_BINARY_DIR}/Common/inc/")
include_directories("${PROJECT_SOURCE_DIR}/Common/inc/")

###################################
# Compiler Settings
###################################

# Yes, it sort of defeats the purpose to only support gcc. But the use of CMake
# here is more to try it out and use it's ability to find/download 3rd party
# packages.

# Global compiler options
# -Wextra produces too much output, Eigen uses deprecated features, so turn off
# that warning
set(CMAKE_CXX_FLAGS "--std=c++11 -Wall -Wno-deprecated")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O")
SET(CMAKE_EXE_LINKER_FLAGS "-pthread -rdynamic")

###################################
# Installation Settings
###################################

# TODO: change this at some point
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
set(QS_INSTALL_BIN_DIR bin)
set(QS_INSTALL_LIB_DIR lib)
set(QS_INSTALL_PLUGIN_BASE_DIR plugins)
set(QS_INSTALL_SIMULATIONS_BASE_DIR simulations)

###################################
# External packages used
###################################

# For reference: builds 3rd party packages first, then re-runs cmake for
# actual build.
# https://cmake.org/pipermail/cmake/2013-October/056105.html
# https://github.com/OpenChemistry/openchemistry

find_package(PkgConfig)

# If problems downloading using https see:
# http://stackoverflow.com/questions/29816529/unsupported-protocol-while-downlod-tar-gz-package

# Eigen is headers only, so just download it instead of trying to find it on
# the system. Build/test/etc. aren't needed either.
set(EIGEN_FILE 3.2.9.tar.gz)
ExternalProject_Add(Eigen3
  PREFIX Eigen3
  URL https://bitbucket.org/eigen/eigen/get/${EIGEN_FILE}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND "")
ExternalProject_Get_Property(Eigen3 source_dir binary_dir)
set(EIGEN3_INCLUDE_DIR ${source_dir})
include_directories(${EIGEN3_INCLUDE_DIR})

ExternalProject_Add(GLM
  PREFIX GLM
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG 0.9.8.0
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "")
ExternalProject_Get_Property(GLM source_dir)
set(GLM_INCLUDE_DIR ${source_dir})

set(GLFW_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/GLFW)
ExternalProject_Add(GLFW
  PREFIX GLFW
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG 3.2.1
  CMAKE_ARGS -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF
             -DBUILD_SHARED_LIBS=ON
             -DCMAKE_INSTALL_PREFIX=${GLFW_INSTALL_DIR})
set(GLFW_INCLUDE_DIR ${GLFW_INSTALL_DIR}/include)
set(GLFW_LIBRARY_DIR ${GLFW_INSTALL_DIR}/lib)
set(GLFW_LIBRARIES "-lglfw")
link_directories(${GLFW_LIBRARY_DIR})
# Use GLFW shared object library to avoid X11 dependencies needed at compile
# time when using .a.
# See http://stackoverflow.com/questions/24345384/glfw-linker-errors-on-ubuntu-14-04-lts
install(DIRECTORY ${GLFW_LIBRARY_DIR}/ DESTINATION ${QS_INSTALL_LIB_DIR}
        FILES_MATCHING REGEX "libglfw.*"
	REGEX "cmake" EXCLUDE
	REGEX "pkg" EXCLUDE)

find_package(GLEW REQUIRED)
link_directories(${FLEW_LIBRARY_DIRS})

pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

find_package(OpenGL REQUIRED)
link_directories(${OPENGL_LIBRARIES})

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" OR
    "${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
# Taken from:
# http://stackoverflow.com/questions/8507723/how-to-start-working-with-gtest-and-cmake
  ExternalProject_Add(gtest
    URL https://github.com/google/googletest/archive/release-1.7.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    INSTALL_COMMAND ""
    )
  ExternalProject_Get_Property(gtest source_dir binary_dir)
  set(GTEST_INCLUDE_DIR ${source_dir}/include)
  set(GTEST_LIBRARY_DIR ${binary_dir})
  set(GTEST_LIBRARY "${GTEST_LIBRARY_DIR}/libgtest.a")
endif()

# The build/install commands both use the --directory=src option to skip
# building the example binaries which just aren't needed.
set(XERCES_DIR ${CMAKE_CURRENT_BINARY_DIR}/xerces-c)
ExternalProject_Add(xerces-c
  PREFIX xerces-c
  URL http://apache.mirrors.pair.com/xerces/c/3/sources/xerces-c-3.1.4.tar.bz2
  URL_MD5 d04ae9d8b2dee2157c6db95fa908abfd
  CONFIGURE_COMMAND ../xerces-c/configure --prefix=${XERCES_DIR} --disable-network --disable-static --enable-silent-rules
  BUILD_COMMAND $(MAKE) --directory=src --silent
  INSTALL_COMMAND $(MAKE) --directory=src install --silent
  TEST_COMMAND "")
set(XERCESC_INCLUDE_DIR ${XERCES_DIR}/include)
set(XERCESC_LIBRARY_DIR ${XERCES_DIR}/lib)
set(XERCESC_LIBRARY "-lxerces-c")
include_directories(${XERCESC_INCLUDE_DIR})
link_directories(${XERCESC_LIBRARY_DIR})
install(DIRECTORY ${XERCESC_LIBRARY_DIR}/ DESTINATION ${QS_INSTALL_LIB_DIR}
        FILES_MATCHING REGEX "libxerces-c.*.so")

###################################
# Sub-directories for building
###################################

add_subdirectory(Control/src)
add_subdirectory(Engine/src)
add_subdirectory(Visualization/src)
add_subdirectory(Main/src)

include(Plugins/Plugins.cmake)
include(Simulations/Simulations.cmake)

###################################
# Tests
# Can either run
#  - make test
#  - ctest
###################################
# To see output on failures:
#   setenv CTEST_OUTPUT_ON_FAILURE 1
# then run tests
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" OR
    "${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")

  enable_testing()

  list(APPEND testDirs Plugins/BasicPlugin/test Engine/test Visualization/test)
  list(APPEND tests    BasicPluginTest          EngineTest  VisualizationTest)

  foreach(testDir IN LISTS testDirs)
    add_subdirectory(${testDir})
  endforeach()
  foreach(test IN LISTS tests)
    add_test(NAME ${test} COMMAND ${test})
  endforeach()
endif()
