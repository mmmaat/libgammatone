set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/python/gammatone/filter)
python_add_module(_filter  ${CMAKE_CURRENT_SOURCE_DIR}/_filter.cpp)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${LIBRARY_OUTPUT_PATH})
