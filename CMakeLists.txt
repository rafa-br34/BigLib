cmake_minimum_required(VERSION 3.20)

project(
	BigLib
	DESCRIPTION "A feature rich multi-purpose library"
	HOMEPAGE_URL "https://github.com/rafa-br34/BigLib"
	LANGUAGES CXX
)

# Setup constants
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Generator = ${CMAKE_GENERATOR}")


add_executable(${PROJECT_NAME} EXAMPLES/LibTest.cpp SOURCE/Base.cpp)