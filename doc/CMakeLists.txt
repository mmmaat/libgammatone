# Build documentation
find_package(Doxygen)
if( DOXYGEN_FOUND )
  add_custom_target( doc
    ${DOXYGEN_EXECUTABLE} Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM )
endif(DOXYGEN_FOUND)
