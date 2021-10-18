@echo off
call #config.bat
Echo.
Echo ================ BUILD NATIVE ===================
Echo.
call %ANDROID_NDK_HOME%/ndk-build

Echo.
Echo.
Echo ================ BUILD ANDROID ===================
Echo.
call %ANT_HOME%\bin\ant debug

Echo.
Echo.
Echo ================ INSTALL ===================
Echo.
call adb install -r .\bin\android-debug.apk

Echo.
Echo ================ DONE ===================
Echo.