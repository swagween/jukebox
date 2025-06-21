
project(jukebox-deps)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)

# SFML
FetchContent_Declare(sfml
        GIT_REPOSITORY https://github.com/SFML/SFML
        GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(sfml)

# capo
FetchContent_Declare(
        capo
        GIT_REPOSITORY https://github.com/capo-devs/capo-lite
        GIT_TAG v2.0.1
)
FetchContent_MakeAvailable(capo)

# Dear ImGui
add_subdirectory(imgui)

# Dear ImGui-SFML
set(IMGUI_SFML_FIND_SFML OFF)
add_subdirectory(imgui-sfml)

# djson
FetchContent_Declare(
        djson
        GIT_REPOSITORY https://github.com/karnkaul/djson
        GIT_TAG v2.1.1
)
FetchContent_MakeAvailable(djson)

add_library(jukebox-deps INTERFACE)
add_library(jukebox::deps ALIAS jukebox-deps)

target_link_libraries(jukebox-deps INTERFACE
        djson::djson
        capo::capo
        ImGui-SFML::ImGui-SFML
        SFML::Graphics
)
