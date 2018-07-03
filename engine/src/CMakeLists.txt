
set(moka_INCLUDES
    "./includes/"
    "./deps/imgui/"
    "./deps/"
)

include_directories(${moka_INCLUDES})

set(MOKA_SRC 
    "src/main.cpp"
    "src/window/window.cpp"
    "src/application/application.cpp"
    "src/application/application_sdl.cpp"
    "src/gl_graphics_api_impl.cpp"
    "src/graphics_api.cpp"
    "deps/imgui/imgui.cpp"
    "deps/imgui/imgui_draw.cpp"
    "deps/imgui/imgui_impl_sdl_gl3.cpp"
    "deps/imgui/imgui_demo.cpp"
	"src/system_info.cpp"
	"src/logger/logger.cpp"
    "includes/application/mvp/model.hpp"
	"includes/logger/logger.hpp"
    "includes/system_info.hpp"
    "includes/graphics_api.hpp"
    "includes/graphics_api_impl.hpp"
    "includes/gl_graphics_api_impl.hpp"
    "includes/game_loop/game_loop.hpp"
    "includes/type_traits/is_any_of.hpp"
    "includes/type_traits/is_either.hpp"
	"includes/safe_queue.hpp"
    "includes/type_traits/size.hpp"
    "includes/application/application.hpp"
    "includes/application/signal.hpp"
    "includes/maths/utilities.hpp"
    "includes/maths/colour.hpp"
    "includes/maths/matrix.hpp"
    "includes/maths/vector2.hpp"
    "includes/maths/vector3.hpp"
    "includes/maths/vector4.hpp"
    "includes/window/window.hpp"
)

add_library(moka STATIC ${MOKA_SRC})

foreach(_source IN ITEMS ${MOKA_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

target_link_libraries(moka
    glew::glew
    SDL2::SDL2
)