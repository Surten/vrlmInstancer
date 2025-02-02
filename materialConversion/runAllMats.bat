rem Set the directory path containing the files
set "directory=output"

rem Iterate over each file in the directory
for %%f in ("%directory%\*") do (
    rem Run the executable with the current file as an argument
    "pbrt" "%%f"
)

echo All tasks completed.
pause