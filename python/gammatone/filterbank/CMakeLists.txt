set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/python/gammatone/filterbank)
python_add_module(_filterbank  ${CMAKE_CURRENT_SOURCE_DIR}/_filterbank.cpp)
file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
  DESTINATION ${LIBRARY_OUTPUT_PATH})
