find_package(Doxygen)
add_custom_target(html
  COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

add_subdirectory(exemple)
