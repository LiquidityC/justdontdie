cmake_minimum_required (VERSION 3.2.0)
SET(CMAKE_COLOR_MAKEFILE ON)

project (maptodat CXX)

add_executable(maptodat
	src/maptodat.cpp
	)

target_compile_options(maptodat PRIVATE -std=c++11 -Wall)
