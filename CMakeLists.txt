cmake_minimum_required (VERSION 3.0)
project(DynamicPath)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-std=c++11 -O3")

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

file(GLOB lib_srcs
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_library(dynamic_path STATIC ${lib_srcs})
