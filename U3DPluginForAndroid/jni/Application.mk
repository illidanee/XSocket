#���STL֧��
#APP_STL := gnustl_static
APP_STL := c++_static
#���C++11֧��

#ʹNDKʹ��android21�汾�ĺ���
APP_PLATFORM := android-21

APP_CPPFLAGS := -std=c++11

#��������֧�ֵ�CPU(ָ����ܹ�)Ŀ���ļ�
#APP_ABI := arm64-v8a armeabi-v7a armeabi
#APP_ABI := all
APP_ABI := armeabi-v7a