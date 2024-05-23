@echo off
@echo Build tools
pushd tools
@echo kt-image.exe: image converter to Katsu format.
cl -O2 -Tc ktimg\*c -Fo.\ktimg\ -link -SUBSYSTEM:CONSOLE -OUT:kt-image.exe
@echo "kt-bin2inc.exe: Converts binary file to include as c file."
cl -O2 -Tc bin2inc\*c -Fo.\bin2inc\ -link -SUBSYSTEM:CONSOLE -OUT:kt-bin2inc.exe
popd
@echo Convert textfiles to include
for /D %%s in (.\src\opengl\shaders\*.glsl) do .\tools\kt-bin2inc.exe -nc %%s


@echo Compile all source files
if not exist build (mkdir build)
pushd build
cl -O2 -c ..\src\*c ..\src\opengl\*c ..\src\platform\windows\*c -EHsc -I"..\include" -Fo.\
popd
@echo Generate lib\katsu.lib
if not exist lib (mkdir lib)
lib build\*.obj -out:lib\katsu.lib


cl -O2 -Wall -Tc main.c -I"..\include" -link User32.lib Opengl32.lib katsu.lib -SUBSYSTEM:CONSOLE -OUT:main.exe