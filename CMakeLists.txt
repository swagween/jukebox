
cmake_minimum_required(VERSION 3.24)

project(jukebox
        LANGUAGES CXX
        VERSION 1.0
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED JUKE_ROOT_DIR)
    set(JUKE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Jukebox root directory")
endif ()

include(cmake/func/AddTargetSource.cmake)

add_subdirectory(deps)
add_subdirectory(app)

add_executable(jukebox app/src/main.cpp)

target_link_libraries(jukebox PRIVATE juke)

target_link_libraries(jukebox
        PUBLIC
        SFML::Graphics

        PRIVATE
        jukebox::deps
)
