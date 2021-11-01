@echo off
cl main.cc /Fe:a /Ox /MP /nologo /link /incremental:no /entry:main /nodefaultlib ^
 ucrt.lib opengl32.lib gdi32.lib kernel32.lib user32.lib libcmt.lib
