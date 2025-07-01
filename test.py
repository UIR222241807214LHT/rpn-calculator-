"""
main.py
"""
import os
import sys
import dearpygui.dearpygui as dpg
from gui_functions import setup_gui_layout
if getattr(sys, "frozen", False):
    base_path = sys._MEIPASS
else:
    base_path = os.path.dirname(os.path.abspath(__file__))

font_path = os.path.join(base_path, "DENG.TTF")
print("字体文件路径：", font_path)