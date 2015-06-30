
add_custom_target(exemple)

# build in doc/example/bin
set(EXECUTABLE_OUTPUT_PATH bin)

# sources are doc/example/*.cpp
file( GLOB_RECURSE EXAMPLES src/*.cpp )

# build each source and add it in example target
foreach( src ${EXAMPLES} )
  string( REPLACE ".cpp" "" bin ${src} )
  string( REGEX REPLACE ".*/" "" bin ${bin} )
  add_executable(${bin} ${src})
  add_dependencies(exemple ${bin})
endforeach(src)
