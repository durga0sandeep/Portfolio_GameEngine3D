@echo off
echo ==========================================
echo Cleaning old documentation...
echo ==========================================

:: Path to output folder (adjust if needed)
set OUTPUT_DIR=docs

if exist %OUTPUT_DIR% (
    rmdir /s /q %OUTPUT_DIR%
)

echo ==========================================
echo Running Doxygen to generate documentation
echo ==========================================

:: Run doxygen with your config file
doxygen Doxyfile

echo.
echo Documentation generated successfully!

:: Auto-open docs in browser (optional)
if exist %OUTPUT_DIR%\html\index.html (
    start %OUTPUT_DIR%\html\index.html
)

pause
