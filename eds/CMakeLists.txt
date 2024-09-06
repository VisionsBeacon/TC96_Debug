cmake_minimum_required(VERSION 3.14)

project(eds LANGUAGES C)

file(GLOB_RECURSE eds_src "./*.c" "./*.h")

# 添加头文件，方便包含头文件
include_directories(
    ../third_party/include/win32
    ../third_party/include/
)

add_library(eds STATIC
    ${eds_src}
)

target_link_libraries(eds
    third_party
)
