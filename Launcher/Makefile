build: Launcher.exe

Launcher.exe: Launcher.cpp Launcher.h
	$(CPP) $(CPPFLAGS) Launcher.cpp /link /MANIFESTINPUT:Windowsist.WINAPI.manifest /MANIFEST:EMBED noarg.obj OneCore.lib

clean:
	@for %i in (*.obj *.res *.ilk *.pdb *.exe) do @del "%i"
