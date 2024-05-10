cmake_minimum_required(VERSION 3.22)

project(client)

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDEBUG)
ENDIF()

include_directories(./)
FILE(GLOB srcs
        ./common/async/*.cpp
        ./common/network/*.cpp
        ./common/threading/*.cpp
        ./common/util/*.cpp
        ./src/*.cpp
        ./thirdparty/protobuf/*.cc
)
FILE(GLOB hdrs
        ./common/async/*.h
        ./common/network/*.h
        ./common/thread/*.h
        ./common/util/*.h
        ./adapter/*.h
        ./src/*.h
)
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(imclient #./client.cpp
        ${srcs} ${hdrs}
        src/eventloop.cpp
        src/eventloop.h
        test/testAckMsg.cpp)
target_link_libraries(imclient ${Boost_LIBRARIES}  ${Protobuf_LIBRARIES})