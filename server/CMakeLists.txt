cmake_minimum_required(VERSION 3.22)

project(server)

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDEBUG)
ENDIF()

set(CMAKE_CXX_STANDARD 17)
include_directories(./)
FILE(GLOB src
        ./common/async/*.cpp
        ./common/network/*.cpp
        ./common/thread/*.cpp
        ./common/util/*.cpp
        ./src/*.cpp
        ./thirdparty/protobuf/*.cc
        ./thirdparty/mysql/*.cpp
)
FILE(GLOB hdr
        ./common/async/*.h
        ./common/network/*.h
        ./common/thread/*.h
        ./common/util/*.h
        ./src/*.h
)
find_package(Protobuf REQUIRED)
message("Protobuf" ${Protobuf_INCLUDE_DIRS})
message("Protobuf" ${Protobuf_LIBRARIES})
include_directories(${Protobuf_INCLUDE_DIRS})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(imserver ./server.cpp ${src} ${hdr})
target_link_libraries(imserver -lmysqlcppconn ${Protobuf_LIBRARIES})