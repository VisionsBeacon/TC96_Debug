qt_add_library(Config STATIC)

#将 Config.qml 被注册为一个QML单例类型
set_source_files_properties(Config.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE true
)

# 添加头文件，方便包含头文件
# include_directories(
#     ../third_party/include/win32/
#     ../third_party/include/
# )

qt_add_qml_module(Config
    URI "Config"
    OUTPUT_DIRECTORY Config
    QML_FILES
        "Config.qml"

    SOURCES datahandler.h datahandler.cpp
    SOURCES registertoqml.h registertoqml.cpp
)

#链接库
target_link_libraries(Config PUBLIC
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
    PRIVATE Qt${QT_VERSION_MAJOR}::Sql
    PRIVATE Qt${QT_VERSION_MAJOR}::Gui
    third_party
    Executor
)
