project(jukebox-app)

add_executable(jukebox)

target_link_libraries(jukebox
  PRIVATE
  jukebox::deps
  juke::lib
)

target_sources(jukebox PRIVATE
  src/main.cpp
)
