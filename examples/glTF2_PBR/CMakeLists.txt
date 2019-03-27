cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

project(glTF2_PBR)

set(EXAMPLE_INCLUDES   
    "./" 
    "../../engine/includes"
    "../../engine/deps/imgui"
    "../common"
)

include_directories(${EXAMPLE_INCLUDES})

add_executable(glTF2_PBR
    main.cpp 
    ../common/imgui.hpp 
    ../common/imgui.cpp
)

set_target_properties(glTF2_PBR PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_features(glTF2_PBR PRIVATE cxx_std_17)

foreach(_source IN ITEMS ${MOKA_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

set_target_properties(moka PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(glTF2_PBR PUBLIC moka)