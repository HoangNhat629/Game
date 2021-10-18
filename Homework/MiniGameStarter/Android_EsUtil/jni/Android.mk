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

LOCAL_MODULE := esUtil

SRC_PATH := ./../../Utilities
INC_PATH := ./../Include

LOCAL_CFLAGS := -Werror -DANDROID

LOCAL_C_INCLUDES := $(INC_PATH) \

LOCAL_SRC_FILES :=  $(SRC_PATH)/esShader.cpp \
					$(SRC_PATH)/Math.cpp \
					$(SRC_PATH)/stdafx.cpp \
					$(SRC_PATH)/TGA.cpp

LOCAL_LDLIBS    := -llog -lGLESv2

include $(BUILD_STATIC_LIBRARY)
