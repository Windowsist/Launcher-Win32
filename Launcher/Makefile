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

CFLAGS=/nologo /D "_UNICODE" /D "UNICODE" /MD /std:c17 /utf-8
CXXFLAGS=/nologo /D "_UNICODE" /D "UNICODE" /MD /std:c++17 /EHsc /await /bigobj /utf-8
CPPFLAGS=/nologo /D "_UNICODE" /D "UNICODE" /MD /std:c++17 /EHsc /await /bigobj /utf-8

CFLAGS_DEBUG=/Zi
CFLAGS_RELEASE=/O2 /Ob2 /GL

CPPFLAGS_DEBUG=/Zi
CPPFLAGS_RELEASE=/O2 /Ob2 /GL

CXXFLAGS_DEBUG=/Zi
CXXFLAGS_RELEASE=/O2 /Ob2 /GL

Debug: build\Debug\Launcher.exe

build\Debug\Launcher.exe: build\Debug Windowsist.WINAPI.manifest Launcher.cpp
	$(CPP) $(CPPFLAGS) $(CPPFLAGS_DEBUG) Launcher.cpp /Fo: build\Debug\ /Fd: build\Debug\ /Fe: $@ /link /MANIFESTINPUT:Windowsist.WINAPI.manifest /MANIFEST:EMBED noarg.obj OneCore.lib

build\Debug:
	mkdir $@

Release: build\Release\Launcher.exe

build\Release\Launcher.exe: build\Release Windowsist.WINAPI.manifest Launcher.cpp
	$(CPP) $(CPPFLAGS) $(CPPFLAGS_RELEASE) Launcher.cpp /Fo: build\Release\ /Fe: $@ /link /MANIFESTINPUT:Windowsist.WINAPI.manifest /MANIFEST:EMBED noarg.obj OneCore.lib

build\Release:
	mkdir $@

clean:
	@rd /s /q build

