# SPDX-FileCopyrightText: Loren Burkholder
#
# SPDX-License-Identifier: GPL-3.0

# basic cmake setup
cmake_minimum_required(VERSION 3.16)
project(muziko VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick QuickControls2)
qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(muziko cpp/main.cpp)
qt_add_qml_module(muziko
    URI dev.lorendb.muziko
    VERSION 1.0
    NO_RESOURCE_TARGET_PATH
    RESOURCE_PREFIX "/"
    QML_FILES
        qml/AddSongPage.qml
        qml/Main.qml
        qml/SongsPage.qml
    SOURCES
        cpp/Muziko.cpp
        cpp/Muziko.h
        cpp/SongsModel.cpp
        cpp/SongsModel.h
    RESOURCES
        qtquickcontrols2.conf
)

target_include_directories(muziko PRIVATE cpp)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(muziko PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER dev.lorendb.muziko
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(muziko PRIVATE
    Qt::Quick
    Qt::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS muziko
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
