
cmake_minimum_required(VERSION 3.2)

project(moka)

cmake_policy(SET CMP0074 OLD)

# Include Catch
hunter_add_package(Catch)
find_package(Catch CONFIG REQUIRED)

# Include Glew
hunter_add_package(glew)
find_package(glew CONFIG REQUIRED)

# Include glm
hunter_add_package(glm)
find_package(glm CONFIG REQUIRED)

# Include SDL
hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)

# Include spdlog
hunter_add_package(spdlog)
find_package(spdlog CONFIG REQUIRED)

enable_testing()

add_subdirectory(src)
