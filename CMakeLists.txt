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

# Put libraries an executables into lib and bin directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

###################################
# External packages used
###################################

# For reference: builds 3rd party packages first, then re-runs cmake for
# actual build.
# https://cmake.org/pipermail/cmake/2013-October/056105.html
# https://github.com/OpenChemistry/openchemistry

find_package(PkgConfig)

# Eigen is headers only, so just download it instead of trying to find it on
# the system. Need to use custom download command as curl that comes with Linux
# Mint doesn't support https.
set(EIGEN_FILE 3.2.9.tar.gz)
ExternalProject_Add(Eigen3
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/Eigen3
  DOWNLOAD_COMMAND /usr/bin/wget https://bitbucket.org/eigen/eigen/get/${EIGEN_FILE}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND /bin/tar xzf ${CMAKE_CURRENT_BINARY_DIR}/Eigen3/src/${EIGEN_FILE} -C ${CMAKE_CURRENT_BINARY_DIR}/Eigen3 --strip-components=1
  TEST_COMMAND "")
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Eigen3)

pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
link_directories(${OPENGL_LIBRARIES})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIBRARIES})

set(XERCESC_VERSION 3.1.4)
ExternalProject_Add(xerces-c
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/xerces-c
  URL http://apache.mirrors.pair.com/xerces/c/3/sources/xerces-c-${XERCESC_VERSION}.tar.bz2
  URL_MD5 d04ae9d8b2dee2157c6db95fa908abfd
  CONFIGURE_COMMAND ../xerces-c/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/xerces-c --disable-network --disable-static
  BUILD_COMMAND $(MAKE) --directory=src --silent
  TEST_COMMAND "")

include_directories(${CMAKE_CURRENT_BINARY_DIR}/xerces-c/include)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/xerces-c/libs)



###################################
# Sub-directories for building
###################################

add_subdirectory(Control/src)
add_subdirectory(Engine/src)
add_subdirectory(Plugins/BasicPlugin/src)
add_subdirectory(Main/src)
