# SPDX-FileCopyrightText: Loren Burkholder
#
# SPDX-License-Identifier: GPL-3.0

# basic cmake setup
cmake_minimum_required(VERSION 3.16)
project(muziko VERSION 0.0.1 LANGUAGES CXX)

set(muziko_VERSION_STR "${muziko_VERSION_MAJOR}.${muziko_VERSION_MINOR}.${muziko_VERSION_PATCH}")
configure_file(cmake/MuzikoVersion.h.in cpp/MuzikoVersion.h)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network Qml Quick QuickControls2 Test)
qt_standard_project_setup(REQUIRES 6.5)

set(MUZIKO_LIB_SRC
    cpp/Muziko.cpp
    cpp/Muziko.h
    cpp/Settings.cpp
    cpp/Settings.h
    cpp/SongsModel.cpp
    cpp/SongsModel.h
)

qt_add_executable(muziko
    cpp/FaviconProvider.cpp
    cpp/FaviconProvider.h
    cpp/main.cpp
)
qt_add_qml_module(muziko
    URI dev.lorendb.muziko
    VERSION 1.0
    QML_FILES
        qml/AboutPage.qml
        qml/AddInstrumentPage.qml
        qml/AddSongPage.qml
        qml/BackupRestorePage.qml
        qml/EditSongPage.qml
        qml/Main.qml
        qml/SettingsPage.qml
        qml/SongCheckbox.qml
        qml/SongInfoPopup.qml
        qml/SongsPage.qml
    SOURCES
        ${MUZIKO_LIB_SRC}
    RESOURCES
        qtquickcontrols2.conf
        qml/icons/add.svg
        qml/icons/calendar.svg
        qml/icons/close.svg
        qml/icons/delete.svg
        qml/icons/done.svg
        qml/icons/edit.svg
        qml/icons/logo.svg
        qml/icons/menu.svg
        qml/icons/null.svg
        qml/icons/settings.svg

        qml/icons/external/github.svg
        qml/icons/external/liberapay.svg
        qml/icons/external/paypal.svg
)

target_include_directories(muziko PRIVATE cpp)

set_target_properties(muziko PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER dev.lorendb.muziko
    MACOSX_BUNDLE_BUNDLE_VERSION ${muziko_VERSION_STR}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${muziko_VERSION_STR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android/
    QT_ANDROID_TARGET_SDK_VERSION 33
)

if(ANDROID)
    include(FetchContent)
    FetchContent_Declare(
      android_openssl
      DOWNLOAD_EXTRACT_TIMESTAMP true
      URL      https://github.com/KDAB/android_openssl/archive/refs/heads/master.zip
    )
    FetchContent_MakeAvailable(android_openssl)
    include(${android_openssl_SOURCE_DIR}/android_openssl.cmake)

    add_android_openssl_libraries(muziko)

    # This makes Android builds have unique versions for each architecture.
    # Taken from https://16bpp.net/blog/post/making-a-cross-platform-mobile-desktop-app-with-qt-62/
    # Figure out the platform & architecture code we need
    set(ANDROID_PLATFORM_ARCHITECTURE_CODE "xxx")
    if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
      set(ANDROID_PLATFORM_ARCHITECTURE_CODE "032")
    elseif (${ANDROID_ABI} STREQUAL "arm64-v8a")
      set(ANDROID_PLATFORM_ARCHITECTURE_CODE "064")
    elseif (${ANDROID_ABI} STREQUAL "x86")
      set(ANDROID_PLATFORM_ARCHITECTURE_CODE "132")
    elseif (${ANDROID_ABI} STREQUAL "x86_64")
      set(ANDROID_PLATFORM_ARCHITECTURE_CODE "164")
    endif()

    # Slap the above together with a version code (major and minor only)
    set(ANDROID_VERSION_CODE "${ANDROID_PLATFORM_ARCHITECTURE_CODE}${PROJECT_VERSION_MAJOR}${PROJECT_VERSION_MINOR}")
    set_property(TARGET muziko APPEND PROPERTY QT_ANDROID_VERSION_CODE ${ANDROID_VERSION_CODE})
endif()

target_link_libraries(muziko PRIVATE
    Qt::Network
    Qt::Quick
    Qt::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS muziko
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#####################
# TESTS
#####################

enable_testing(true)

qt_add_executable(SongsFilterModelTest tests/SongsFilterModelTest.cpp ${MUZIKO_LIB_SRC}
    qml/SettingsPage.qml)
target_include_directories(SongsFilterModelTest PRIVATE cpp)
add_test(NAME SongsFilterModelTest COMMAND SongsFilterModelTest)
target_link_libraries(SongsFilterModelTest PRIVATE Qt::Test Qt::Qml)
