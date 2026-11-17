@echo off
setlocal
set "ROOT=%~dp0.."
set "QMAKE=D:\Qt\6.11.0\mingw_64\bin\qmake.exe"
set "MAKE=D:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe"
set "DEPLOYQT=D:\Qt\6.11.0\mingw_64\bin\windeployqt.exe"
set "BUILD=%ROOT%\build-release"
set "DEPLOY=%ROOT%\deploy"

if not exist "%BUILD%" mkdir "%BUILD%"
cd /d "%BUILD%"

echo [1/3] qmake...
"%QMAKE%" "%ROOT%\StudentManagementSystem_Qt.pro" -spec win32-g++ CONFIG+=release
if errorlevel 1 exit /b 1

echo [2/3] mingw32-make...
"%MAKE%" -j8
if errorlevel 1 exit /b 1

if not exist "%DEPLOY%" mkdir "%DEPLOY%"
copy /Y "%BUILD%\release\StudentManagementSystem_Qt.exe" "%DEPLOY%\" >nul
if errorlevel 1 exit /b 1

echo [3/3] windeployqt...
"%DEPLOYQT%" --release --compiler-runtime --no-translations --dir "%DEPLOY%" "%DEPLOY%\StudentManagementSystem_Qt.exe"
if errorlevel 1 exit /b 1

echo.
echo Build finished: %DEPLOY%\StudentManagementSystem_Qt.exe
endlocal
