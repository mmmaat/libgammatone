set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/python/gammatone)
file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
  DESTINATION ${LIBRARY_OUTPUT_PATH})

add_subdirectory(filter)
add_subdirectory(filterbank)
