cmake_minimum_required(VERSION 3.2)

project(jsonpacker)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(
	"../build/_deps/googletest-src/googletest/include/" 
	"../jsonpacker/"
	"../jsoncpp/include/")

add_executable(
  jsonpacker_test
  main.cpp
  ../jsonpacker/TLVConverter.cpp
)

target_link_libraries(
  jsonpacker_test
  jsoncpp
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(jsonpacker_test)


