# ProcessTools

## Launcher

    start process according to configuration file(ini file encoding in UTF-8)

this is an example (not all parameters are needed):

    ``` ini
[LaunchApp]
AppPath=%systemroot%\system32\cmd.exe
WorkingDirectory=%LauncherDir%
CommandLine=""%LauncherDir%\myprog.exe"" /k dir
[EnvironmentVariables]
_LauncherDir=%LauncherDir%
Path=C:\my\path;%Path%
    ```
