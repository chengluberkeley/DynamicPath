cmake_minimum_required (VERSION 3.0)
project(DynamicPath)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-std=c++17 -O3")

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

file(GLOB lib_srcs
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_library(dynamic_path STATIC ${lib_srcs})

add_executable(test_main ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(test_main PRIVATE dynamic_path)
