
cmake_minimum_required(VERSION 3.2)

project(neon)

set(CMAKE_POLICY_DEFAULT_CMP0054 NEW)

set(CMAKE_CXX_STANDARD 17)
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