# Build the tests of libgammatone. Requires boost, libgnuplot-iostream-dev, gtest

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")

# configure boost
find_package( Boost 1.55 REQUIRED COMPONENTS system filesystem iostreams )
link_directories( ${DEPS_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})
add_definitions( ${DEPS_CFLAGS_OTHER} )

# include libutils
set(utils_root ~/dev/utils)
include_directories(${utils_root}/include)


set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test/bin )
include_directories(${Boost_INCLUDE_DIR} ${PROJECT_BINARY_DIR}/include)

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
add_executable(unit_tests ${UNIT_TESTS})
target_link_libraries(unit_tests ${Boost_LIBRARIES} gtest gtest_main pthread)
