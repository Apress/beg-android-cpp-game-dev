LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE			:= arm
LOCAL_MODULE    		:= Chapter4
LOCAL_C_INCLUDES		:= $(LOCAL_PATH)
LOCAL_SRC_FILES 		:= Chapter4.cpp \
						   Framework/Application/Application.cpp \
						   Framework/Kernel/Kernel.cpp \
						   Framework/Platform/Android.cpp \
						   Framework/Utilities/Timer.cpp
LOCAL_LDLIBS    		:= -llog -landroid -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES	:= android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
