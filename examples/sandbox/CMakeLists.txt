project(sandbox)

# import other libraries
add_library(mse STATIC IMPORTED)
set_target_properties(mse PROPERTIES IMPORTED_LOCATION ${MSE_BASE_SOURCE_DIR}/build/libMini_Serpent_Engine.a)

add_executable(sandbox
    sandbox.cpp)
target_compile_options(sandbox PRIVATE ${GCC_GLOBAL_COMPILER_SETTINGS})
target_link_libraries(sandbox PUBLIC
    mse
    sdl3
    sdl3_image
    sdl3_mixer
    sdl3_ttf
    yaml-cpp
    )
target_link_libraries(sandbox PRIVATE -lpthread ${GCC_GLOBAL_LINKER_SETTINGS})

install(TARGETS sandbox
        RUNTIME DESTINATION ${MSE_BASE_SOURCE_DIR}/out/examples)
