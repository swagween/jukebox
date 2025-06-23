
project(jukebox-deps)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)

# gvdi
FetchContent_Declare(gvdi
  GIT_REPOSITORY https://github.com/karnkaul/gvdi
  GIT_TAG v0.2.5
)

# capo
FetchContent_Declare(
  capo
  GIT_REPOSITORY https://github.com/capo-devs/capo-lite
  GIT_TAG v2.0.1
)

# libxmp
FetchContent_Declare(
  libxmp
  GIT_REPOSITORY https://github.com/libxmp/libxmp
  GIT_TAG libxmp-4.6.3
)

# djson
FetchContent_Declare(
  djson
  GIT_REPOSITORY https://github.com/karnkaul/djson
  GIT_TAG v2.1.1
)

FetchContent_MakeAvailable(gvdi capo libxmp djson)

add_library(jukebox-deps INTERFACE)
add_library(jukebox::deps ALIAS jukebox-deps)

target_link_libraries(jukebox-deps INTERFACE
  djson::djson
  capo::capo
  gvdi::gvdi
)
