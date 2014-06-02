LOCAL_PATH := $(call my-dir)
    include $(CLEAR_VARS)
    LOCAL_MODULE    := OgreGLES2Sample

    LOCAL_LDLIBS	:= -landroid -lc -lm -ldl -llog -lEGL -lGLESv2
	LOCAL_LDLIBS	+= -lz
    LOCAL_LDLIBS	+= -L$(LOCAL_PATH)/../../../Ogre/lib/armeabi-v7a
    LOCAL_LDLIBS	+= -L$(LOCAL_PATH)/../../../Dependencies/lib/armeabi-v7a
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libPlugin_ParticleFXStatic.a 
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libPlugin_OctreeSceneManagerStatic.a
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libRenderSystem_GLES2Static.a
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libOgreRTShaderSystemStatic.a
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libOgreOverlayStatic.a
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libOgreMainStatic.a
    
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libzzip.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libFreeImage.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libfreetype.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libOIS.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libmesa.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libglsl_optimizer.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libsupc++.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libstdc++.a
	LOCAL_LDLIBS	+= ./obj/local/armeabi-v7a/libcpufeatures.a
    LOCAL_STATIC_LIBRARIES := android_native_app_glue cpufeatures
    
    LOCAL_CFLAGS := -DGL_GLEXT_PROTOTYPES=1
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/Build
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/OgreMain
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/OgreMain/Android/
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/RenderSystems/GLES2
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/RenderSystems/GLES2/EGL
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/RenderSystems/GLES2/EGL/Android
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/RenderSystems/GLES2/EGL/WIN32
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/RenderSystems/GLES2/EGL/X11
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/Components/Overlay
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/Components/RTShaderSystem
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/PlugIns/ParticleFX
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/PlugIns/OctreeSceneManager
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/Dependencies/include
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/Dependencies/include/OIS 
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/Dependencies/include/freetype 
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/include/Dependencies/include/zzip 
     


    LOCAL_CFLAGS += -I$(ANDROID_NDK)/sources/cpufeatures
    
    LOCAL_CFLAGS += -fexceptions -frtti -x c++ -D___ANDROID___ -DANDROID -DZZIP_OMIT_CONFIG_H -DUSE_RTSHADER_SYSTEM=1 
    
    LOCAL_PATH := ./jni
    LOCAL_SRC_FILES := Main.cpp 
        
    include $(BUILD_SHARED_LIBRARY) 
      
    $(call import-module,android/cpufeatures) 
    $(call import-module,android/native_app_glue) 
    