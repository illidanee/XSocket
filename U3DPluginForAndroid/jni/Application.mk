#���STL֧��
#APP_STL := gnustl_static
APP_STL := c++_shared
#���C++11֧��

#ʹNDKʹ��android21�汾�ĺ���
APP_PLATFORM := android-21

APP_CPPFLAGS := -std=c++11 -Wno-error=format-security


#��������֧�ֵ�CPU(ָ����ܹ�)Ŀ���ļ�
#APP_ABI := arm64-v8a armeabi-v7a armeabi
APP_ABI := all