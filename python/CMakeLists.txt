# Configure necessary packages, boost::python and python headers
find_package(PythonLibs 2.7 REQUIRED)
find_package(Boost COMPONENTS python REQUIRED)

include_directories(
  ${PYTHON_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR})

link_libraries(
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARIES})

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/python/gammatone)

python_add_module(_gammatone ${CMAKE_CURRENT_SOURCE_DIR}/gammatone.cpp)
file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
  DESTINATION ${LIBRARY_OUTPUT_PATH})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_wrapper.py DESTINATION ${PROJECT_BINARY_DIR}/python)
