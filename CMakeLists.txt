
cmake_minimum_required(VERSION 3.24)

project(jukebox
  LANGUAGES CXX
  VERSION 1.0
)

option(JUKE_USE_LIBXMP "Use libxmp backend" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED JUKE_ROOT_DIR)
  set(JUKE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Jukebox root directory")
endif()

include(cmake/func/AddTargetSource.cmake)

add_subdirectory(deps)
add_subdirectory(library)

option(JUKE_BUILD_APP "Build Media Player app" ${PROJECT_IS_TOP_LEVEL})

if(JUKE_BUILD_APP)
  add_subdirectory(app)
endif()
