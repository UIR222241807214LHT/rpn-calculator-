"""
main.py
"""

import dearpygui.dearpygui as dpg
from gui_functions import setup_gui_layout

# --- 创建 GUI 上下文 ---
dpg.create_context()

# 注册字体，自选字体
with dpg.font_registry():
    with dpg.font("DENG.TTF", 18) as font1:  # 增加中文编码范围，防止问号
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Chinese_Simplified_Common)
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Chinese_Full)
    dpg.bind_font(font1)

# 调用 gui_functions 中定义的函数来设置 GUI 布局
setup_gui_layout()

# --- 启动 ---
dpg.create_viewport(title="Reverse Polish Notation GUI", width=515, height=540)
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
