
cmake_minimum_required(VERSION 3.2)

set(MOKA_ASSET_PATH "${CMAKE_CURRENT_LIST_DIR}/assets")
add_definitions(-DMOKA_ASSET_PATH=\"${MOKA_ASSET_PATH}\")

add_subdirectory(example_1_triangle)
add_subdirectory(example_2_textures)
add_subdirectory(example_3_cube)
add_subdirectory(example_4_lighting)