# from https://www.tty1.net/blog/2014/cmake-doxygen_en.html

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION
  "Create and install the HTML based API documentation (requires Doxygen)"
  ${DOXYGEN_FOUND})


if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    # Build the exemples
    add_custom_target(example)
    file(GLOB_RECURSE EXAMPLES ./example/*.cpp)

    # build each source and add it in example target
    foreach(src ${EXAMPLES})
      string(CONCAT bin "example-" ${src})
      string(REPLACE ".cpp" "" bin ${bin})
      string(REGEX REPLACE ".*/" "" bin ${bin})
      add_executable(${bin} ${src})
      add_dependencies(example ${bin})
    endforeach(src)
    
    # Configure Doxyfile.in
    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    # Build documentation
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)   
endif()

# install documentation
install(
  DIRECTORY    ${PROJECT_BINARY_DIR}/doc/html/
  DESTINATION  share/doc/gammatone)
