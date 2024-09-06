cmake_minimum_required(VERSION 3.20)


if (HCCONTROLS_BUILD_STATIC_LIB AND (QT_VERSION VERSION_GREATER_EQUAL "6.2"))
    project(hccontrols VERSION 1.0)
else()
    project(hccontrolsplugin VERSION 1.0)
endif()

#配置通用编译
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (HCCONTROLS_BUILD_STATIC_LIB)
    add_definitions(-DHCCONTROLS_BUILD_STATIC_LIB)
endif()

#导入Qt相关依赖包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Qml Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Qml Svg)


qt_standard_project_setup()


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cpp)

#遍历所有Cpp文件
file(GLOB_RECURSE CPP_FILES *.cpp *.h)
foreach(filepath ${CPP_FILES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND sources_files ${filename})
endforeach(filepath)


#HcControlsplugin.HcControlsplugin.h，这些只要Qt5使用，Qt6不需要
list(REMOVE_ITEM sources_files HcControlsplugin.h HcControlsplugin.cpp)
if (NOT HCCONTROLS_BUILD_STATIC_LIB)
    list(REMOVE_ITEM sources_files HcControls.h HcControls.cpp)
endif()

#遍历所有qml文件
file(GLOB_RECURSE QML_PATHS *.qml)
foreach(filepath ${QML_PATHS})
    #if(${filepath} MATCHES "Qt${QT_VERSION_MAJOR}/")
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND qml_files ${filename})
    #endif()
endforeach(filepath)

#遍历所有资源文件
file(GLOB_RECURSE RES_PATHS *.png *.jpg *.svg *.ico *.ttf *.webp *.js)
foreach(filepath ${RES_PATHS})
    #if(${filepath} MATCHES "Qt${QT_VERSION_MAJOR}/")
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND resource_files ${filename})
    #endif()
endforeach(filepath)

#修改资源文件导出路径
foreach(filepath IN LISTS qml_files resource_files)
    #string(REPLACE "Qt${QT_VERSION_MAJOR}/imports/HcControls/" "" filename ${filepath})
    string(REPLACE "qml/" "" filename ${filepath})
    set_source_files_properties(${filepath} PROPERTIES QT_RESOURCE_ALIAS ${filename})

    #标记单例
    string(REGEX MATCH "HcPcrHelper|Constants|HcMessageDlg" RESULT ${filepath})
    if (RESULT)
        set_source_files_properties(${filepath} PROPERTIES QT_QML_SINGLETON_TYPE TRUE)
    endif()
endforeach()


if (HCCONTROLS_BUILD_STATIC_LIB)
    set(LIB_TYPE "STATIC")
else()
    set(LIB_TYPE "SHARED")
endif()

if (HCCONTROLS_BUILD_STATIC_LIB)
    set(PLUGIN_TARGET_NAME "")
else()
    #如果是动态库，则使用插件目标作为其自己的支持目标来定义 QML 模块，在这种情况下，模块必须在运行时动态加载，并且不能由其他目标直接链接到
    set(PLUGIN_TARGET_NAME ${PROJECT_NAME})
endif()


#如果是Windows平台，则生成rc文件
set(HCCONTROLS_VERSION_RC_PATH "")
if(WIN32)
    set(HCCONTROLS_VERSION_RC_PATH ${CMAKE_BINARY_DIR}/version_${PROJECT_NAME}.rc)
    configure_file(
        ${HCCONTROLS_DIRECTORY}/.cmake/version_dll.rc.in
        ${HCCONTROLS_VERSION_RC_PATH}
    )
endif()


#如果是Qt6.2版本以上，则使用qt_add_library，qt_add_qml_module函数添加资源文件
if(HCCONTROLS_BUILD_STATIC_LIB)
    set(HCCONTROLS_QML_PLUGIN_DIRECTORY ${CMAKE_BINARY_DIR}/HcControls)
endif()

qt_add_library(${PROJECT_NAME} ${LIB_TYPE})
qt_add_qml_module(${PROJECT_NAME}
    PLUGIN_TARGET ${PLUGIN_TARGET_NAME}
    OUTPUT_DIRECTORY ${HCCONTROLS_QML_PLUGIN_DIRECTORY}
    VERSION 1.0
    URI "HcControls"
    #修改qmltypes文件名称。默认HcControlsplugin.qmltypes，使用默认名称有时候import HcControls 1.0会爆红，所以修改成plugins.qmltypes
    TYPEINFO "plugins.qmltypes"
    SOURCES ${sources_files} ${HCCONTROLS_VERSION_RC_PATH}
    QML_FILES ${qml_files}
    RESOURCES ${resource_files}
    RESOURCE_PREFIX "/qt/qml"
)


target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    #导入qrcode配置文件
    HAVE_CONFIG_H
)

if(WIN32)
    #去掉mingw生成的动态库libxxx前缀lib，不去掉前缀会导致 module "HcControls" plugin "HCCONTROLSplugin" not found
    if(MINGW)
        set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
    endif()

    #MSVC Debug 添加后缀d，与Qt插件风格保持一致
    if(MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
    endif()
endif()



#链接库
target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::CorePrivate
    Qt${QT_VERSION_MAJOR}::QuickPrivate
    Qt${QT_VERSION_MAJOR}::QmlPrivate
    Qt${QT_VERSION_MAJOR}::Svg
)

#安装
install(DIRECTORY ${HCCONTROLS_QML_PLUGIN_DIRECTORY} DESTINATION ${CMAKE_INSTALL_PREFIX}/imports)
