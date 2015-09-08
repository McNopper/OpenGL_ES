LOCAL_PATH			:= $(call my-dir)

#
# GLUS.
#

include $(CLEAR_VARS)

LOCAL_MODULE := GLUS

LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../GLUS/Android/obj/local/armeabi/libGLUS.a

LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(PREBUILT_STATIC_LIBRARY)

#$(call import-module,android/native_app_glue)

#
# Example.
#

include $(CLEAR_VARS)

LOCAL_MODULE := Example06

# All files.

PROJECT_FILES := $(wildcard $(LOCAL_PATH)/../../src/*.c)

# Generate the final list.

PROJECT_FILES := $(PROJECT_FILES:$(LOCAL_PATH)/%=%)


LOCAL_CFLAGS := -DGLUS_ES2=1

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/../../../GLUS/src/

LOCAL_SRC_FILES := $(PROJECT_FILES)

LOCAL_LDLIBS    := -landroid -lEGL -lGLESv2

LOCAL_STATIC_LIBRARIES := GLUS

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
