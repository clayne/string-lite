@echo off & setlocal enableextensions enabledelayedexpansion
::
:: tcl.bat - compile & run tests (clang-cl).
::

set      unit=string
set unit_file=string

:: if no std is given, use c++17

set std=%1
if "%std%"=="" set std=c++17

set  clang=clang-cl

call :CompilerVersion version
echo %clang% %version%: %std%

set unit_config=^
    -Dstring_STRING_HEADER=\"nonstd/string.hpp\" ^
    -Dstring_TEST_NODISCARD=0 ^
    -Dstring_CONFIG_SELECT_STRING_VIEW=string_CONFIG_SELECT_STRING_VIEW_INTERNAL

::string_CONFIG_SELECT_STRING_VIEW_INTERNAL
::string_CONFIG_SELECT_STRING_VIEW_NONSTD
::string_CONFIG_SELECT_STRING_VIEW_STD

rem -flto / -fwhole-program
set  optflags=-O2
set warnflags=-Wall -Wextra -Wpedantic -Weverything -Wshadow -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-missing-noreturn -Wno-documentation-unknown-command -Wno-documentation-deprecated-sync -Wno-documentation -Wno-weak-vtables -Wno-missing-prototypes -Wno-missing-variable-declarations -Wno-exit-time-destructors -Wno-global-constructors -Wno-sign-conversion -Wno-sign-compare -Wno-implicit-int-conversion -Wno-deprecated-declarations

"%clang%" -m32 -EHsc -std:%std% %optflags% %warnflags% %unit_config% -fms-compatibility-version=19.00 -I../../string-view-lite/include -I../include -Ics_string -I. -o %unit_file%-main.t.exe %unit_file%-main.t.cpp %unit_file%.t.cpp && %unit_file%-main.t.exe
endlocal & goto :EOF

:: subroutines:

:CompilerVersion  version
echo off & setlocal enableextensions
set tmpprogram=_getcompilerversion.tmp
set tmpsource=%tmpprogram%.c

echo #include ^<stdio.h^>     > %tmpsource%
echo int main(){printf("%%d.%%d.%%d\n",__clang_major__,__clang_minor__,__clang_patchlevel__);} >> %tmpsource%

"%clang%" -m32 -o %tmpprogram% %tmpsource% >nul
for /f %%x in ('%tmpprogram%') do set version=%%x
del %tmpprogram%.* >nul
endlocal & set %1=%version%& goto :EOF

:: toupper; makes use of the fact that string
:: replacement (via SET) is not case sensitive
:toupper
for %%L IN (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) DO SET %1=!%1:%%L=%%L!
goto :EOF
