LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE			:= arm
LOCAL_C_INCLUDES		:= $(LOCAL_PATH)
LOCAL_MODULE    		:= Chapter5
LOCAL_SRC_FILES 		:= Chapter5.cpp \
						   Chapter5App.cpp \
						   Chapter5Task.cpp \
						   Framework/Application/Application.cpp \
						   Framework/Kernel/Kernel.cpp \
						   Framework/Platform/Android.cpp \
						   Framework/Renderer/Renderer.cpp \
						   Framework/Renderer/Geometry/Geometry.cpp \
						   Framework/Renderer/Shaders/Shader.cpp \
						   Framework/Renderer/Shaders/BasicShader.cpp \
						   Framework/Renderer/Shaders/TextureShader.cpp \
						   Framework/Renderer/Textures/Texture.cpp \
						   Framework/Utilities/File.cpp \
						   Framework/Utilities/TGAFile.cpp \
						   Framework/Utilities/Timer.cpp
LOCAL_LDLIBS    		:= -llog -landroid -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES	:= android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
