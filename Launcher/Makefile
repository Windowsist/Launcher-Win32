# MSVC NMake Makefile

# to build this project, do the following steps:
# 1.open Visual Studio Developer Command Prompt (for example: open "x64 Native Tools Command Prompt for VS 2019")
# 2.change the working directory to the project directory 
# 3.type "nmake Debug" or "nmake Release"
#
# if you whant to edit the source code, you can use Visual Studio Code.
#
# you can also create a project from Visual Studio IDE add the source files.

# default libraries replacements:
# vcruntime.lib -> api-ms-win-crt-private-l1-1-0.lib (extracted from api-ms-win-crt-private-l1-1-0.dll)
# msvcprt.lib -> msvcp_win.lib (extracted from msvcp_win.dll)

CFLAGS=/nologo /D "_UNICODE" /D "UNICODE" /MD /utf-8 /Wall
CXXFLAGS=/nologo /D "_UNICODE" /D "UNICODE" /MD /utf-8 /Wall
CPPFLAGS=/nologo /D "_UNICODE" /D "UNICODE" /MD /utf-8 /Wall

CFLAGS_DEBUG=/Zi /sdl # /RTC1
CFLAGS_RELEASE=/O2 /Ob2 /GL /GS-

CPPFLAGS_DEBUG=/Zi /sdl # /RTC1
CPPFLAGS_RELEASE=/O2 /Ob2 /GL /GS-

CXXFLAGS_DEBUG=/Zi /sdl # /RTC1
CXXFLAGS_RELEASE=/O2 /Ob2 /GL /GS-

RFLAGS=/nologo

Debug: build\Debug\Launcher.exe

build\Debug\Launcher.exe: build\Debug Windowsist.WINAPI.manifest Launcher.cpp build\Debug\Launcher.res
	$(CPP) $(CPPFLAGS) $(CPPFLAGS_DEBUG) Launcher.cpp /Fo: build\Debug\ /Fd: build\Debug\ /Fe: $@ /link /ENTRY:Startup /SUBSYSTEM:WINDOWS /MANIFESTINPUT:Windowsist.WINAPI.manifest /MANIFEST:EMBED build\Debug\Launcher.res OneCore.lib

build\Debug\Launcher.res: Launcher.rc
	$(RC) $(RFLAGS) /fo $@ $**

build\Debug:
	mkdir $@

Release: build\Release\Launcher.exe

build\Release\Launcher.exe: build\Release Windowsist.WINAPI.manifest Launcher.cpp build\Release\Launcher.res
	$(CPP) $(CPPFLAGS) $(CPPFLAGS_RELEASE) Launcher.cpp /Fo: build\Release\ /Fe: $@ /link /ENTRY:Startup /SUBSYSTEM:WINDOWS /MANIFESTINPUT:Windowsist.WINAPI.manifest /MANIFEST:EMBED build\Release\Launcher.res OneCore.lib

build\Release\Launcher.res: Launcher.rc
	$(RC) $(RFLAGS) /fo $@ $**

build\Release:
	mkdir $@

clean:
	@rd /s /q build

