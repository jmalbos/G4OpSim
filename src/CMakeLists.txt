## -----------------------------------------------------------------------------
##  G4OpSim | src/CMakeLists.txt
##
##  * Author: <justo.martin-albo@ific.uv.es>
##  * Creation date: 10 February 2020
## -----------------------------------------------------------------------------

file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(${CMAKE_PROJECT_NAME}_SRC OBJECT ${SRCS})
