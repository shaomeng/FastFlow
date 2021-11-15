# Tutorial: https://cmake.org/cmake/help/latest/guide/tutorial/index.html

cmake_minimum_required(VERSION 3.10)

project(FastFlow VERSION 0.1 DESCRIPTION "Flow Integration Run Fast")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
endif()

#
# Gather git commit SHA1
#
find_package(Git)
execute_process(COMMAND
  ${GIT_EXECUTABLE} describe --always --dirty
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_SHA1
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(COMMAND
  ${GIT_EXECUTABLE} branch --show-current
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Repository Git Branch: ${GIT_BRANCH}, SHA1: ${GIT_SHA1}")

#
# configure a header file to pass some of the CMake settings to the source code
#
configure_file("${CMAKE_SOURCE_DIR}/FastFlowConfig.h.in" FastFlowConfig.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#
# Download dependency: GLM
#
set( GLM_GZ "glm-0.9.9.8.tar.gz" )
if( NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${GLM_GZ} )
  message(STATUS "Downloading GLM...")
  file(DOWNLOAD https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.tar.gz
                ${CMAKE_CURRENT_BINARY_DIR}/${GLM_GZ} )
  execute_process(
    COMMAND rm -rf glm-0.9.9.8
    COMMAND tar xf ${GLM_GZ}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
else()
  message(STATUS "Using cached GLM...")
endif()
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/glm-0.9.9.8/glm/include )

#
# Download CLI11
#
if( NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/CLI11.hpp )
    file(DOWNLOAD https://github.com/CLIUtils/CLI11/releases/latest/download/CLI11.hpp
                  ${CMAKE_CURRENT_BINARY_DIR}/CLI11.hpp )
endif()

#
# Compile the SPERR library
#
add_subdirectory( src )

#
# Create a list of libraries that can be liked by apps
#
#list( APPEND LINK_LIST SPERR )
#list( APPEND LINK_LIST OpenMP::OpenMP_CXX )

#add_subdirectory( utilities ${CMAKE_BINARY_DIR}/bin )

