@echo off
setlocal enabledelayedexpansion

set exe_path=mitsuba3\mitsuba
set dir_path=staticSceneCam
set output_dir=Output

for %%f in ("%dir_path%\*cam*.*") do (
    set filename=%%~nf
    "%exe_path%" "%%f" -o "%output_dir%\!filename!"
)
endlocal