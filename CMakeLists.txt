cmake_minimum_required(VERSION 3.2)

project(json_packer)

include_directories("jsoncpp/include/")

add_executable(jsonpacker
	jsonpacker/main.cpp
	jsonpacker/JsonPacker.cpp
	jsonpacker/JsonPacker.h
	jsonpacker/TLVConverter.h
	jsonpacker/TLVConverter.cpp
	jsoncpp/src/lib_json/json_internalarray.inl
	jsoncpp/src/lib_json/json_internalmap.inl
	jsoncpp/src/lib_json/json_reader.cpp
	jsoncpp/src/lib_json/json_value.cpp
	jsoncpp/src/lib_json/json_valueiterator.inl
	jsoncpp/src/lib_json/json_writer.cpp
	)
	
