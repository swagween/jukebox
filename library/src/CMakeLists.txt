
add_subdirectory(core)
add_subdirectory(ext)

target_sources(juke-lib PRIVATE
  juke.cpp
)
