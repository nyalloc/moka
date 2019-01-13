
set(CMAKE_CXX_STANDARD 17)

set(moka_INCLUDES
    "./includes/"
    "./deps/nlohmann/"
    "./deps/tiny_gltf/"
    "./deps/"
)

include_directories(${moka_INCLUDES})

set(APPLICATION_SRC
    "includes/application/app_settings.hpp"  
    "includes/application/window.hpp"  
    "includes/application/application.hpp"
    "includes/application/open_file.hpp"
    "includes/application/signal.hpp"
    "includes/application/logger.hpp"
    "includes/application/system_info.hpp"
    "includes/application/timer.hpp"
    "src/application/app_settings.cpp"  
    "src/application/window.cpp"
    "src/application/application.cpp"
    "src/application/application_sdl.cpp" 
    "src/application/open_file.cpp"
    "src/application/system_info.cpp"
    "src/application/timer.cpp"	
)

set(INPUT_SRC
    "includes/input/mouse.hpp"
    "includes/input/keyboard.hpp"	
    "src/input/mouse.cpp"
    "src/input/keyboard.cpp"
)

set(GRAPHICS_API_SRC
    "includes/graphics/api/gl_graphics_api.hpp"
    "includes/graphics/api/graphics_api.hpp"
    "src/graphics/api/gl_graphics_api.cpp"
    "src/graphics/api/graphics_api.cpp"
)

set(GRAPHICS_BUFFER_SRC
    "includes/graphics/buffer/index_buffer.hpp"
    "includes/graphics/buffer/vertex_attribute.hpp"
    "includes/graphics/buffer/vertex_buffer.hpp"
    "includes/graphics/buffer/vertex_layout.hpp"
    "includes/graphics/buffer/vertex_layout_builder.hpp"
    "src/graphics/buffer/index_buffer.cpp"
    "src/graphics/buffer/vertex_attribute.cpp"
    "src/graphics/buffer/vertex_buffer.cpp"
    "src/graphics/buffer/vertex_layout.cpp"
    "src/graphics/buffer/vertex_layout_builder.cpp"
)

set(GRAPHICS_CAMERA_SRC
    "includes/graphics/camera/base_camera.hpp"
    "includes/graphics/camera/basic_camera.hpp"
    "includes/graphics/camera/camera.hpp"
    "includes/graphics/camera/camera_builder.hpp"
    "includes/graphics/camera/camera_decorator.hpp"
    "includes/graphics/camera/camera_mouse_controller.hpp"
    "src/graphics/camera/base_camera.cpp"
    "src/graphics/camera/basic_camera.cpp"
    "src/graphics/camera/camera.cpp"
    "src/graphics/camera/camera_builder.cpp"
    "src/graphics/camera/camera_decorator.cpp"
    "src/graphics/camera/camera_mouse_controller.cpp"
)

set(GRAPHICS_COMMAND_SRC
    "includes/graphics/command/graphics_command.hpp"
    "includes/graphics/command/clear_command.hpp"
    "includes/graphics/command/command_buffer.hpp"
    "includes/graphics/command/command_list.hpp"
    "includes/graphics/command/draw_command.hpp"
    "includes/graphics/command/fill_index_buffer_command.hpp"
    "includes/graphics/command/fill_vertex_buffer_command.hpp"
    "includes/graphics/command/scissor_command.hpp"
    "includes/graphics/command/sort_key.hpp"
    "includes/graphics/command/viewport_command.hpp"
    "src/graphics/command/clear_command.cpp"
    "src/graphics/command/command_buffer.cpp"
    "src/graphics/command/command_list.cpp"
    "src/graphics/command/draw_command.cpp"
    "src/graphics/command/fill_index_buffer_command.cpp"
    "src/graphics/command/fill_vertex_buffer_command.cpp"
    "src/graphics/command/graphics_command.cpp"
    "src/graphics/command/viewport_command.cpp"
    "src/graphics/command/sort_key.cpp"
    "src/graphics/command/scissor_command.cpp"
)

set(GRAPHICS_DEVICE_SRC
    "includes/graphics/device/graphics_device.hpp"
    "includes/graphics/device/graphics_visitor.hpp"
    "src/graphics/device/graphics_device.cpp"
    "src/graphics/device/graphics_visitor.cpp"
)

set(GRAPHICS_MATERIAL_SRC
    "includes/graphics/material/material.hpp"
    "includes/graphics/material/material_builder.hpp"
    "includes/graphics/material/material_properties.hpp"
    "src/graphics/material/material.cpp"
    "src/graphics/material/material_builder.cpp"
    "src/graphics/material/material_properties.cpp"
)

set(GRAPHICS_SRC
    ${GRAPHICS_API_SRC}
    ${GRAPHICS_BUFFER_SRC}
    ${GRAPHICS_CAMERA_SRC}
    ${GRAPHICS_COMMAND_SRC}
    ${GRAPHICS_DEVICE_SRC}
    ${GRAPHICS_MATERIAL_SRC}

    "includes/graphics/colour.hpp"
    "includes/graphics/model.hpp"
    "includes/graphics/program.hpp"
    "includes/graphics/shader.hpp"
    "includes/graphics/texture.hpp"
    "includes/graphics/transform.hpp"
    "includes/graphics/utilities.hpp"

    "src/graphics/colour.cpp"
    "src/graphics/model.cpp"
    "src/graphics/program.cpp"
    "src/graphics/shader.cpp"
    "src/graphics/texture.cpp"
    "src/graphics/transform.cpp"
    "src/graphics/utilities.cpp"
)

set(TRAITS_SRC
    "includes/type_traits/is_any_of.hpp"
    "includes/type_traits/is_either.hpp"
    "includes/type_traits/size.hpp"
)

set(ASSET_SRC
    "includes/asset_importer/asset_importer.hpp" 
    "includes/asset_importer/texture_importer.hpp" 
    "includes/asset_importer/model_importer.hpp"
    "src/asset_importer/texture_importer.cpp"
    "src/asset_importer/asset_importer.cpp" 
    "src/asset_importer/model_importer.cpp"
)

set(MOKA_SRC
    ${APPLICATION_SRC}
    ${INPUT_SRC}
	${GRAPHICS_SRC}
    ${TRAITS_SRC}
    ${MATHS_SRC}
    ${ASSET_SRC}
)

add_library(moka STATIC ${MOKA_SRC})

add_compile_options("/std:c++latest")

foreach(_source IN ITEMS ${MOKA_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

set(PLATFORM_SPECIFIC_LIBS
    comctl32.lib
    Propsys.lib
    Shlwapi.lib
)

target_link_libraries(moka
    glew::glew
    glm
    SDL2::SDL2 
    spdlog::spdlog
    ${PLATFORM_SPECIFIC_LIBS}
)

set(MOKA_TESTS
    "src/tests/unit_tests_main.cpp"
    "src/tests/buffer_tests.cpp"
)

add_executable(moka_tests ${MOKA_TESTS})

target_link_libraries(moka_tests moka)

add_test(buffer_tests moka_tests [buffer])