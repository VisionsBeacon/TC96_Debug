cmake_minimum_required(VERSION 3.14)

project(Executor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE Executor_src "./*.cpp" "./*.h")

add_library(Executor STATIC ${Executor_src})

# 添加 third_party 的头文件目录，确保能包含 third_party 的头文件
target_include_directories(Executor PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/include/win32
    ${CMAKE_SOURCE_DIR}/third_party/include
)

# 添加 eds 的头文件目录，确保能包含 eds 的头文件
target_include_directories(Executor PRIVATE ${CMAKE_SOURCE_DIR}/eds)

# 添加 eds 的头文件目录，确保能包含 eds 的头文件
target_include_directories(Executor PRIVATE ${CMAKE_SOURCE_DIR}/Config)

target_link_libraries(Executor
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
    PRIVATE Qt${QT_VERSION_MAJOR}::Gui
    PRIVATE third_party
    PRIVATE eds
    PRIVATE Configplugin
)
