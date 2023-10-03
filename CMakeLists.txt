# SPDX-FileCopyrightText: Loren Burkholder
#
# SPDX-License-Identifier: GPL-3.0

# basic cmake setup
cmake_minimum_required(VERSION 3.16)
project(muziko VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# import packages
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 QmlImportScanner)
get_target_property(QMAKE Qt::qmake IMPORTED_LOCATION)

# set up corrosion for Rust
find_package(Corrosion QUIET)
if(NOT Corrosion_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        Corrosion
        GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
        GIT_TAG v0.4.3
    )
    FetchContent_MakeAvailable(Corrosion)
endif()

set(MUZIKO_CRATE_NAME muziko_rs)
corrosion_import_crate(MANIFEST_PATH rs/Cargo.toml CRATES ${MUZIKO_CRATE_NAME})

set(CXXQT_EXPORT_DIR "${CMAKE_CURRENT_BINARY_DIR}/cxxqt")
corrosion_set_env_vars(${MUZIKO_CRATE_NAME}
    "CXXQT_EXPORT_DIR=${CXXQT_EXPORT_DIR}"
    "QMAKE=${QMAKE}"
)
add_library(muziko_rs_lib INTERFACE)
target_include_directories(muziko_rs_lib INTERFACE "${CXXQT_EXPORT_DIR}/${MUZIKO_CRATE_NAME}")
target_link_libraries(muziko_rs_lib INTERFACE
    "$<LINK_LIBRARY:WHOLE_ARCHIVE,${MUZIKO_CRATE_NAME}-static>"
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::QuickControls2
)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(muziko
    cpp/CppHelper.cpp
    cpp/CppHelper.h
    cpp/main.cpp

    qml/qml.qrc
)

#qt_add_qml_module(muziko
#    URI dev.lorendb.muziko
#    VERSION 1.0
#    QML_FILES qml/main.qml
#)

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
    Qt6::Quick
    muziko_rs_lib
)

include(GNUInstallDirs)
install(TARGETS muziko
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
