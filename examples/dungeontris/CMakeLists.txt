project(dungeontris)

# import other libraries
add_library(mse STATIC IMPORTED)
set_target_properties(mse PROPERTIES IMPORTED_LOCATION ${MSE_BASE_SOURCE_DIR}/build/libMini_Serpent_Engine.a)

add_executable(dungeontris
    data-loader.cpp
    dungeontris.cpp
    game.cpp
    gamestates.cpp
    gui.cpp
    layers.cpp
    layers/arena_ui.cpp
    layers/character_create_ui.cpp
    layers/character_load_ui.cpp
    layers/character_save_ui.cpp
    layers/character_update_ui.cpp
    layers/credits_ui.cpp
    layers/exit_ui.cpp
    layers/gameover_ui.cpp
    layers/highscores_ui.cpp
    layers/intro_ui.cpp
    layers/main_menu_ui.cpp
    layers/settings_ui.cpp
    layers/shop_ui.cpp
    layers/winner_ui.cpp
    tetris.cpp
    )
target_include_directories(dungeontris PUBLIC ${MSE_BASE_SOURCE_DIR}/examples)
target_compile_options(dungeontris PRIVATE ${GCC_GLOBAL_COMPILER_SETTINGS})
target_link_libraries(dungeontris PUBLIC
    mse
    sdl3
    sdl3_image
    sdl3_mixer
    sdl3_ttf
    yaml-cpp
    )
target_link_libraries(dungeontris PRIVATE -lpthread ${GCC_GLOBAL_LINKER_SETTINGS})

install(TARGETS dungeontris
        RUNTIME DESTINATION ${MSE_BASE_SOURCE_DIR}/out/examples)
