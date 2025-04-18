cmake_minimum_required(VERSION 3.28)

project(
  XPlayerpp
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg)

add_subdirectory(./libdependencies)
include_directories(./libdependencies/include)
include_directories(./)

set(PROJECT_SOURCES main.cpp mainwindow.cpp mainwindow.h mainwindow.ui)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(
    XPlayerpp
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    devicemanager.h
    devicemanager.cpp
    devicemanager.ui
		clickableprogressbar.h
		clickableprogressbar.cpp
    audioorbitcontroller.h
    audioorbitcontroller.cpp
    audioorbitcontroller.ui
    resources.qrc
    newaudioorbitdialog.h
    newaudioorbitdialog.cpp
    newaudioorbitdialog.ui)
  # Define target properties for Android with Qt 6 as: set_property(TARGET
  # XPlayerpp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  # ${CMAKE_CURRENT_SOURCE_DIR}/android) For more information, see
  # https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(XPlayerpp SHARED ${PROJECT_SOURCES})
    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(XPlayerpp ${PROJECT_SOURCES})
  endif()
endif()

target_link_libraries(XPlayerpp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg xaudio)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. If
# you are developing for iOS or macOS you should consider setting an explicit,
# fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.XPlayerpp)
endif()
set_target_properties(
  XPlayerpp
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

include(GNUInstallDirs)
install(
  TARGETS XPlayerpp
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(XPlayerpp)
endif()
