cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# import other libraries
add_library(mse STATIC IMPORTED)
set_target_properties(mse PROPERTIES IMPORTED_LOCATION ${MSE_BASE_SOURCE_DIR}/build/libMini_Serpent_Engine.a)

add_executable(card-game
    card-game.cpp)
target_include_directories(card-game PUBLIC ${MSE_BASE_SOURCE_DIR}/examples)
target_compile_options(card-game PRIVATE ${GCC_GLOBAL_COMPILER_SETTINGS})
target_link_libraries(card-game PUBLIC
    mse
    sdl3
    sdl3_image
    sdl3_mixer
    sdl3_ttf
    yaml-cpp
    )
target_link_libraries(card-game PRIVATE -lpthread ${GCC_GLOBAL_LINKER_SETTINGS})

install(TARGETS card-game
        RUNTIME DESTINATION ${MSE_BASE_SOURCE_DIR}/out/examples)
