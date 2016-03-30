cmake_minimum_required(VERSION 2.8)
project("WireCAM")

FIND_PACKAGE(OCE REQUIRED)
INCLUDE_DIRECTORIES(${OCE_INCLUDE_DIRS} ./include contrib/jsoncpp/include)

ADD_SUBDIRECTORY(/home/adam/googletest/googletest ${CMAKE_CURRENT_BINARY_DIR}/gtest)

file(GLOB WIRECAMSRC src/*C contrib/*/src/*cpp)
ADD_LIBRARY(LIBWIRECAM ${WIRECAMSRC})
ADD_EXECUTABLE(test test/unittest.C)

# copy geometry files for test
get_target_property(TEST_LOCATION test LOCATION)
get_filename_component(TEST_DIRECTORY ${TEST_LOCATION} DIRECTORY CACHE)
ADD_CUSTOM_TARGET(test_step_files)
FILE(GLOB STEP_FILES ${CMAKE_SOURCE_DIR}/test/*.STEP)
FOREACH(STEP_FILE ${STEP_FILES})
  ADD_CUSTOM_COMMAND(TARGET test_step_files PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${STEP_FILE} ${TEST_DIRECTORY})
ENDFOREACH()

TARGET_LINK_LIBRARIES (test gtest gtest_main ${OCE_LIBRARIES} LIBWIRECAM)
add_dependencies(test test_step_files)

set(CMAKE_CXX_FLAGS -std=c++11)
