cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# set the project name
project(Mini_Serpent_Engine)

# add directories
add_subdirectory(includes)
add_subdirectory(external)
add_subdirectory(src build)

# global strings
set(MSE_BASE_SOURCE_DIR CMAKE_SOURCE_DIR)

set(MSE_EXAMPLES_DIR "{MSE_BASE_SOURCE_DIR}/examples")

set(MSE_INCLUDES_DIR "{MSE_BASE_SOURCE_DIR}/include")
list(APPEND MSE_INCLUDES_DIR "{MSE_BASE_SOURCE_DIR}/external")

# add the executable
# add_executable(Tutorial tutorial.cxx)
target_include_directories(Mini_Serpent_engine PUBLIC
                           "{MSE_INCLUDES_DIR}"
                           )