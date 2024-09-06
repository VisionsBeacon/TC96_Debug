cmake_minimum_required(VERSION 3.14)

project(third_party LANGUAGES C)

file(GLOB_RECURSE third_party_src "./*.c" "./*.h")

# 添加头文件，方便包含头文件
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/win32
)

add_library(third_party STATIC
    ${third_party_src}
)
