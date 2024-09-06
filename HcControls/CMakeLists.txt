cmake_minimum_required(VERSION 3.16)

project(HcControls VERSION 1.0)

if(MSVC)
    #让Release也生成pdb文件
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

set(HCCONTROLS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
list(APPEND CMAKE_MODULE_PATH ${HCCONTROLS_DIRECTORY}/.cmake/)

include(GetGitRevisionDescription)

option(HCCONTROLS_BUILD_EXAMPLES "Build HcControls demo applications." OFF)
option(HCCONTROLS_BUILD_TEST "Build HcControls demo applications." ON)
option(HCCONTROLS_BUILD_STATIC_LIB "Build static library." OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(QT_SDK_DIR "${Qt${QT_VERSION_MAJOR}_DIR}")
cmake_path(GET QT_SDK_DIR PARENT_PATH QT_SDK_DIR)
cmake_path(GET QT_SDK_DIR PARENT_PATH QT_SDK_DIR)
cmake_path(GET QT_SDK_DIR PARENT_PATH QT_SDK_DIR)

#设置QML插件输出目录，可以通过外部设置，如果外部没有设置就默认到<QT_SDK_DIR_PATH>\qml\HcControls目录下
set(HCCONTROLS_QML_PLUGIN_DIRECTORY "" CACHE PATH "Path to HcControls plugin")
if(NOT HCCONTROLS_QML_PLUGIN_DIRECTORY)
    set(HCCONTROLS_QML_PLUGIN_DIRECTORY ${QT_SDK_DIR}/qml/HcControls)
endif()

add_subdirectory(src)

#Release也支持日志打印代码位置
#target_compile_definitions(hccontrolsplugin
#    PRIVATE
#    QT_MESSAGELOGCONTEXT
#)





message("------------------------ HcControls ------------------------")
message("Build static library.: ${HCCONTROLS_BUILD_STATIC_LIB}")
message("Path to HcControls plugin.: ${HCCONTROLS_QML_PLUGIN_DIRECTORY}")
