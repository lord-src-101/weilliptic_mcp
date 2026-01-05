cmake_minimum_required(VERSION 3.10)
project(in_memory_db)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIBWEIL_DIR "${CMAKE_SOURCE_DIR}/lib")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(in_memory_db src/main.cpp)

target_link_libraries(in_memory_db "${LIBWEIL_DIR}/libweilsdk_static.a")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s STANDALONE_WASM --no-entry -O3 -s ERROR_ON_UNDEFINED_SYMBOLS=0")