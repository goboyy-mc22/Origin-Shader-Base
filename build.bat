@echo off
rem Origin Shader build tool launcher (Windows)
cd /d "%~dp0"
where /q python || (
    echo [ERROR] Python 3.11+ not found. Install it and add it to PATH.
    exit /b 1
)
python tools %*
exit /b %errorlevel%
