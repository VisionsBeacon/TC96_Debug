cmake_minimum_required(VERSION 3.14)

project(Log LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE Log_src "./*.cpp" "./*.h")

add_library(Log STATIC
    ${Log_src}
)

target_link_libraries(Log
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
    PRIVATE Qt${QT_VERSION_MAJOR}::Sql
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
)

