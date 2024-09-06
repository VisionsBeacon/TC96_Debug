cmake_minimum_required(VERSION 3.14)

project(Executor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE Executor_src "./*.cpp" "./*.h")

# 添加头文件，方便包含头文件
include_directories(
    ../third_party/include/win32/
    ../third_party/include/
)

add_library(Executor STATIC
    ${Executor_src}
)

target_link_libraries(Executor
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
    PRIVATE Qt${QT_VERSION_MAJOR}::Gui
    third_party
)
