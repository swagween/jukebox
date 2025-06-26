project(jukebox-app)

add_library(jukebox-app)
add_library(jukebox-app::lib ALIAS jukebox-app)

# gvdi
FetchContent_Declare(gvdi
  GIT_REPOSITORY https://github.com/karnkaul/gvdi
  GIT_TAG v0.2.5
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(gvdi)

target_link_libraries(jukebox-app
  PUBLIC
  juke::juke
  gvdi::gvdi
)

# Glob all headers
file(GLOB_RECURSE headers "include/*.hpp")

# Set as FILE_SET, this also sets the include directory
target_sources(jukebox-app PUBLIC FILE_SET HEADERS
  BASE_DIRS include
  FILES ${headers}
)

# Setup our include structure
target_include_directories(jukebox-app
  PRIVATE "src"
)

add_subdirectory(src)
