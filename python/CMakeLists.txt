# Configure necessary packages, boost::python and python headers
find_package(PythonLibs 2.7 REQUIRED)
find_package(Boost COMPONENTS python REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})
link_libraries(${Boost_LIBRARIES} ${PYTHON_LIBRARIES})


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_wrapper.py DESTINATION ${PROJECT_BINARY_DIR}/python)
add_subdirectory(gammatone)
