LOCAL_PATH := $(call my-dir)
ROOT_PATH := $(LOCAL_PATH)


########################################################
## libev
########################################################

include $(CLEAR_VARS)

LOCAL_MODULE := libev
LOCAL_CFLAGS += -DNDEBUG -DHAVE_CONFIG_H \
				-I$(LOCAL_PATH)/libev
LOCAL_SRC_FILES := \
	libev/ev.c \
	libev/event.c

include $(BUILD_STATIC_LIBRARY)

########################################
## libjni
########################################

include $(CLEAR_VARS)

LOCAL_MODULE := libjni

LOCAL_C_INCLUDES := $(LOCAL_PATH)/libjni

SOURCES := jni_caller.cpp

LOCAL_SRC_FILES := $(addprefix libjni/, $(SOURCES))

LOCAL_CFLAGS := -Werror=return-type

LOCAL_LDLIBS := -ldl -llog

LOCAL_STATIC_LIBRARIES := libev

include $(BUILD_SHARED_LIBRARY)