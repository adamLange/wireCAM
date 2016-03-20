cmake_minimum_required(VERSION 2.8)
project("WireCAM")

FIND_PACKAGE(OCE REQUIRED)
INCLUDE_DIRECTORIES(${OCE_INCLUDE_DIRS})

ADD_SUBDIRECTORY(/home/adam/googletest/googletest ${CMAKE_CURRENT_BINARY_DIR}/gtest)

#include_directories(src ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

ADD_LIBRARY(TargetSurface TargetSurface.C)
ADD_EXECUTABLE(test unittest.C)
TARGET_LINK_LIBRARIES (test gtest gtest_main ${OCE_LIBRARIES} TargetSurface)
