#
# Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
#
# SPDX-License-Identifier: MIT
#

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project (nederrock CXX)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

include_directories(./)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

include(CTest)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
