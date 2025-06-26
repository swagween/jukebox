project(juke-lib)

add_library(${PROJECT_NAME})
add_library(juke::juke ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  jukebox::deps
)

if(JUKE_USE_LIBXMP)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
    libxmp::xmp_static
  )
  target_compile_definitions(${PROJECT_NAME} PRIVATE JUKE_USE_LIBXMP)
endif()

# Glob all headers
file(GLOB_RECURSE headers "include/*.hpp")

# Set as FILE_SET, this also sets the include directory
target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS
  BASE_DIRS include
  FILES ${headers}
)

# Setup our include structure
target_include_directories(${PROJECT_NAME}
  PRIVATE "src"
)

add_subdirectory(src)
