project(gui-simple)

# import other libraries
add_library(mse STATIC IMPORTED)
set_target_properties(mse PROPERTIES IMPORTED_LOCATION ${MSE_BASE_SOURCE_DIR}/build/libMini_Serpent_Engine.a)

add_executable(gui-simple
    gui-simple.cpp)
target_compile_options(gui-simple PRIVATE ${GCC_GLOBAL_COMPILER_SETTINGS})
target_link_libraries(gui-simple PUBLIC
    mse
    sdl3
    sdl3_image
    sdl3_mixer
    sdl3_ttf
    yaml-cpp
    )
target_link_libraries(gui-simple PRIVATE -lpthread ${GCC_GLOBAL_LINKER_SETTINGS})

install(TARGETS gui-simple
        RUNTIME DESTINATION ${MSE_BASE_SOURCE_DIR}/out/examples)
