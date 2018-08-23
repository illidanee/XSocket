#添加STL支持
#APP_STL := gnustl_static
APP_STL := c++_shared
#添加C++11支持

#使NDK使用android21版本的函数
APP_PLATFORM := android-21

APP_CPPFLAGS := -std=c++11 -Wno-error=format-security


#编译所有支持的CPU(指令集、架构)目标文件
#APP_ABI := arm64-v8a armeabi-v7a armeabi
APP_ABI := all