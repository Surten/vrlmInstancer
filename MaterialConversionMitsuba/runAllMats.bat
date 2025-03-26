@echo off
setlocal

REM Set the subfolder path relative to the script
set "subfolder=Output"

call .venv\Scripts\activate.bat

REM Loop until there are 53 files in the subfolder
:loop
REM Count the number of files in the subfolder
for /f %%a in ('dir /b "%subfolder%" ^| find /c /v ""') do set filecount=%%a

REM Check if the file count is less than 53
if %filecount% lss 53 (
    REM Run the Python script
    python mitsubaRunner.py
    REM Pause for a short duration before checking again (optional)
    timeout /t 5 >nul
    REM Go back to the start of the loop
    goto :loop
)

echo "There are 53 or more files in the subfolder. Exiting loop."