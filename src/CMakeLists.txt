
add_library( FastFlow
             Particle.cpp
             Field.cpp )
target_include_directories( FastFlow PUBLIC ${CMAKE_SOURCE_DIR}/include )
set_target_properties( FastFlow PROPERTIES VERSION ${FastFlow_VERSION} )

#
# The list of headers is formatted a little cumbersome, but don't change it!
#
set( public_h_list 
"include/Particle.h;\
include/Field.h")
#include/SPERR.h;")

set_target_properties( FastFlow PROPERTIES PUBLIC_HEADER "${public_h_list}" )


