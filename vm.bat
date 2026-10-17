@echo off

cd VIRTUAL_MACHINE

g++ -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal ^
-Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs ^
-Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual ^
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 ^
-Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor ^
-Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing ^
-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG ^
-D_EJUDGE_CLIENT_SIDE STACK/stack.c VM.c -o vm_prog.exe

if exist "%~dp0STACK_ERRORS.log" (
del /q "%~dp0STACK_ERRORS.log"
) else (
rem
)

.\vm_prog.exe ../%1
