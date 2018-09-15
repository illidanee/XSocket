LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := U3DPlugin

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
					$(LOCAL_PATH)/../../XSrc

LOCAL_SRC_FILES :=	../../U3DPlugin/Unity3DPlugin.cpp \
					../../XSrc/XBuffer.cpp \
					../../XSrc/XByteStream.cpp \
					../../XSrc/XRecvByteStream.cpp \
					../../XSrc/XSendByteStream.cpp \
					../../XSrc/XLog.cpp \
					../../XSrc/XMemory.cpp \
					../../XSrc/XMemoryBlock.cpp \
					../../XSrc/XMemoryManager.cpp \
					../../XSrc/XMemoryPool.cpp \
					../../XSrc/XClient.cpp \
					../../XSrc/XNet.cpp \
					../../XSrc/XTCPClient.cpp \
					../../XSrc/XSignal.cpp \
					../../XSrc/XThread.cpp \
					../../XSrc/XTimer.cpp \
					../../XSrc/XFdSet.cpp \
					../../XSrc/XConfig.cpp

include $(BUILD_SHARED_LIBRARY)