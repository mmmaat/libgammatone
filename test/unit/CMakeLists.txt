# Build the tests of libgammatone. Requires boost

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test/bin )

# build unit tests
file(GLOB_RECURSE UNIT_TESTS src/*.cpp)
add_executable(unit_test ${UNIT_TESTS})
target_link_libraries(unit_test ${Boost_LIBRARIES})
