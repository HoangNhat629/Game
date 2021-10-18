# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libesUtil
LOCAL_SRC_FILES := ./../../Android_EsUtil/libs/armeabi/libesUtil.a
include $(PREBUILT_STATIC_LIBRARY)



include $(CLEAR_VARS)
LOCAL_MODULE    := libgl2jni
SRC_PATH := $(LOCAL_PATH)/../../TrainingFramework
INC_PATH := $(LOCAL_PATH)/../../Include



LOCAL_C_INCLUDES := $(INC_PATH) \ ./../../TrainingFramework

LOCAL_CFLAGS    := -Werror \
				-DANDROID

LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(wildcard $(SRC_PATH)/*.cpp)) \
			
				
				 
				
				

LOCAL_LDLIBS    := -lGLESv2 -ldl -llog -lstdc++

LOCAL_STATIC_LIBRARIES := libesUtil \
							libfreetype

include $(BUILD_SHARED_LIBRARY)
