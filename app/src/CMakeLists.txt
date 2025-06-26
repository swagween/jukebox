
target_sources(jukebox-app PRIVATE
  MediaPlayer.cpp
  Application.cpp
)

add_executable(jukebox-player)

target_sources(jukebox-player PRIVATE 
  main.cpp
)

target_link_libraries(jukebox-player PRIVATE
  jukebox-app::lib
)
