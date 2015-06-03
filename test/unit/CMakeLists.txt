set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test/bin )

# configure boost for use with boost::test
find_package(Boost 1.55 REQUIRED COMPONENTS unit_test_framework)
link_directories(${DEPS_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})
add_definitions(${DEPS_CFLAGS_OTHER})
include_directories(${Boost_INCLUDE_DIR})

# locate the sources
file(GLOB_RECURSE UNIT_TESTS src/*.cpp)

# build unit tests (by make or make-unit)
# tests are done on a subset of gammatone types
add_executable(unit ${UNIT_TESTS})
target_link_libraries(unit ${Boost_LIBRARIES})

# build all unit tests (by make unit-all)
# tests are done on all gammatone types (huge to compile !)
add_executable(unit-all EXCLUDE_FROM_ALL ${UNIT_TESTS})
target_link_libraries(unit-all ${Boost_LIBRARIES})
set_property(
  TARGET unit-all
  PROPERTY COMPILE_DEFINITIONS LIBGAMMATONE_TEST_ALL)
