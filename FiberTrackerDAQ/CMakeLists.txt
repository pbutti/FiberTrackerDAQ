# Set the minimum version of CMake that's required
cmake_minimum_required(VERSION 3.12)

# Set the project name
project(FiberTrackerDAQ
  VERSION 0.1
  DESCRIPTION "Module containing all emulator classes." 
  LANGUAGES CXX)

# Find all of the source files we want to add to the emulators library.
# Note, this excludes all executables which have '_' in the name.
file(GLOB SRC_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/[A-Z]*.cxx)

file(GLOB INCLUDE_FILES ${PROJECT_SOURCE_DIR}/include/*.h)

message(STATUS "INCLUDE_FILES== ${INCLUDE_FILES}")

add_library(FiberTrackerDAQ SHARED ${SRC_FILES} ${INCLUDE_FILES})

target_include_directories(FiberTrackerDAQ
  PUBLIC ${PROJECT_SOURCE_DIR}/include ${DIP_Dir}/include)

target_link_libraries(FiberTrackerDAQ
  PUBLIC ${DIP_Dir}/lib64/libdip.so)

set_target_properties(FiberTrackerDAQ PROPERTIES
  PUBLIC_HEADER ${INCLUDE_FILES})

install(TARGETS FiberTrackerDAQ
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  )

