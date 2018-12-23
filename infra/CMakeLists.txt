# 指定cmake版本号
cmake_minimum_required(VERSION 3.9)

# 项目名称 支持C和C++，不写的话默认支持所有语言
project(challenge C CXX)
message(STATUS "project root directory: ${PROJECT_SOURCE_DIR}")

# 指定编译类型 CMAKE_BUILD_TYPE_DEBUG or CMAKE_BUILD_TYPE_RELEASE
set(CMAKE_BUILD_TYPE_DEBUG)

# 指定编译器及编译选项，CMAKE_C_FLAGS_DEBUG为C编译器
#set(CMAKE_CXX_FLAG_DEBUG "-g -Wall -O3")

# 添加类似于makefile的-D宏定义
#ADD_DEFINITIONS(
#-O3 -g -W -Wall
# -Wunused-variable -Wunused-parameter -Wunused-function -Wunused
# -Wno-deprecated -Woverloaded-virtual -Wwrite-strings
# -D__WUR= -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DTIXML_USE_STL
#)
add_definitions(-O3 -g -W -Wall)
add_definitions(-DUSE_COLORFULPRINT)
add_definitions(-DMODULE_NAME="Infra")

# 使用运行test_main
option(use_gtest "use google test, run test_main.cpp" ON)

# 头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)
#include_directories(${PROJECT_SOURCE_DIR}/include/json)
include_directories(${PROJECT_SOURCE_DIR}/src)

# 源文件目录
# 是否使用gtest的测试代码
if (use_gtest)
    AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/tests DIR_SRCS)
    #message("all source files in tests directory is ${DIR_SRCS}")
else()
    AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src DIR_SRCS)
endif()
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/application DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/os DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/utils DIR_SRCS)
message("all source files is ${DIR_SRCS}")

# 库文件路径
link_directories(${PROJECT_SOURCE_DIR}/libs)

# 可执行文件
add_executable(${PROJECT_NAME} ${DIR_SRCS})

# 链接静态库
TARGET_LINK_LIBRARIES(${PROJECT_NAME} gtest pthread jsoncpp)
