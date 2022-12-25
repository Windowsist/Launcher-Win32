# Launcher

start process according to configuration file(ini file encoding in UTF-8)

this is an example (not all parameters are needed):

``` ini
;UTF-8 or UTF16 LE
[LaunchApp]
AppPath=%systemroot%\system32\cmd.exe
WorkingDirectory=%LauncherDir%
CommandLine="%LauncherDir%\myprog.exe" /k set
[EnvironmentVariables]
;remove EnvironmentVariable "windir"
windir
;%LauncherDir% means the directory that contains the launcher executable
_LauncherDir=%LauncherDir%
Path=C:\my\path;%Path%
```
