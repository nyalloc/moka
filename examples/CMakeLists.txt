
cmake_minimum_required(VERSION 3.2)

set(ASSET_PATH "${CMAKE_CURRENT_LIST_DIR}/assets")
add_definitions(-DASSET_PATH=\"${ASSET_PATH}\")

add_subdirectory(example_1_triangle)
add_subdirectory(example_2_indexing)
add_subdirectory(example_3_textures)