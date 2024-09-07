cmake_minimum_required(VERSION 3.14)

project(eds LANGUAGES C)

file(GLOB_RECURSE eds_src "./*.c" "./*.h")

add_library(eds STATIC
    ${eds_src}
)

# 添加第三方库头文件，方便包含头文件
target_include_directories(eds PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/include/win32
    ${CMAKE_SOURCE_DIR}/third_party/include
)

target_link_libraries(eds
    PRIVATE third_party
)
