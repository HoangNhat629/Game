cls

@echo off

set CURRENT_DIR=%CD%

set JAVA_HOME=C:\Program\Java\jdk1.6.0_45

set CYGWIN_PATH=C:\Program\Cygwin
set CYGWIN_BIN=%CYGWIN_PATH%\bin
set ANT_HOME=C:\Program\apache-ant-1.8.0

set ANDROID_NDK_HOME=C:\DevTools\android-ndk-r10d


set PRJ_DIR=%CD%

set OBJ_DIR=%PRJ_DIR%\obj
set BIN_DIR=%PRJ_DIR%\bin
set SO_DIR=%PRJ_DIR%\libs\armeabi

echo.
echo ************** build .a file ********************

if exist %CURRENT_DIR%\obj rmdir /S /Q %CURRENT_DIR%\obj
if exist %CURRENT_DIR%\libs\armeabi\*.so del /S /Q %CURRENT_DIR%\libs\armeabi\*.so
if exist %CURRENT_DIR%\libs\armeabi\*.a del /S /Q %CURRENT_DIR%\libs\armeabi\*.a

rem echo aaa call %CYGWIN_BIN%\bash.exe -i %ANDROID_NDK_HOME%/ndk-build
rem call %CYGWIN_BIN%\bash.exe -i 
call %ANDROID_NDK_HOME%/ndk-build

if exist %CURRENT_DIR%\obj\local\armeabi\*.a (
	echo ### buid .a successed ....
	cd %CURRENT_DIR%
	copy %CURRENT_DIR%\obj\local\armeabi\*.a %CURRENT_DIR%\libs\armeabi
) else (
	echo.
	echo ### build .a fail, recheck ...
)

