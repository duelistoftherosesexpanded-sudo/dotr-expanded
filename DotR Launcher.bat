@echo off
rem Opens the DotR Launcher (tools\launcher.py): plays the mod from the project's files.
set PYW=%LOCALAPPDATA%\Programs\Python\Python312\pythonw.exe
if exist "%PYW%" (
    start "" "%PYW%" "%~dp0tools\launcher.py"
) else (
    start "" pythonw "%~dp0tools\launcher.py"
)
