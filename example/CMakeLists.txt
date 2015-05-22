# Build the exeamples provided with libgammatone. Requires boost, libgnuplot-iostream-dev, libsndfile-dev

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/example/bin)

add_executable(minimal src/minimal.cpp)

add_executable(cochleogram src/cochleogram.cpp)
target_link_libraries(cochleogram ${Boost_LIBRARIES} sndfile)

add_executable(test_holder src/test_holder.cpp)
