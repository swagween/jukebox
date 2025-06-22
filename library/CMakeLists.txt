project(juke-lib)

add_library(juke)
add_library(juke::lib ALIAS juke)

target_link_libraries(juke
  PUBLIC
  jukebox::deps
)

# Glob all headers
file(GLOB_RECURSE headers "include/*.hpp")

# Set as FILE_SET, this also sets the include directory
target_sources(juke PUBLIC FILE_SET HEADERS
  BASE_DIRS include
  FILES ${headers}
)

# Setup our include structure
target_include_directories(juke
  PRIVATE "src"
)

add_subdirectory(src)
