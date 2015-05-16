# Build the tests of libgammatone. Requires boost, libgnuplot-iostream-dev, gtest

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test/bin )

# build standalone tests
file( GLOB_RECURSE TESTS_SOURCE src/*.cpp )
foreach( src ${TESTS_SOURCE} )
  string( REPLACE ".cpp" "" bin ${src} )
  string( REGEX REPLACE ".*/" "" bin ${bin} )
  add_executable( ${bin} ${src} )
  target_link_libraries( ${bin} ${Boost_LIBRARIES} )
endforeach(src)

# include gtest
set(gtest_root ~/dev/gtest-1.7.0)
link_directories(${gtest_root})
include_directories(${gtest_root}/include)

# build unit tests
file(GLOB_RECURSE UNIT_TESTS unit/*.cpp)
include_directories(${PROJECT_BINARY_DIR}/test/include)
add_executable(unit_tests ${UNIT_TESTS})
target_link_libraries(unit_tests ${Boost_LIBRARIES} gtest gtest_main pthread)
