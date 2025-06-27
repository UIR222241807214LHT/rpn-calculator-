"""
测试
"""

from ctypes import cdll, c_char_p, c_double
import dearpygui.dearpygui as dpg


# --- 加载 DLL ---
rpn_lib = cdll.LoadLibrary("./rpn_lib.dll")
rpn_lib.evaluate_rpn_c.restype = c_double

# --- 创建 GUI 上下文 ---
dpg.create_context()

# 注册字体，自选字体
with dpg.font_registry():
    with dpg.font("DENG.TTF", 18) as font1:  # 增加中文编码范围，防止问号
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Chinese_Simplified_Common)
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Chinese_Full)
    dpg.bind_font(font1)


# --- C++ 封装函数 ---
def evaluate_rpn_expr(token_list):
    token_array = c_char_p * len(token_list)
    c_array = token_array(*(s.encode("utf-8") for s in token_list))
    return rpn_lib.evaluate_rpn_c(c_array, len(token_list))


# --- 按钮回调 ---
def on_calculate(sender, app_data, user_data):
    input_expr = dpg.get_value("input")
    tokens = input_expr.strip().split()
    if not tokens:
        dpg.set_value("output", "请输入逆波兰表达式！")
        return

    try:
        result = evaluate_rpn_expr(tokens)
        if result != result:
            dpg.set_value("output", "表达式有误或计算失败")
        else:
            dpg.set_value("output", f"计算结果：{result}")
    except Exception as e:
        dpg.set_value("output", f"错误：{str(e)}")


# --- 创建窗口 ---
with dpg.window(label="逆波兰计算器", width=500, height=300):
    dpg.add_text("请输入逆波兰表达式（空格分隔）")
    dpg.add_input_text(tag="input", multiline=True, height=80)
    dpg.add_button(label="计算", callback=on_calculate)
    dpg.add_spacer(height=10)
    dpg.add_text("", tag="output")

# --- 启动 ---
dpg.create_viewport(title="Reverse Polish Notation GUI", width=600, height=400)
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
