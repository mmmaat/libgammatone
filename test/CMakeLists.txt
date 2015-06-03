add_custom_target(tests)

# test specific headers in test/include
include_directories(include)

# the unit tests are in a separate directory
add_subdirectory(unit)

# build in test/bin
set(EXECUTABLE_OUTPUT_PATH bin)

# configure boost for use with gnuplot-iostream
find_package(Boost 1.55 REQUIRED COMPONENTS
  system
  filesystem
  iostreams)
link_directories(${DEPS_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})
add_definitions(${DEPS_CFLAGS_OTHER})
include_directories(${Boost_INCLUDE_DIR})

# build standalone tests
file( GLOB_RECURSE TESTS_SOURCE src/*.cpp )
foreach( src ${TESTS_SOURCE} )
  string( REPLACE ".cpp" "" bin ${src} )
  string( REGEX REPLACE ".*/" "" bin ${bin} )
  add_executable( ${bin} ${src} )
  target_link_libraries( ${bin} ${Boost_LIBRARIES} )
  add_dependencies(tests ${bin})
endforeach(src)
