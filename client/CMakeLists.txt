# Set the minimum version of CMake that's required
cmake_minimum_required(VERSION 3.12)

# Set the project name
project(client
  VERSION 0.1
  DESCRIPTION "Dip client writer and reader" 
  LANGUAGES CXX)

add_executable(clientBinary ${PROJECT_SOURCE_DIR}/src/clientBinary.cxx)

target_include_directories(clientBinary
  PUBLIC FiberTrackerDAQ/include
  PUBLIC $DIP_Dir/include)

target_link_libraries(clientBinary
  PUBLIC FiberTrackerDAQ ${DIP_Dir}/lib64/libdip.so)

install(TARGETS clientBinary DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

add_executable(readBinary ${PROJECT_SOURCE_DIR}/src/readBinary.cxx)

target_include_directories(readBinary
  PUBLIC FiberTrackerDAQ/include
  PUBLIC $DIP_Dir/include)

target_link_libraries(readBinary
  PUBLIC FiberTrackerDAQ ${DIP_Dir}/lib64/libdip.so)

install(TARGETS readBinary DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
