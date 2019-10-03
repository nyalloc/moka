
cmake_minimum_required(VERSION 3.0)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project(moka CXX)

if (WIN32)
add_compile_options("/std:c++latest")
else()
add_compile_options("-std=c++17")
endif()

enable_testing()

add_subdirectory(engine)
add_subdirectory(examples)