#pragma once

#include "targetver.h"

#ifndef ANDROID
#include "esUtil.h"
#endif //android
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

// Create Win32 instance and window
GLboolean WinCreate(ESContext* esContext, const char* title);

// Start main windows loop
void WinLoop(ESContext* esContext);

