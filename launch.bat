@echo off
REM Créer le dossier "build" s'il n'existe pas
if not exist build (
    mkdir build
)

REM Aller dans le dossier "build"
cd build

REM Générer les fichiers avec CMake pour MinGW
cmake .. -G "MinGW Makefiles"

REM Compiler le projet
cmake --build .

REM Exécuter le programme
.\GLFW_Interface.exe
