set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test/bin )

# configure boost for use with boost::test
find_package(Boost 1.55 REQUIRED COMPONENTS unit_test_framework)
link_directories(${DEPS_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})
add_definitions(${DEPS_CFLAGS_OTHER})
include_directories(${Boost_INCLUDE_DIR})

# build unit tests
file(GLOB_RECURSE UNIT_TESTS src/*.cpp)
add_executable(unit ${UNIT_TESTS})
target_link_libraries(unit ${Boost_LIBRARIES})
