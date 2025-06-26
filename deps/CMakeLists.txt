
project(jukebox-deps)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)

set(fetch_list capo)

# capo
FetchContent_Declare(
  capo
  GIT_REPOSITORY https://github.com/capo-devs/capo-lite
  GIT_TAG v2.1.0
  GIT_SHALLOW TRUE
)

if(JUKE_USE_LIBXMP)
  # libxmp
  FetchContent_Declare(
    libxmp
    GIT_REPOSITORY https://github.com/libxmp/libxmp
    GIT_TAG libxmp-4.6.3
    GIT_SHALLOW TRUE
  )
  list(APPEND fetch_list libxmp)
endif()

set(BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(LIBXMP_PIC ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(${fetch_list})

add_library(jukebox-deps INTERFACE)
add_library(jukebox::deps ALIAS jukebox-deps)

target_link_libraries(jukebox-deps INTERFACE
  capo::capo
)
