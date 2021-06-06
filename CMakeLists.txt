cmake_minimum_required( VERSION 3.15 )
project( meadow VERSION 0.0.1 )

enable_language( CXX )
set( CMAKE_CONFIGURATION_TYPES "Debug;Release;Native" )

set( CMAKE_CXX_FLAGS         "-std=c++20" )
set( CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -Wpedantic -Og -march=core2 -mtune=native -ggdb3" )
set( CMAKE_CXX_FLAGS_RELEASE "-w -O2 -march=core2 -mtune=generic -DNDEBUG" )
set( CMAKE_CXX_FLAGS_NATIVE  "-w -Ofast -march=native -mtune=native -DNDEBUG" )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Release )
endif()
message( "Using build configuration: ${CMAKE_BUILD_TYPE}" )

configure_file( "${CMAKE_SOURCE_DIR}/src/include/meadow/version.hh.in" "include/meadow/version.hh" )
include_directories( "${CMAKE_SOURCE_DIR}/src/include" "${CMAKE_BINARY_DIR}/include" )

file( GLOB_RECURSE LIB_FILES 
	"${CMAKE_SOURCE_DIR}/src/lib/*.cc"
)

add_library( meadow SHARED ${LIB_FILES} )
install( TARGETS meadow LIBRARY DESTINATION "lib" )

file( GLOB_RECURSE TEST_FILES 
	"${CMAKE_SOURCE_DIR}/src/test/*.cc"
)

find_package( SDL2 REQUIRED )

add_executable( meadow_tests ${TEST_FILES} )
set_target_properties( meadow PROPERTIES
	SOVERSION 0
	VERSION   ${PROJECT_VERSION}
)
target_link_libraries( meadow_tests PUBLIC meadow "-lpthread" SDL2::SDL2 )
install( TARGETS meadow_tests RUNTIME DESTINATION "bin" )

install( 
	DIRECTORY "${CMAKE_SOURCE_DIR}/src/include/meadow" 
	DESTINATION "include"
	FILES_MATCHING PATTERN "*.hh"
)
install( FILES "${CMAKE_BINARY_DIR}/include/meadow/version.hh" DESTINATION "include/meadow" )
