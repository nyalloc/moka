
cmake_minimum_required(VERSION 3.0)

include(cmake/HunterGate.cmake)

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.230.tar.gz"
    SHA1 "f48b033e80f16890c5c450d46177f60b475469e4"
)

project(moka CXX)

cmake_policy(SET CMP0074 OLD)

if (WIN32)
add_compile_options("/std:c++latest")
else()
add_compile_options("-std=c++17")
endif()

enable_testing()

add_subdirectory(engine)
add_subdirectory(examples)
