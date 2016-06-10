LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2d_facebookx_static

LOCAL_MODULE_FILENAME := libfacebookx

FILE_LIST := $(wildcard $(LOCAL_PATH)/Classes/*.cpp) 
FILE_LIST += $(wildcard $(LOCAL_PATH)/Classes/js-bindings/*.cpp) 
FILE_LIST += $(wildcard $(LOCAL_PATH)/platforms/android/*.cpp) 

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/Classes \
					$(LOCAL_PATH)/Classes/js-bindings \
                    $(LOCAL_PATH)/platforms/android

LOCAL_STATIC_LIBRARIES := cocos2d_js_static

LOCAL_EXPORT_CFLAGS := -DCOCOS2D_JAVASCRIPT

include $(BUILD_STATIC_LIBRARY)

$(call import-module, scripting/js-bindings/proj.android)