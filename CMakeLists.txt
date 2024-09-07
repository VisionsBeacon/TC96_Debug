cmake_minimum_required(VERSION 3.16)

project(TC96_Debug VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Sql Widgets Network Gui)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appTC96_Debug
    main.cpp
)

qt_add_qml_module(appTC96_Debug
    URI TC96_Debug
    VERSION 1.0

    QML_FILES
        "Main.qml"
        "LoginPage.qml"
        "ActionsPage.qml"
        "DevicesPage.qml"
        "OperatorPage.qml"
        "ParamsPage.qml"
        "CurvePage.qml"
        "LogPage.qml"
        "ActPage.qml"
        "TestLogPage.qml"
        "OperateLogPage.qml"
        "ErrorLogPage.qml"

    RESOURCES
        "Icons/logo_hcsci.png"
        "Icons/backgroud.png"
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appTC96_Debug PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appTC96_Debug
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_subdirectory(log)
add_subdirectory(third_party)
add_subdirectory(eds)
add_subdirectory(Executor)
add_subdirectory(Config)
add_subdirectory(CustomControls)
add_subdirectory(HcControls)



# 添加 third_party 的头文件目录，确保能包含 third_party 的头文件
target_include_directories(appTC96_Debug PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/include/win32
    ${CMAKE_SOURCE_DIR}/third_party/include
)

# 添加 eds 的头文件目录，确保能包含 eds 的头文件
target_include_directories(appTC96_Debug PRIVATE ${CMAKE_SOURCE_DIR}/eds)

target_link_libraries(appTC96_Debug
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Sql
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::Network
    PRIVATE Qt6::Gui
    Log
    third_party
    eds
    Executor
    Configplugin
    CustomControlsplugin
)

include(GNUInstallDirs)
install(TARGETS appTC96_Debug
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
