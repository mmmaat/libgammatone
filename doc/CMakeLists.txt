
# Configure version in doc
configure_file(
  "${PROJECT_SOURCE_DIR}/doc/Doxyfile.in"
  "${PROJECT_BINARY_DIR}/doc/Doxyfile"
  )

find_package(Doxygen)
add_custom_target(html
  COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

add_subdirectory(exemple)
