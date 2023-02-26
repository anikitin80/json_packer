cmake_minimum_required(VERSION 3.2)

project(jsoncpp)

include_directories("include/")

add_library(jsoncpp
    include/json/config.h
    include/json/features.h
    include/json/forwards.h
    include/json/json.h
    include/json/reader.h
    include/json/value.h
    include/json/writer.h
	src/lib_json/json_internalarray.inl
	src/lib_json/json_internalmap.inl
	src/lib_json/json_reader.cpp
	src/lib_json/json_value.cpp
	src/lib_json/json_valueiterator.inl
	src/lib_json/json_writer.cpp
	)
	
