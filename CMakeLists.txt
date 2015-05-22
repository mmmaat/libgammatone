
cmake_minimum_required(VERSION 2.8)
project(libgammatone)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")

# include libutils
set(utils_root ~/dev/libutils)

# configure boost
find_package(Boost 1.55 REQUIRED COMPONENTS system filesystem iostreams program_options)
link_directories(${DEPS_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})
add_definitions(${DEPS_CFLAGS_OTHER})

include_directories(
  ${Boost_INCLUDE_DIR}
  ${utils_root}/include
  ${PROJECT_BINARY_DIR}/include)


add_subdirectory(test)
add_subdirectory(example)
add_subdirectory(doc EXCLUDE_FROM_ALL)
