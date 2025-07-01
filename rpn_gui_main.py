import os
import sys
import dearpygui.dearpygui as dpg
from gui_functions import setup_gui_layout

# --- 字体路径 ---
if getattr(sys, "frozen", False):
    base_path = sys._MEIPASS
else:
    base_path = os.path.dirname(os.path.abspath(__file__))
font_path = os.path.join(base_path, "DENG.TTF")
print("字体文件路径：", font_path)

# --- 创建 GUI 上下文 ---
dpg.create_context()

# --- 注册字体 ---
with dpg.font_registry():
    with dpg.font(font_path, 18, default_font=True) as font1:
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Chinese_Full)
    dpg.bind_font(font1)

# --- 设置 GUI 布局 ---
setup_gui_layout()

# --- 启动窗口 ---
dpg.create_viewport(title="Reverse Polish Notation GUI", width=515, height=540)
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
