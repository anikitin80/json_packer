cmake_minimum_required(VERSION 3.2)

project(JsonPacker)

include_directories("jsoncpp/include/")

add_executable(JsonPacker
	JsonPacker/main.cpp
	JsonPacker/JsonPacker.cpp
	JsonPacker/JsonPacker.h
	JsonPacker/TLVConverter.h
	JsonPacker/TLVConverter.cpp
	jsoncpp/src/lib_json/json_internalarray.inl
	jsoncpp/src/lib_json/json_internalmap.inl
	jsoncpp/src/lib_json/json_reader.cpp
	jsoncpp/src/lib_json/json_value.cpp
	jsoncpp/src/lib_json/json_valueiterator.inl
	jsoncpp/src/lib_json/json_writer.cpp
	)
	
