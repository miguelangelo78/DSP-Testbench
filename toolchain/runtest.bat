SET old_dir=%cd%

@cd "%~dp0"
@echo off
@cls

@cd ..

@make -f toolchain\makefile.mak --no-print-directory test_%1 all

@cd "%old_dir%"