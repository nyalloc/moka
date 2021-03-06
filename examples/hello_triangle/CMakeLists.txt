cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

project(hello_triangle CXX)

set(Example_Includes 
	"./"
	"../../engine/includes"
)

include_directories(${Example_Includes})

add_executable(hello_triangle main.cpp)

if (WIN32)
    add_compile_options("/std:c++latest")
    foreach(_source IN ITEMS ${MOKA_SRC})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
else()
    add_compile_options("-std=c++17")
endif()

set_target_properties(moka PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(hello_triangle PUBLIC moka)
