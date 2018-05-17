
set(LOKI_INCLUDES
    "./includes/"
    "./deps/imgui/")

include_directories(${LOKI_INCLUDES})

add_executable(
    loki 
    main.cpp
    application/application.cpp
    deps/imgui/imgui.cpp
    deps/imgui/imgui_draw.cpp
    deps/imgui/imgui_impl_sdl_gl3.cpp
    deps/imgui/imgui_demo.cpp
    includes/type_traits/is_any_of.hpp
    includes/type_traits/is_either.hpp
    includes/type_traits/size.hpp
	includes/application/application.hpp
)

target_link_libraries(loki
    glew::glew
    SDL2::SDL2
)