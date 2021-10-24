# Set the minimum version of CMake that's required
cmake_minimum_required(VERSION 3.12)

# Set the name of the project
project(FiberTrackerDipClient
	VERSION 1.0.0
	DESCRIPTION "FiberTracker DAQ Dip Client" 
	LANGUAGES CXX)

add_subdirectory(FiberTrackerDAQ)
add_subdirectory(client)
