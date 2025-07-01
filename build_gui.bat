@echo off
REM 使用 PyInstaller 打包，不显示控制台，单文件，添加 DLL 和字体文件
pyinstaller --noconsole --onefile --add-data "rpn_lib.dll;." --add-data "DENG.TTF;." rpn_gui_main.py
pause
