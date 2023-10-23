# SPDX-FileCopyrightText: Loren Burkholder
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
#
# SPDX-License-Identifier: GPL-3.0-or-later

# basic cmake setup
cmake_minimum_required(VERSION 3.16)
project(muziko VERSION 0.1.0 LANGUAGES CXX)

include(CMakeDependentOption)
cmake_dependent_option(GENERATE_WINDOWS_INSTALLER "Generate a Windows installer." OFF WIN32 OFF)
option(RUN_CLANG_TIDY "Run clang-tidy checks at build time." OFF)

set(muziko_VERSION_STR "${muziko_VERSION_MAJOR}.${muziko_VERSION_MINOR}.${muziko_VERSION_PATCH}")
configure_file(cmake/MuzikoVersion.h.in cpp/MuzikoVersion.h)

# TODO: change this to 20 as soon as Apple Clang supports concepts
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(RUN_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy
        # Remove these checks because in the latest clang-tidy they go berserk over Qt headers
        -checks=-llvmlibc-restrict-system-libc-cleaners,-misc-include-cleaner;
        -header-filter=.;
        -warnings-as-errors=*;
    )
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Multimedia Network Qml Quick QuickControls2 Test)
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
        qml/ConfettiEffect.qml
        qml/EditSongPage.qml
        qml/Main.qml
        qml/MetronomePage.qml
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
        qml/icons/metronome.svg
        qml/icons/null.svg
        qml/icons/pause.svg
        qml/icons/play.svg
        qml/icons/remove.svg
        qml/icons/settings.svg

        qml/icons/external/github.svg
        qml/icons/external/liberapay.svg
        qml/icons/external/paypal.svg

        qml/res/confettiparticle.svg
        qml/res/metronome.wav
        qml/res/metronome-high.wav
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

    # Slap the above together with a version code
    # NOTE: we are inserting 7s to make sure that every major.minor.patch gets processed as a unique number. Basically, they are like demarcators
    # for the different parts of the version number. Without this, Google Play AABs will end up conflicting (e.g. 1.20.0 would equal 12.0.0).
    set(ANDROID_VERSION_CODE "${ANDROID_PLATFORM_ARCHITECTURE_CODE}7${PROJECT_VERSION_MAJOR}7${PROJECT_VERSION_MINOR}7${PROJECT_VERSION_PATCH}7")
    set_property(TARGET muziko APPEND PROPERTY QT_ANDROID_VERSION_CODE ${ANDROID_VERSION_CODE})
endif()

target_link_libraries(muziko PRIVATE
    Qt::CorePrivate
    Qt::Multimedia
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

qt_add_executable(SongsFilterModelTest tests/SongsFilterModelTest.cpp ${MUZIKO_LIB_SRC})
target_include_directories(SongsFilterModelTest PRIVATE cpp)
add_test(NAME SongsFilterModelTest COMMAND SongsFilterModelTest)
target_link_libraries(SongsFilterModelTest PRIVATE Qt::Test Qt::Qml)

if(GENERATE_WINDOWS_INSTALLER)
    # Set up the installer
    # https://www.ics.com/blog/revisiting-qt-installer-framework-cmake and https://cmake.org/cmake/help/latest/cpack_gen/ifw.html
    # were very helpful in creating the installer code
    set(CPACK_GENERATOR "IFW")
    set(CPACK_IFW_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VENDOR "Loren Burkholder")
    set(CPACK_PACKAGE_VERSION ${Muziko_VERSION_STR})
    set(CPACK_IFW_PACKAGE_TITLE ${CPACK_IFW_PACKAGE_NAME})
    set(CPACK_IFW_PRODUCT_URL "https://github.com/LorenDB/muziko")
    set(CPACK_IFW_PACKAGE_WIZARD_DEFAULT_WIDTH 640)
    set(CPACK_IFW_PACKAGE_WIZARD_DEFAULT_HEIGHT 480)
    set(CPACK_IFW_PACKAGE_WIZARD_STYLE "Modern")
    set(CPACK_IFW_TARGET_DIRECTORY "@ApplicationsDirUser@/Loren Burkholder")
    if(UNIX AND NOT APPLE)
        set(CPACK_IFW_TARGET_DIRECTORY "~/.local/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    endif()
    if(WIN32)
        set(CPACK_IFW_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/icon.ico")
    elseif(APPLE)
        set(CPACK_IFW_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/icon.icns")
    endif()
    set(CPACK_IFW_PACKAGE_WINDOW_ICON "${CMAKE_SOURCE_DIR}/installer/icons/WindowIcon.png")
    set(CPACK_IFW_PACKAGE_LOGO "${CMAKE_SOURCE_DIR}/installer/icons/Logo.png")
    set(CPACK_IFW_PACKAGE_START_MENU_DIRECTORY "")
    set(CPACK_IFW_PACKAGE_RUN_PROGRAM "@TargetDir@/bin/muziko")
    set(CPACK_IFW_PACKAGE_CONTROL_SCRIPT "${CMAKE_SOURCE_DIR}/installer/controller.qs")

    if(WIN32)
        # set up windeployqt
        find_program(WINDEPLOYQT windeployqt HINTS "${_qt_bin_dir}")
        set(WINDEPLOYQT_COMPONENT "${PROJECT_NAME}")
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/windeployqt.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/windeployqt.cmake" @ONLY)
        set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_CURRENT_BINARY_DIR}/windeployqt.cmake")
    endif()

    include(CPack)
    include(CPackIFW)

    cpack_add_component(TimeLight
        DISPLAY_NAME "${PROJECT_NAME}"
        DESCRIPTION "A button for controlling time tracker services from your system tray."
        REQUIRED
    )

    cpack_ifw_configure_component(TimeLight
        FORCED_INSTALLATION
        LICENSES "License" LICENSE
        SCRIPT "${CMAKE_SOURCE_DIR}/installer/components.qs"
        USER_INTERFACES "${CMAKE_SOURCE_DIR}/installer/pages/InstallType.ui"
    )
endif()
