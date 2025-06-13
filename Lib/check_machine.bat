@echo off
for %%f in (*.lib) do (
    echo Checking %%f ...
    dumpbin /headers %%f | findstr machine
    echo.
)
pause
