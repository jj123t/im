cmake_minimum_required(VERSION 3.5)

project(Client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_INCLUDE_DIRS  "D:/Code/boost_1_85_0")
FILE(GLOB_RECURSE Boost_LIBRARIES #D:/Code/boost_1_85_0/bin.v2/libs/*.a
    D:/Code/boost_1_85_0/bin.v2/libs/*.dll
)
message("include: " D:/Code/boost_1_85_0)
message("lib: " ${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

set(Protobuf_INCLUDE_DIRS "D:/Code/protobuf-21.6/src")
FILE(GLOB Protobuf_LIBRARIES
    D:/Code/out/libprotobuf.dll
    D:/Code/out/libprotoc.dll
)
message("include: " ${Protobuf_INCLUDE_DIRS})
message("lib: " ${Protobuf_LIBRARIES})
include_directories(${Protobuf_INCLUDE_DIRS})

include_directories(D:/Code/Client/)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        # client.cpp
        # client.h
        # client.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        common/async/asiotimer.cpp common/async/asiotimer.h
        common/network/networkbase.cpp common/network/networkbase.h
        common/threading/threadpool.cpp common/threading/threadpool.h
        src/eventloop.cpp src/eventloop.h

        # src/msgmanager.cpp src/msgmanager.h
        src/qnchatmessage.cpp src/qnchatmessage.h

        src/mainwindow.cpp src/mainwindow.h
        src/register.cpp src/register.h
        src/login.cpp src/login.h
        # src/server.cpp src/server.h
        thirdparty/protobuf/ack.pb.cc
        thirdparty/protobuf/ack.pb.h
        # thirdparty/protobuf/msg.pb.cc thirdparty/protobuf/msg.pb.h
        src/client.cpp src/client.h

        src/client.ui src/login.ui
        src/mainwindow.ui src/register.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Client SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Client
            ${PROJECT_SOURCES}
        )
    endif()
endif()
target_link_libraries(Client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ws2_32 -lwsock32 ${Protobuf_LIBRARIES} ${Boost_LIBRARIES})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Client)
endif()
set_target_properties(Client PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Client)
endif()
