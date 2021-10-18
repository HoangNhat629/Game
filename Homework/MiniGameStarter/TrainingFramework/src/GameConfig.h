#pragma once

#include <string>
#include "targetver.h"
#include "CMath.h"
#include "ft2build.h"
#include FT_FREETYPE_H

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef ANDROID	
#include <stdio.h>
#include <tchar.h>
#endif

// define game debug  -------------------	1: yes	0: no
#define GAME_DEBUG 1

class Globals
{
public:
	static int screenWidth;
	static int screenHeight;
};

// define value
#define PI 3.1415926535f
#define MAX_DEGREE 360
// define vetex 
#define VETEX_POSITION		(char*)0
#define VETEX_NORM			(char*)0+(sizeof(Vector3))
#define VETEX_BITTANGEN		(char*)0+2*(sizeof(Vector3))
#define VETEX_TANGEN		(char*)0+3*(sizeof(Vector3))
#define VETEX_UV			(char*)0+4*(sizeof(Vector3))

// define type of Models
#define NONE	111 
#define RAW		222 
#define NFG		333

#ifdef WIN32

#define FSCANF fscanf

#elif ANDROID

#define FSCANF fscanf

#endif

// for debug game

#ifdef WIN32 

#define LOGI(...) printf(__VA_ARGS__);
#define LOGE(...) printf(__VA_ARGS__);
#define LOGV(...) printf(__VA_ARGS__);
#define LOGD(...) printf(__VA_ARGS__);
#define LOGW(...) printf(__VA_ARGS__);
#define LOGF(...) printf(__VA_ARGS__);

#elif ANDROID

#include <android/log.h>
#define  LOG_TAG    "GAME"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__)

#endif

#ifndef GAME_DEBUG
#define LOGI(...)
#define LOGE(...)
#define LOGV(...)
#define LOGD(...)
#define LOGW(...)
#define LOGF(...)
#endif

// KEY define 
#ifdef WIN32

#define KEY_BACK			VK_ESCAPE
#define KEY_LEFT			VK_LEFT
#define KEY_RIGHT			VK_RIGHT
#define KEY_UP				VK_UP
#define KEY_DOWN			VK_DOWN
#define KEY_MOVE_LEFT		'A'
#define KEY_MOVE_RIGHT		'D'
#define KEY_MOVE_FORWARD	'W'
#define KEY_MOVE_BACKWARD	'S'

#endif
