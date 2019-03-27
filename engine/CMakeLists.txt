
cmake_minimum_required(VERSION 3.2)

project(moka)

cmake_policy(SET CMP0074 OLD)

# Include Catch
hunter_add_package(Catch)
find_package(Catch CONFIG REQUIRED)

# Include Glew
hunter_add_package(glew)
find_package(glew CONFIG REQUIRED)

# Include glm
hunter_add_package(glm)
find_package(glm CONFIG REQUIRED)

# Include SDL
hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)

# Include spdlog
hunter_add_package(spdlog)
find_package(spdlog CONFIG REQUIRED)

enable_testing()

set(CMAKE_CXX_STANDARD 17)

set(MOKA_INCLUDES
    "./includes/"
    "./deps/imgui/"
    "./deps/mikktspace/"
    "./deps/nlohmann/"
    "./deps/stb/"
    "./deps/tiny_gltf/"
    "./deps/"
)

include_directories(${MOKA_INCLUDES})

set(IMGUI_SRC
    "deps/imgui/imgui.h"
    "deps/imgui/imgui.cpp"
    "deps/imgui/imgui_demo.cpp"
    "deps/imgui/imgui_widgets.cpp"
    "deps/imgui/imgui_draw.cpp"
)

set(MIKKTSPACE_SRC
    "deps/mikktspace/mikktspace.h"
    "deps/mikktspace/mikktspace.c"
)

set(APPLICATION_SRC
    "includes/application/app_settings.hpp"  
    "includes/application/window.hpp"  
    "includes/application/application.hpp"
    "includes/application/signal.hpp"
    "includes/application/logger.hpp"
    "includes/application/timer.hpp"
    "src/application/app_settings.cpp"  
    "src/application/window.cpp"
    "src/application/application.cpp"
    "src/application/application_sdl.cpp" 
    "src/application/timer.cpp"	
)

set(INPUT_SRC
    "includes/input/mouse.hpp"
    "includes/input/keyboard.hpp"	
    "src/input/mouse.cpp"
    "src/input/keyboard.cpp"
)

set(GRAPHICS_API_SRC
    "includes/graphics/pbr_scene.hpp"
    "includes/graphics/pbr.hpp"
	"includes/graphics/pbr_constants.hpp"
	"src/graphics/pbr.cpp"
    "includes/graphics/context.hpp"
    "includes/graphics/api/gl_graphics_api.hpp"
    "includes/graphics/api/graphics_api.hpp"
    "src/graphics/api/gl_graphics_api.cpp"
    "src/graphics/api/graphics_api.cpp"
)

set(GRAPHICS_BUFFER_SRC
    "includes/graphics/buffer/frame_buffer_handle.hpp"
    "includes/graphics/buffer/index_buffer_handle.hpp"
    "includes/graphics/buffer/vertex_attribute.hpp"
    "includes/graphics/buffer/vertex_buffer_handle.hpp"
    "includes/graphics/buffer/vertex_layout.hpp"
    "includes/graphics/buffer/vertex_layout_builder.hpp"
    "src/graphics/buffer/frame_buffer_handle.cpp"
    "src/graphics/buffer/index_buffer_handle.cpp"
    "src/graphics/buffer/vertex_attribute.cpp"
    "src/graphics/buffer/vertex_buffer_handle.cpp"
    "src/graphics/buffer/vertex_layout.cpp"
    "src/graphics/buffer/vertex_layout_builder.cpp"
)

set(GRAPHICS_CAMERA_SRC
    "includes/graphics/camera/basic_camera.hpp"
    "includes/graphics/camera/camera_mouse_controller.hpp"
    "src/graphics/camera/basic_camera.cpp"
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
    "includes/graphics/command/frame_buffer_command.hpp"
    "includes/graphics/command/frame_buffer_texture_command.hpp"
    "includes/graphics/command/generate_mipmaps_command.hpp"
    "includes/graphics/command/scissor_command.hpp"
	"includes/graphics/command/set_material_properties_command.hpp"
    "includes/graphics/command/viewport_command.hpp"
    "src/graphics/command/clear_command.cpp"
    "src/graphics/command/command_buffer.cpp"
    "src/graphics/command/command_list.cpp"
    "src/graphics/command/draw_command.cpp"
    "src/graphics/command/fill_index_buffer_command.cpp"
    "src/graphics/command/fill_vertex_buffer_command.cpp"
    "src/graphics/command/frame_buffer_command.cpp"
    "src/graphics/command/frame_buffer_texture_command.cpp"
    "src/graphics/command/generate_mipmaps_command.cpp"
    "src/graphics/command/graphics_command.cpp"
    "src/graphics/command/viewport_command.cpp"
	"src/graphics/command/set_material_properties_command.cpp"
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
    "includes/graphics/material/material_parameter.hpp"
    "includes/graphics/material/material_builder.hpp"
    "includes/graphics/material/material_properties.hpp"
    "includes/graphics/material/parameter_collection.hpp"
    "src/graphics/material/material.cpp"
    "src/graphics/material/material_parameter.cpp"
    "src/graphics/material/material_builder.cpp"
    "src/graphics/material/material_properties.cpp"
    "src/graphics/material/parameter_collection.cpp"
)

set(GRAPHICS_SRC
    ${GRAPHICS_API_SRC}
    ${GRAPHICS_BUFFER_SRC}
    ${GRAPHICS_CAMERA_SRC}
    ${GRAPHICS_COMMAND_SRC}
    ${GRAPHICS_DEVICE_SRC}
    ${GRAPHICS_MATERIAL_SRC}

    "includes/graphics/colour.hpp"
    "includes/graphics/color.hpp"
    "includes/graphics/model.hpp"
    "includes/graphics/program.hpp"
    "includes/graphics/shader.hpp"
    "includes/graphics/default_shaders.hpp"
    "includes/graphics/texture_handle.hpp"
    "includes/graphics/transform.hpp"
    "includes/graphics/utilities.hpp"

    "src/graphics/colour.cpp"
    "src/graphics/model.cpp"
    "src/graphics/program.cpp"
    "src/graphics/shader.cpp"
    "src/graphics/default_shaders.cpp"
    "src/graphics/texture_handle.cpp"
    "src/graphics/transform.cpp"
    "src/graphics/utilities.cpp"
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
    ${IMGUI_SRC}
    ${MIKKTSPACE_SRC}
    ${APPLICATION_SRC}
    ${INPUT_SRC} 
    ${GRAPHICS_SRC}
    ${MATHS_SRC}
    ${ASSET_SRC}
)

add_library(moka STATIC ${MOKA_SRC})

set_target_properties(moka PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_features(moka PRIVATE cxx_std_17)

foreach(_source IN ITEMS ${MOKA_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

if(WIN32)
set(PLATFORM_SPECIFIC_LIBS
    comctl32.lib
    Propsys.lib
    Shlwapi.lib
)
else()
set(PLATFORM_SPECIFIC_LIBS
    stdc++fs
)
endif()

target_link_libraries(moka
    glew::glew
    glm
    SDL2::SDL2 
    spdlog::spdlog
    ${PLATFORM_SPECIFIC_LIBS}
)