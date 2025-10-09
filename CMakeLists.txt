cmake_minimum_required(VERSION 3.16)
set(CMAKE_PREFIX_PATH "/home/dev/qt-build/qtbase")

project(chatroom VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Correctly find Qt6 and the required components
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZeroMQ REQUIRED libzmq)
find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia)

include_directories(${ZeroMQ_INCLUDE_DIRS})
link_directories(${ZeroMQ_LIBRARY_DIRS})
find_package(Protobuf REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/protobuf/cpp)
# Add the executable for appchatroom
qt_add_executable(appchatroom
        model/File_Handle.h
        model/File_Handle.cpp
        model/Connection_Client.h
        model/Connection_Client.cpp
        client.h
        client.cpp
        main.cpp
        protobuf/cpp/message.pb.cc
)

# Add the executable for server
add_executable(server
        model/Server_Obj.h
        model/Connection_Manager.h
        model/Server_Obj.cpp
        model/Connection_Manager.cpp
        model/Connection_Server.cpp
        model/Connection_Client.h
        model/User.cpp
        model/User.h
        model/File_Handle.cpp
        model/File_Handle.h
        server.cpp
        protobuf/cpp/message.pb.cc
)





# Add the QML module for appchatroom
qt_add_qml_module(appchatroom
        URI chatroom
        VERSION 1.0
        QML_FILES qml.qrc
)

# Set properties for the appchatroom target (bundle version for macOS, etc.)
set_target_properties(appchatroom PROPERTIES
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

# Link necessary Qt6 libraries for appchatroom
target_link_libraries(appchatroom
        PRIVATE Qt6::Quick Qt6::Multimedia ${ZeroMQ_LIBRARIES} ${Protobuf_LIBRARIES})

# Link necessary Qt6 libraries for server
target_link_libraries(server
        PRIVATE Qt6::Core Qt6::Network ${ZeroMQ_LIBRARIES} ${Protobuf_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS appchatroom
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
