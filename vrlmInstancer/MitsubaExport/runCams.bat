@echo off
setlocal enabledelayedexpansion

set exe_path=mitsuba
set dir_path=staticSceneCam

for %%f in ("%dir_path%\*cam*.*") do (
    "%exe_path%" "%%f"
)

endlocal