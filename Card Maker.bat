@echo off
rem Opens the Card Maker (tools\card_maker.py) for the DLC's new cards.
set PYW=%LOCALAPPDATA%\Programs\Python\Python312\pythonw.exe
if exist "%PYW%" (
    start "" "%PYW%" "%~dp0tools\card_maker.py"
) else (
    start "" pythonw "%~dp0tools\card_maker.py"
)
