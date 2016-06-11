# Build setup for the python wrapper to libgammatone
#add_custom_target(python)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/python)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/python)

# Configure necessary packages
find_package(PythonLibs 2.7 REQUIRED)
find_package(Boost COMPONENTS python REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

# Define the wrapper library that wraps libgammatone headers
add_library(gammatone
  SHARED ${CMAKE_CURRENT_SOURCE_DIR}/gammatone_wrapper.cpp)

target_link_libraries(gammatone ${Boost_LIBRARIES})
# don't prepend wrapper library name with lib
set_target_properties(gammatone PROPERTIES PREFIX "")
#add_library(python ALIAS gammatone-python)

# install python wrapper
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  DESTINATION include
  PATTERN "*.in" EXCLUDE
  )

set(input ${CMAKE_CURRENT_SOURCE_DIR}/test_wrapper.py)
set(output  ${EXECUTABLE_OUTPUT_PATH}/test_wrapper.py)
#configure_file(${input} ${output} COPYONLY)
add_custom_command(
  TARGET gammatone
  COMMAND ${CMAKE_COMMAND} -E copy ${input} ${output}
  DEPENDS ${input}
  )
