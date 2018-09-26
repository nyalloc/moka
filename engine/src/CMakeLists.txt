
set(moka_INCLUDES
    "./includes/"
    "./deps/imgui/"
    "./deps/tiny_gltf/"
    "./deps/"
)

include_directories(${moka_INCLUDES})

set(MOKA_SRC 
    "src/application/window.cpp"
    "src/application/application.cpp"
    "src/application/application_sdl.cpp"
	"src/application/system_info.cpp"
	"includes/application/mvp/model.hpp"
    "includes/application/application.hpp"
    "includes/application/signal.hpp"
	"includes/application/window.hpp"
	"includes/application/logger.hpp"
    "includes/application/system_info.hpp"
    "includes/application/game_loop.hpp"
    "includes/application/timer.hpp"

    "src/graphics/gl_graphics_api.cpp"
    "src/graphics/graphics_device.cpp"
    "src/graphics/draw_call_builder.cpp"
	"includes/graphics/buffer.hpp"
	"includes/graphics/graphics_device.hpp"
    "includes/graphics/graphics_api.hpp"
    "includes/graphics/gl_graphics_api.hpp"
	"includes/graphics/draw_call_builder.hpp"
	"includes/graphics/buffer.hpp"
	"includes/graphics/camera.hpp"
	"includes/graphics/create_program.hpp"
	"includes/graphics/create_shader.hpp"
	"includes/graphics/create_vertex_buffer.hpp"
	"includes/graphics/draw_call.hpp"
	"includes/graphics/frame.hpp"
	"includes/graphics/sort_key.hpp"
	"includes/graphics/uniform_buffer.hpp"

    "deps/imgui/imgui.cpp"
    "deps/imgui/imgui_draw.cpp"
    "deps/imgui/imgui_impl_sdl_gl3.cpp"
    "deps/imgui/imgui_demo.cpp"
	
	"includes/messaging/base_dispatcher.hpp"
	"includes/messaging/basic_dispatcher.hpp"
	"includes/messaging/blocking_policy.hpp"
	"includes/messaging/chained_dispatcher.hpp"
	"includes/messaging/close_queue.hpp"
	"includes/messaging/dispatcher.hpp"
	"includes/messaging/message_base.hpp"
	"includes/messaging/message_queue.hpp"
    "includes/messaging/receiver.hpp"

    "includes/type_traits/is_any_of.hpp"
    "includes/type_traits/is_either.hpp"
    "includes/type_traits/size.hpp"

    "includes/maths/utilities.hpp"
    "includes/maths/colour.hpp"
    "includes/maths/matrix4.hpp"
    "includes/maths/matrix3.hpp"
    "includes/maths/vector2.hpp"
    "includes/maths/vector3.hpp"
    "includes/maths/vector4.hpp"
	
	"includes/asset_importer/texture_import.hpp"
	"includes/asset_importer/model_import.hpp"
    "src/asset_importer/texture_import.cpp"
    "src/asset_importer/model_import.cpp"
)

add_library(moka STATIC ${MOKA_SRC})

add_compile_options("/std:c++latest")

foreach(_source IN ITEMS ${MOKA_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

target_link_libraries(moka
    glew::glew
	glm
    SDL2::SDL2
	spdlog::spdlog
	Assimp::assimp
)

set(MOKA_TESTS
  "src/tests/unit_tests_main.cpp"
  "src/tests/buffer_tests.cpp"
)

add_executable(moka_tests ${MOKA_TESTS})

target_link_libraries(moka_tests moka)

add_test(buffer_tests moka_tests [buffer])