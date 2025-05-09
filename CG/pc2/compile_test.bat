@echo off
setlocal EnableDelayedExpansion

REM Verifica si se proporcionó un parámetro
if "%~1"=="" (
    echo Error: Debes especificar la carpeta del ejercicio.
    echo Uso: compile_test.bat nombre_carpeta
    echo Ejemplo: compile_test.bat exercise01
    exit /b 1
)

set ejercicio=%~1
echo Compilando test.cpp en %ejercicio%...

REM Verifica si la carpeta existe
if not exist %ejercicio% (
    echo Error: La carpeta %ejercicio% no existe.
    exit /b 1
)

REM Entra en la carpeta del ejercicio
cd %ejercicio%

REM Verifica si existen los archivos necesarios
if not exist test.cpp (
    echo Error: No se encontró el archivo test.cpp en %ejercicio%
    cd ..
    exit /b 1
)

REM Compila amobs archivos juntos
C:\mingw64\bin\g++.exe test.cpp -o test.exe -I C:\mingw64\include -L C:\mingw64\lib -lgtest -lgtest_main

if %ERRORLEVEL% NEQ 0 (
    echo Error en la compilacion!
    cd ..
    exit /b %ERRORLEVEL%
)

echo.
echo Ejecucion de tests en %ejercicio%:
echo ------------------------------------

if exist test.exe (
    .\test.exe --gtest_color=yes
    
    REM Guarda el código de salida antes de eliminarlo
    set test_result=%ERRORLEVEL%
    
    REM Elimina el archivo test.exe
    del test.exe
) else (
    echo ERROR: No se encontro el archivo test.exe
    set test_result=1
)

echo ------------------------------------
cd ..

REM Devuelve el código de resultado del test
exit /b %test_result%