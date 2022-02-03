cmake_minimum_required (VERSION 3.15)
project (PythiaGen)

# The version number
set (PythiaGen_VERSION_MAJOR 1)
set (PythiaGen_VERSION_MINOR 0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if(CMAKE_BUILD_TYPE MATCHES Debug)
	message(STATUS "In Debug Mode")
	add_compile_options(-g)
	#add_compile_options(-DDebug)
endif()

add_compile_options(-Wfatal-errors)
add_compile_options(-Wno-deprecated)
add_compile_options(-std=c++11)
add_compile_options(-DGZIPSUPPORT)

# source code
add_subdirectory(src)