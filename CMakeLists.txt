## -----------------------------------------------------------------------------
##  G4OpSim | CMakeLists.txt
##
##  * Author: <justo.martin-albo@ific.uv.es>
##  * Creation date: 10 February 2020
## -----------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(G4OPSIM)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Find Geant4 package, activating all available UI and Vis
## drivers by default. You can set WITH_GEANT4_UIVIS to OFF
## via the command line or ccmake/cmake-gui to build a batch
## mode only executable.
option(WITH_GEANT4_UIVIS "Build with Geant4 UI/VIS drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

## Locate ROOT and define a number of useful targets and variables
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

message("ROOT include directories: ${ROOT_INCLUDE_DIRS}")
message("ROOT libraries: ${ROOT_LIBRARIES}") 

## Setup Geant4 include directories and compile definitions.
include(${Geant4_USE_FILE})

## Recurse through sub-directories
add_subdirectory(src)

add_executable(G4OpSim G4OpSim.cpp $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}_SRC>)
target_include_directories(G4OpSim PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(G4OpSim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
