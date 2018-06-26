cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

project(neon_example_cube)

set(Example_Includes 
	"./"
	"../../engine/src/includes"
)

include_directories(${Example_Includes})

add_executable(neon_example_cube main.cpp)

foreach(_source IN ITEMS ${NEON_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

set_target_properties(neon PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(neon_example_cube PUBLIC neon)