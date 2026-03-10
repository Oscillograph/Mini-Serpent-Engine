cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# import other libraries
add_library(mse STATIC IMPORTED)
set_target_properties(mse PROPERTIES IMPORTED_LOCATION ${MSE_BASE_SOURCE_DIR}/build/libMini_Serpent_Engine.a)

add_executable(lesta-autobattler
    lesta-autobattler.cpp
#    lesta-autobattler/data-loader.h
    lesta-autobattler/data-loader.cpp
#    lesta-autobattler/game-data.h
#    lesta-autobattler/game-fwd.h
    lesta-autobattler/game.cpp
#    lesta-autobattler/gamestates.h
    lesta-autobattler/gamestates.cpp
#    lesta-autobattler/layers.h
    lesta-autobattler/layers.cpp)
target_include_directories(lesta-autobattler PUBLIC ${MSE_BASE_SOURCE_DIR}/examples)
target_compile_options(lesta-autobattler PRIVATE -Wall -Wfatal-errors)
target_link_libraries(lesta-autobattler PUBLIC 
    mse
    sdl3
    sdl3_image
    sdl3_mixer
    sdl3_ttf
    yaml-cpp
    )
target_link_libraries(lesta-autobattler PRIVATE -lpthread "-Wl,-rpath,./bin")
    
add_executable(dungeontris
    dungeontris.cpp
#    dungeontris/data-loader.h
    dungeontris/data-loader.cpp
#    dungeontris/game-data.h
#    dungeontris/game-fwd.h
    dungeontris/game.cpp
#    dungeontris/gamestates.h
    dungeontris/gamestates.cpp
#    dungeontris/layers.h
    dungeontris/layers.cpp)
target_include_directories(dungeontris PUBLIC ${MSE_BASE_SOURCE_DIR}/examples)
target_compile_options(dungeontris PRIVATE -Wall -Wfatal-errors)
target_link_libraries(dungeontris PUBLIC
    mse
    sdl3
    sdl3_image
    sdl3_mixer
    sdl3_ttf
    yaml-cpp
    )
target_link_libraries(dungeontris PRIVATE -lpthread "-Wl,-rpath,./bin")
    
add_executable(chalkboard
    chalkboard.cpp)
target_compile_options(chalkboard PRIVATE -Wall -Wfatal-errors)
target_link_libraries(chalkboard PUBLIC
    mse
    sdl3
    sdl3_image
    sdl3_mixer
    sdl3_ttf
    yaml-cpp
    )
target_link_libraries(chalkboard PRIVATE -lpthread "-Wl,-rpath,./bin")
    
add_executable(proto-snake
    proto-snake.cpp)
target_compile_options(proto-snake PRIVATE -Wall -Wfatal-errors)
target_link_libraries(proto-snake PUBLIC
    mse
    sdl3
    sdl3_image
    sdl3_mixer
    sdl3_ttf
    yaml-cpp
    )
target_link_libraries(proto-snake PRIVATE -lpthread "-Wl,-rpath,./bin")

# actually, chalkboard, protosnake, lesta-autobattler and dungeontris shouldn't share the same /out/lesta-autobattler directory, but I didn't plan ahead
install(TARGETS chalkboard proto-snake lesta-autobattler dungeontris RUNTIME DESTINATION ${MSE_BASE_SOURCE_DIR}/out/lesta-autobattler)