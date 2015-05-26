# build exemples
set( EXECUTABLE_OUTPUT_PATH bin )

file( GLOB_RECURSE EXEMPLE_SOURCE src/*.cpp )
foreach( src ${EXEMPLE_SOURCE} )
  string( REPLACE ".cpp" "" bin ${src} )
  string( REGEX REPLACE ".*/" "" bin ${bin} )
  add_executable( ${bin} ${src} )
  target_link_libraries( ${bin} ${Boost_LIBRARIES} )
endforeach(src)
