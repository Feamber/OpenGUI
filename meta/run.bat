echo off

if not exist bin\ tar -xf bin.zip
if not exist lib\ tar -xf bin.zip

%~dp0bin\meta.exe %~dp0..\build -folder %~dp0..\source\Dummy