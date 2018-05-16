
set(LOKI_INCLUDES
    "./includes/")

include_directories(${LOKI_INCLUDES})

add_executable(
    loki 
    main.cpp
    includes/type_traits/is_any_of.hpp
    includes/type_traits/is_either.hpp
    includes/type_traits/size.hpp
)

target_link_libraries(loki
    glew::glew
    SDL2::SDL2
)