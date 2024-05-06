cmake_minimum_required(VERSION 3.22)

#project(thirdparty)
#execute_process(COMMAND sudo protoc
#        --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/server/thirdparty/protobuf
#        ${CMAKE_CURRENT_SOURCE_DIR}/server/thirdparty/protobuf/msg.proto)

#project(server)
#set(CMAKE_CXX_STANDARD 17)
#include_directories(./server)
#FILE(GLOB src
#        server/common/async/*.cpp
#        server/common/network/*.cpp
#        server/common/thread/*.cpp
#        server/common/util/*.cpp
#        server/src/*.cpp
#        server/thirdparty/protobuf/*.cc
#        server/thirdparty/mysql/*.cpp
#)
#FILE(GLOB hdr
#        server/common/async/*.h
#        server/common/network/*.h
#        server/common/thread/*.h
#        server/common/util/*.h
#        server/src/*.h
#)
#find_package(Protobuf REQUIRED)
#message("Protobuf" ${Protobuf_INCLUDE_DIRS})
#message("Protobuf" ${Protobuf_LIBRARIES})
#include_directories(${Protobuf_INCLUDE_DIRS})
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#add_executable(imserver server/server.cpp ${src} ${hdr})
#target_link_libraries(imserver -lmysqlcppconn ${Protobuf_LIBRARIES})
#
#project(testClient)
#include_directories(./server/)
#find_package(Protobuf REQUIRED)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#add_executable(testClient server/testClient.cpp server/common/network/networkbase.cpp server/common/network/networkbase.h server/thirdparty/protobuf/msg.pb.cc)
#target_link_libraries(testClient  ${Protobuf_LIBRARIES})
#
#project(testMysql)
#include_directories(./server/)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#add_executable(testMysql server/testMysql.cpp server/thirdparty/mysql/mysqlmanager.cpp)
#target_link_libraries(testMysql -lmysqlcppconn)
#
#
#project(client)
#include_directories(./client)
#FILE(GLOB srcs
#        client/common/async/*.cpp
#        client/common/network/*.cpp
#        client/common/threading/*.cpp
#        client/common/util/*.cpp
#        client/src/*.cpp
#        client/common/thirdparty/protobuf/*.cc
#)
#FILE(GLOB hdrs
#        client/common/async/*.h
#        client/common/network/*.h
#        client/common/thread/*.h
#        client/common/util/*.h
#        client/adapter/*.h
#        client/src/*.h
#)
#find_package(Protobuf REQUIRED)
#include_directories(${Protobuf_INCLUDE_DIRS})
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#find_package(Boost REQUIRED)
#include_directories(${Boost_INCLUDE_DIRS})
#add_executable(imclient client/client.cpp ${srcs} ${hdrs})
#target_link_libraries(imclient ${Boost_LIBRARIES}  ${Protobuf_LIBRARIES})

project(server)
add_subdirectory(server)
project(client)
add_subdirectory(client)