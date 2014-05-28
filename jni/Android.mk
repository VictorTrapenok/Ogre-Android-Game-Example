LOCAL_PATH := $(call my-dir)
    include $(CLEAR_VARS)
    LOCAL_MODULE    := OgreGLES2Sample

    LOCAL_LDLIBS	:= -landroid -lc -lm -ldl -llog -lEGL -lGLESv2
    LOCAL_LDLIBS	+= -L$(LOCAL_PATH)/../../../Ogre/lib/armeabi-v7a -L$(LOCAL_PATH)/../../../Dependencies/lib/armeabi-v7a
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libPlugin_ParticleFXStatic.a 
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libPlugin_OctreeSceneManagerStatic.a
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libRenderSystem_GLES2Static.a
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libOgreRTShaderSystemStatic.a
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libOgreOverlayStatic.a
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libOgreMainStatic.a
    
    LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libzzip.a
	LOCAL_LDLIBS	+= -lz
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libFreeImage.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libfreetype.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libOIS.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libmesa.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libglsl_optimizer.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libsupc++.a
	LOCAL_LDLIBS	+= $(LOCAL_PATH)/../libs/armeabi-v7a/libstdc++.a ./obj/local/armeabi-v7a/libcpufeatures.a
    LOCAL_STATIC_LIBRARIES := android_native_app_glue cpufeatures
    
    LOCAL_CFLAGS := -DGL_GLEXT_PROTOTYPES=1 -I$(LOCAL_PATH)/../../../Ogre/include/Build -I$(LOCAL_PATH)/../../../Ogre/include/OgreMain -I$(LOCAL_PATH)/../../../Ogre/include/RenderSystems/GLES2
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/../../../Ogre/include/RenderSystems/GLES2/EGL -I$(ANDROID_NDK)/sources/cpufeatures -I$(LOCAL_PATH)/../../../Ogre/include/Components/Overlay -I$(LOCAL_PATH)/../../../Ogre/include/Components/RTShaderSystem 
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/../../../Ogre/include/PlugIns/ParticleFX -I$(LOCAL_PATH)/../../../Ogre/include/PlugIns/OctreeSceneManager 
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/../../../Dependencies/include -I$(LOCAL_PATH)/../../../Dependencies/include/OIS 
    LOCAL_CFLAGS += -fexceptions -frtti -x c++ -D___ANDROID___ -DANDROID -DZZIP_OMIT_CONFIG_H -DUSE_RTSHADER_SYSTEM=1 
    
    LOCAL_PATH := ./jni
    LOCAL_SRC_FILES := Main.cpp 
        
    include $(BUILD_SHARED_LIBRARY) 
      
    $(call import-module,android/cpufeatures) 
    $(call import-module,android/native_app_glue) 
    