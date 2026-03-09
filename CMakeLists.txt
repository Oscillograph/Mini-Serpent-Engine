cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# set the project name
project(Mini_Serpent_Engine)

# add directories
#add_subdirectory(includes)
#add_subdirectory(external)
#add_subdirectory(source build)

# global strings
set(MSE_BASE_SOURCE_DIR ${CMAKE_SOURCE_DIR})

set(MSE_EXAMPLES_DIR 
    "${MSE_BASE_SOURCE_DIR}/examples")

set(MSE_INCLUDES_DIR 
    ${MSE_BASE_SOURCE_DIR}/include
    ${MSE_BASE_SOURCE_DIR}/external
    )
include_directories(${MSE_INCLUDES_DIR})

# build MSE as a static library
file(GLOB_RECURSE MSE_SOURCES 
    ${MSE_BASE_SOURCE_DIR}/source/*.cpp)
file(GLOB_RECURSE EXTERNAL_SOURCES 
    ${MSE_BASE_SOURCE_DIR}/external/*.cpp)

add_library(${PROJECT_NAME} STATIC 
    ${MSE_SOURCES} 
    )
target_include_directories(${PROJECT_NAME} PUBLIC
    ${MSE_INCLUDES_DIR}
    ${MSE_BASE_SOURCE_DIR}/external)
target_compile_options(${PROJECT_NAME} PRIVATE 
	-Wall 
	-Wfatal-errors
	)
	

# link external libraries
add_library(sdl3 STATIC IMPORTED)
set_target_properties(sdl3 PROPERTIES IMPORTED_LOCATION ${MSE_BASE_SOURCE_DIR}/bin/libSDL3.so)
#set_target_properties(sdl3 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${MSE_BASE_SOURCE_DIR}/external/sdl3)
add_library(sdl3_image STATIC IMPORTED)
set_target_properties(sdl3_image PROPERTIES IMPORTED_LOCATION ${MSE_BASE_SOURCE_DIR}/bin/libSDL3_image.so)
#set_target_properties(sdl3_image PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${MSE_BASE_SOURCE_DIR}/external/sdl3)
add_library(sdl3_mixer STATIC IMPORTED)
set_target_properties(sdl3_mixer PROPERTIES IMPORTED_LOCATION ${MSE_BASE_SOURCE_DIR}/bin/libSDL3_mixer.so)
#set_target_properties(sdl3_mixer PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${MSE_BASE_SOURCE_DIR}/external/sdl3)
add_library(sdl3_ttf STATIC IMPORTED)
set_target_properties(sdl3_ttf PROPERTIES IMPORTED_LOCATION ${MSE_BASE_SOURCE_DIR}/bin/libSDL3_ttf.so)
#set_target_properties(sdl3_ttf PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${MSE_BASE_SOURCE_DIR}/external/sdl3)
add_library(yaml-cpp STATIC IMPORTED)
set_target_properties(yaml-cpp PROPERTIES IMPORTED_LOCATION ${MSE_BASE_SOURCE_DIR}/lib/libyaml-cpp.a)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    sdl3
    sdl3_image
    sdl3_mixer
    sdl3_ttf
    yaml-cpp
    )
target_link_libraries(${PROJECT_NAME} PRIVATE -lpthread "-Wl,-rpath,./bin")

# build examples
add_subdirectory(${MSE_BASE_SOURCE_DIR}/examples)
#include(${MSE_BASE_SOURCE_DIR}/examples/CMakeLists.txt)