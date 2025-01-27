cmake_minimum_required(VERSION 3.16)

project(chatroom VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)
find_package(Qt6Multimedia)

qt_add_executable(appchatroom

        model/File_Handle.h
        model/File_Handle.cpp
        model/Connection_Client.h
        model/Connection_Client.cpp
        client.h
        client.cpp

        main.cpp

)

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


)

qt_add_qml_module(appchatroom
    URI chatroom
    VERSION 1.0
    QML_FILES qml.qrc
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appchatroom PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appchatroom
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appchatroom
    PRIVATE Qt6::Quick)

include(GNUInstallDirs)
install(TARGETS appchatroom
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
