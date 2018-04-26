LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := U3DPlugin

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
					$(LOCAL_PATH)/../../XSrc \
					$(LOCAL_PATH)/../../XSrc/Buffer \
					$(LOCAL_PATH)/../../XSrc/ByteStream \
					$(LOCAL_PATH)/../../XSrc/Log \
					$(LOCAL_PATH)/../../XSrc/Memory \
					$(LOCAL_PATH)/../../XSrc/Object \
					$(LOCAL_PATH)/../../XSrc/Protocol \
					$(LOCAL_PATH)/../../XSrc/Server \
					$(LOCAL_PATH)/../../XSrc/Signal \
					$(LOCAL_PATH)/../../XSrc/Task \
					$(LOCAL_PATH)/../../XSrc/Thread \
					$(LOCAL_PATH)/../../XSrc/Timer

LOCAL_SRC_FILES :=	../../U3DPlugin/Unity3DPlugin.cpp \
					../../XSrc/Buffer/XBuffer.cpp \
					../../XSrc/ByteStream/XByteStream.cpp \
					../../XSrc/ByteStream/XRecvByteStream.cpp \
					../../XSrc/ByteStream/XSendByteStream.cpp \
					../../XSrc/Log/XLog.cpp \
					../../XSrc/Memory/XMemory.cpp \
					../../XSrc/Memory/XMemoryBlock.cpp \
					../../XSrc/Memory/XMemoryManager.cpp \
					../../XSrc/Memory/XMemoryPool.cpp \
					../../XSrc/Server/XClient.cpp \
					../../XSrc/Server/XNet.cpp \
					../../XSrc/Server/XServer.cpp \
					../../XSrc/Server/XTCPClient.cpp \
					../../XSrc/Server/XTCPServer.cpp \
					../../XSrc/Signal/XSignal.cpp \
					../../XSrc/Task/XTaskServer.cpp \
					../../XSrc/Thread/XThread.cpp \
					../../XSrc/Timer/XTimer.cpp

include $(BUILD_SHARED_LIBRARY)