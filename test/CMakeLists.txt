
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test/bin )

include_directories(include)

# build standalone tests
file( GLOB_RECURSE TESTS_SOURCE src/*.cpp )
foreach( src ${TESTS_SOURCE} )
  string( REPLACE ".cpp" "" bin ${src} )
  string( REGEX REPLACE ".*/" "" bin ${bin} )
  add_executable( ${bin} ${src} )
  target_link_libraries( ${bin} ${Boost_LIBRARIES} )
endforeach(src)

add_subdirectory(unit)
