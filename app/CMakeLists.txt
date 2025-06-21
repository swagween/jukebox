
add_library(juke)
add_library(juke::app ALIAS juke)

include(${JUKE_ROOT_DIR}/cmake/func/AddTargetSource.cmake)

target_link_libraries(juke
        PUBLIC
        SFML::Graphics

        PRIVATE
        jukebox::deps
)

# Setup our include structure
target_include_directories(juke
        PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}/include"
        PRIVATE "src"
)

add_subdirectory(include/juke)
add_subdirectory(src)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU|Clang|IntelLLVM|AppleClang")
  if ("${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}" STREQUAL "MSVC") # clang-cl
    add_compile_options(juke INTERFACE /W4056 /W4146 /W4018 /W4056)
  endif ()
  add_compile_options(juke INTERFACE -Wno-sign-conversion -Wno-format-security)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_compile_options(juke INTERFACE /W4056 /W4146 /W4018 /W4056)
endif ()

