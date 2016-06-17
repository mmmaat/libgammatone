##############
# build documentation
##############

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


################
# build exemples
################

add_custom_target(exemple)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/exemple)

# configure boost for use with gnuplot-iostream
find_package(Boost 1.55 REQUIRED COMPONENTS
  system
  filesystem
  iostreams
  timer)
link_directories(${DEPS_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})
add_definitions(${DEPS_CFLAGS_OTHER})
include_directories(${Boost_INCLUDE_DIR})
include_directories(
  ${PROJECT_SOURCE_DIR}/test/include
  ${PROJECT_BINARY_DIR}/test/include)


# build standalone exemples
file( GLOB_RECURSE EXEMPLE_SOURCE exemple/*.cpp )
foreach( src ${EXEMPLE_SOURCE} )
  string( REPLACE ".cpp" "" bin ${src} )
  string( REGEX REPLACE ".*/" "" bin ${bin} )
  add_executable( ${bin} ${src} )
  target_link_libraries( ${bin} ${Boost_LIBRARIES} )
  add_dependencies(exemple ${bin})
endforeach(src)
