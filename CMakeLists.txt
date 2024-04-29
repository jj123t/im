cmake_minimum_required(VERSION 3.22)

#project(thirdparty)
#execute_process(COMMAND sudo protoc
#        --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/server/thirdparty/protobuf
#        ${CMAKE_CURRENT_SOURCE_DIR}/server/thirdparty/protobuf/msg.proto)

project(server)
set(CMAKE_CXX_STANDARD 17)
include_directories(server)
FILE(GLOB src
        server/common/async/*.cpp
        server/common/network/*.cpp
        server/common/thread/*.cpp
        server/common/util/*.cpp
        server/adapter/*.cpp
        server/src/*.cpp
        server/thirdparty/protobuf/msg.pb.cc
)
FILE(GLOB hdr
        server/common/async/*.h
        server/common/network/*.h
        server/common/thread/*.h
        server/common/util/*.h
        server/adapter/*.h
        server/src/*.h
)
find_package(Protobuf REQUIRED)
message("Protobuf" ${Protobuf_INCLUDE_DIRS})
message("Protobuf" ${Protobuf_LIBRARIES})
include_directories(${Protobuf_INCLUDE_DIRS})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(imserver server/server.cpp ${src} ${hdr})
target_link_libraries(imserver -lmysqlcppconn ${Protobuf_LIBRARIES})

project(client)
include_directories(./server/)
find_package(Protobuf REQUIRED)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(testClient server/testClient.cpp server/common/network/networkbase.cpp server/common/network/networkbase.h server/thirdparty/protobuf/msg.pb.cc)
target_link_libraries(testClient  ${Protobuf_LIBRARIES})

project(testMysql)
include_directories(./server/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(testMysql server/testMysql.cpp server/common/network/mysqlmanager.cpp)
target_link_libraries(testMysql -lmysqlcppconn)

