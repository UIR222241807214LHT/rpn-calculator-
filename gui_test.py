import dearpygui.dearpygui as dpg

dpg.create_context()

# 注册字体，自选字体
with dpg.font_registry():
    with dpg.font("DENG.TTF", 18) as font1:  # 增加中文编码范围，防止问号
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Chinese_Simplified_Common)
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Chinese_Full)
    dpg.bind_font(font1)

with dpg.window(label="Demo 窗口", width=500, height=150):
    dpg.add_text("Hello, world")
    dpg.add_text("你好，世界")
    # dpg.bind_font(font1)  # 不绑定也没关系，default_font=True 已生效

dpg.create_viewport(title="dear_pygui", width=800, height=600)
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
