add_library(sdlgame sdlgame.c)
target_link_libraries(sdlgame PUBLIC SDL2 SDL2_image SDL2_ttf)

add_executable(bees main.c)
target_link_libraries(bees sdlgame)

