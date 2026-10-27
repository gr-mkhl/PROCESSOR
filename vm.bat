@echo off

cd VIRTUAL_MACHINE

g++ -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Winline ^
-Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wall ^
-Wswitch-default -Weffc++ -Wmain -Wextra -g -pipe -fexceptions -Wcast-qual ^
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 ^
-Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor ^
-Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing ^
-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG ^
-D_EJUDGE_CLIENT_SIDE ../STACK/stack.c ../STACK/verify.c ../STACK/stack_print.c ../log.c VM.c ^
-I ../../raylib/include -L ../../raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -o vm_prog.exe


.\vm_prog.exe ../%1
