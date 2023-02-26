cmake_minimum_required(VERSION 3.2)

project(jsonpacker)

add_executable(jsonpacker
	main.cpp
	jsonpacker.cpp
	jsonpacker.h
	TLVConverter.h
	TLVConverter.cpp
	)

include_directories("../jsoncpp/include/")
target_link_libraries(jsonpacker jsoncpp)
