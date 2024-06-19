cmake_minimum_required(VERSION 3.27)

project(
  RemoteControl
  VERSION 0.1
  LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Boost REQUIRED COMPONENTS system filesystem thread)
find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(
  RemoteControl
  main.cpp
  msgnode.h
  msgnode.cpp
  centercontrol.h
  centercontrol.cpp
  csession.h
  csession.cpp
  cmanagement.h
  cmanagement.cpp
  data.h
  data.cpp
  viewcontrol.h
  viewcontrol.cpp
  pevent.h
  pevent.cpp
  Main.qml
  MainPage.qml
  Dialogs.qml
  Actions.qml
  Content.qml
  imageprovider.h imageprovider.cpp
  viewbridge.h viewbridge.cpp)

target_include_directories(RemoteControl PRIVATE .)

target_compile_features(RemoteControl PRIVATE cxx_std_20)

target_link_libraries(RemoteControl PRIVATE Qt6::Widgets)
target_link_libraries(RemoteControl PRIVATE Qt6::Quick)
target_link_libraries(RemoteControl PRIVATE Qt6::Gui)
target_link_libraries(RemoteControl PRIVATE Qt6::Core)
target_link_libraries(RemoteControl PRIVATE Boost::system Boost::filesystem
                                            Boost::thread)

set_target_properties(
  RemoteControl
  PROPERTIES AUTORCC TRUE
             MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
             MACOSX_BUNDLE TRUE
             WIN32_EXECUTABLE TRUE)

include(GNUInstallDirs)
install(
  TARGETS RemoteControl
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

qt_finalize_executable(RemoteControl)

# Define target properties for Android with Qt 6 as: set_property(TARGET
# RemoteControl APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# ${CMAKE_CURRENT_SOURCE_DIR}/android) For more information, see
# https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

# target_sources(RemoteControl PRIVATE main.cpp widget.cpp widget.h @if true
# widget.ui @endif @if false
#
# @endif
#
# PUBLIC
#
# )

# qt_standard_project_setup()     #set(CMAKE_AUTOUIC ON)  set(CMAKE_AUTOMOC ON)
# set_target_properties(RemoteControl PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER
# my.example.com MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
# MACOSX_BUNDLE_SHORT_VERSION_STRING
# ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE TRUE
# WIN32_EXECUTABLE TRUE )
#
# install(TARGETS RemoteControl BUNDLE DESTINATION . LIBRARY DESTINATION
# ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} )
#
# if(QT_VERSION_MAJOR EQUAL 6) qt_finalize_executable(RemoteControl) endif()

# qt_add_executable(RemoteControl)  //Creates and finalizes an application
# target of a platform-specific type. This command performs the following tasks:

# 1. Create a CMake target of the appropriate type for the target platform.
# 2. Link the target to the Qt::Core library.
# 3. Handle finalization of the CMake target.

# cmake_minimum_required(VERSION 3.5)
#
# project(RemoteControl VERSION 0.1 LANGUAGES CXX)
#
# set(CMAKE_AUTOUIC ON) set(CMAKE_AUTOMOC ON) set(CMAKE_AUTORCC ON)
#
# set(CMAKE_CXX_STANDARD 20) set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
# @if false find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets
# LinguistTools) find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets
# LinguistTools)
#
# set(TS_FILES ) @else find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
# Widgets) find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
# @endif
#
# set(PROJECT_SOURCES main.cpp widget.cpp widget.h @if true widget.ui @endif @if
# false ${TS_FILES} @endif )
#
# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6) qt_add_executable(RemoteControl
# MANUAL_FINALIZATION ${PROJECT_SOURCES} ) # Define target properties for
# Android with Qt 6 as: #    set_property(TARGET RemoteControl APPEND PROPERTY
# QT_ANDROID_PACKAGE_SOURCE_DIR # ${CMAKE_CURRENT_SOURCE_DIR}/android) # For
# more information, see
# https://doc.qt.io/qt-6/qt-add-executable.html#target-creation @if false
#
# qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES}) @endif else()
# if(ANDROID) add_library(RemoteControl SHARED ${PROJECT_SOURCES} ) # Define
# properties for Android with Qt 5 after find_package() calls as: #
# set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android") else()
# add_executable(RemoteControl ${PROJECT_SOURCES} ) endif() @if false
#
# qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES}) @endif
# endif()
#
# target_link_libraries(RemoteControl PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
#
# # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. #
# If you are developing for iOS or macOS you should consider setting an #
# explicit, fixed bundle identifier manually though. if(${QT_VERSION}
# VERSION_LESS 6.1.0) set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER
# com.example.RemoteControl) endif() set_target_properties(RemoteControl
# PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
# MACOSX_BUNDLE_SHORT_VERSION_STRING
# ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE TRUE
# WIN32_EXECUTABLE TRUE )
#
# include(GNUInstallDirs) install(TARGETS RemoteControl BUNDLE DESTINATION .
# LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION
# ${CMAKE_INSTALL_BINDIR} )
#
# if(QT_VERSION_MAJOR EQUAL 6) qt_finalize_executable(RemoteControl) endif()
