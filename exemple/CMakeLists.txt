# Build the exeamples provided with libgammatone. Requires boost, libgnuplot-iostream-dev, libsndfile-dev

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/example/bin)

add_executable(minimal src/minimal.cpp)

include_directories(${PROJECT_BINARY_DIR}/exemple/include)
add_executable(cochleogram src/cochleogram.cpp)
target_link_libraries(cochleogram ${Boost_LIBRARIES} sndfile)
