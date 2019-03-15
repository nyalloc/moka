
cmake_minimum_required(VERSION 3.2)

set(MOKA_ASSET_PATH "${CMAKE_CURRENT_LIST_DIR}/assets")
add_definitions(-DMOKA_ASSET_PATH=\"${MOKA_ASSET_PATH}\")

add_subdirectory(hello_triangle)
add_subdirectory(texturing)
add_subdirectory(glTF2_PBR)