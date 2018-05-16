
cmake_minimum_required(VERSION 3.2)

include(cmake/HunterGate.cmake)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.230.tar.gz"
    SHA1 "f48b033e80f16890c5c450d46177f60b475469e4"
)

project(loki)

set(CMAKE_POLICY_DEFAULT_CMP0054 NEW)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include Catch
hunter_add_package(Catch)
find_package(Catch CONFIG REQUIRED)

# Include Glew
hunter_add_package(glew)
find_package(glew CONFIG REQUIRED)

# Include SDL
hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)

enable_testing()

add_subdirectory(src)