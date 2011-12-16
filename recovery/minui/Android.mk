LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

ifeq ($(HAVE_FSL_EPDC_FB),true)
LOCAL_CFLAGS += -DFSL_EPDC_FB
endif

LOCAL_SRC_FILES := graphics.c events.c resources.c

LOCAL_C_INCLUDES +=\
    external/libpng\
    external/zlib

LOCAL_MODULE := libminui

include $(BUILD_STATIC_LIBRARY)
