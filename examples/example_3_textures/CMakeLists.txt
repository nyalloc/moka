cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

project(example_3_textures)

set(Example_Includes 
	"./"
	"../../engine/src/includes"
)

include_directories(${Example_Includes})

add_executable(example_3_textures main.cpp)

add_compile_options("/std:c++latest")

foreach(_source IN ITEMS ${MOKA_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

set_target_properties(moka PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(example_3_textures PUBLIC moka)