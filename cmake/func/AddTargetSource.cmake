
# Helper function to add headers to the global source list
function(JUKE_ADD_HEADERS)
  set(SOURCE_FILES "")
  foreach(header IN LISTS ARGV)
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${header}")
  endforeach()
  target_sources(juke PUBLIC ${SOURCE_FILES})
endfunction()

# Helper function to add sources to the global source list
function(JUKE_ADD_SOURCES)
  set(SOURCE_FILES "")
  foreach(source IN LISTS ARGV)
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
  endforeach()
  target_sources(juke PRIVATE ${SOURCE_FILES})
endfunction()
