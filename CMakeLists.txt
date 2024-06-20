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
  imageprovider.h
  imageprovider.cpp
  viewbridge.h
  viewbridge.cpp
        MyButton.qml
)

qt_add_qml_module(
  RemoteControl
  URI
  remotecontrol
  VERSION
  1.0
  QML_FILES
  Main.qml
  Dialogs.qml
  Content.qml
  Actions.qml
  MainPage.qml MyButton.qml
  SOURCES)

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
