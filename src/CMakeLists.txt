## -----------------------------------------------------------------------------
##  G4Basic | src/CMakeLists.txt
##
##
## -----------------------------------------------------------------------------

file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(G4OPSIM_SRC OBJECT ${SRCS})
