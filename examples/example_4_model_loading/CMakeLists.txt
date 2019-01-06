cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

project(example_4_model_loading)

set(Example_Includes 
	"./"
	"../../engine/src/includes"
    "../../engine/src/deps/imgui"
    "../common"
)

include_directories(${Example_Includes})

add_executable(example_4_model_loading
    main.cpp 
    ../common/imgui.hpp 
    ../common/imgui.cpp
    ../../engine/src/deps/imgui/imgui.h
    ../../engine/src/deps/imgui/imgui.cpp
    ../../engine/src/deps/imgui/imgui_demo.cpp
    ../../engine/src/deps/imgui/imgui_widgets.cpp
    ../../engine/src/deps/imgui/imgui_draw.cpp)

add_compile_options("/std:c++latest")

foreach(_source IN ITEMS ${MOKA_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

set_target_properties(moka PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(example_4_model_loading PUBLIC moka)