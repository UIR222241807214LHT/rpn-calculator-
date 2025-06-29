"""
functions.py
"""

import math
from ctypes import cdll, c_char_p, c_double
import dearpygui.dearpygui as dpg

# --- 加载 DLL (保留在函数文件，因为它被 evaluate_rpn_expr 使用) ---
rpn_lib = cdll.LoadLibrary("./rpn_lib.dll")
rpn_lib.evaluate_rpn_c.restype = c_double
rpn_lib.get_last_error.restype = c_char_p


# --- C++ 封装函数 ---
def evaluate_rpn_expr(tokens):
    """
    接收一个逆波兰表达式的 token 列表，调用 C++ DLL 进行计算。
    参数:
        tokens: list[str]: 是一个字符串列表，例如 ["3", "4", "+"]
    返回:
        float: 计算结果（或 NaN）
    """
    token_array = c_char_p * len(tokens)
    arr = token_array(*(t.encode("utf-8") for t in tokens))
    result = rpn_lib.evaluate_rpn_c(arr, len(tokens))
    if math.isnan(result):
        # 如果是 NaN，说明 C++ 侧出错，取出错误信息
        err = rpn_lib.get_last_error().decode("utf-8", "ignore")
        raise RuntimeError(err)
    return result


# --- 插入到输入框 ---
def insert_to_input(_sender, _app_data, _user_data):
    content = _user_data

    current = dpg.get_value("input")

    if content == "C": # 清空按钮
        dpg.set_value("input", "")
        dpg.set_value("output", "") # 顺便清空输出
        return
    elif content == "<-": # 退格按钮
        dpg.set_value("input", current[:-1]) # 移除最后一个字符
        return

    # 定义所有不需要在后面自动加空格的字符（主要指运算符，通常在表达式尾部或紧跟操作数）
    # 这里考虑了单目运算符和二元运算符的显示习惯
    # 对于 sin, cos, tan, log 这些函数名，它们本身就是 token，后面应该跟空格，以便输入其操作数
    # 对于 pi, e, $ 这些常量，它们也是 token，后面通常也应该跟空格
    # 所以，只有真正的二元运算符 (+-*/) 和空格本身不需要额外加空格

    operators = {"+", "-", "*", "/"} # 这些是二元运算符

    if content == " ":
        # 如果是空格键，就加个空格
        content = " "
    elif content in operators:
        # 如果是二元运算符，不加额外空格，直接追加
        pass # 已经是一个完整的token，不需要再加空格了
    else:
        # 其他所有字符（数字、小数点、函数名如 sin/cos、常量如 pi/e/$）后面都加空格
        content += ""

    dpg.set_value("input", current + content)


# --- 按钮回调 ---
def on_calculate(_sender, _app_data, _user_data):
    """
    当用户点击“计算”按钮时触发的回调函数。

    功能：
    - 读取输入框中的逆波兰表达式
    - 解析为 token 列表
    - 调用 C++ DLL 执行计算
    - 将结果或错误信息显示在输出区域

    参数:
        sender (int): 触发该回调的控件 ID（由 DearPyGui 自动传入）
        app_data (Any): 控件传入的数据（未使用）
        user_data (Any): 用户自定义数据（未使用）
    """
    input_expr = dpg.get_value("input")
    tokens = input_expr.strip().split()
    if not tokens:
        dpg.set_value("output", "请输入逆波兰表达式！")
        return

    try:
        result = evaluate_rpn_expr(tokens)
        dpg.set_value("output", f"计算结果：{result}")
    except RuntimeError as e:
        # 这里 e.args[0] 是 C++ 端的错误消息（中文）
        dpg.set_value("output", f"错误：{e}")
    except Exception as e:
        dpg.set_value("output", f"未知错误：{e}")



def setup_gui_layout():
    """
    定义一个初始化 GUI 的函数
    这样 main 文件中只需要调用一个函数
    """
    # --- 主题 ---
    with dpg.theme() as button_theme:
        with dpg.theme_component(dpg.mvButton):
            dpg.add_theme_color(
                dpg.mvThemeCol_Button, (100, 150, 255), category=dpg.mvThemeCat_Core
            )   # 背景色
            dpg.add_theme_color(
                dpg.mvThemeCol_ButtonHovered,
                (120, 180, 255),
                category=dpg.mvThemeCat_Core,
            )   # 悬停色
            dpg.add_theme_color(
                dpg.mvThemeCol_ButtonActive,
                (80, 120, 220),
                category=dpg.mvThemeCat_Core,
            )   # 点击色
            dpg.add_theme_color(
                dpg.mvThemeCol_Text, (0, 0, 0), category=dpg.mvThemeCat_Core
            )   # 文字色

    # --- 创建窗口 ---
    with dpg.window(label="逆波兰计算器", width=500, height=500):

        dpg.add_text("请输入逆波兰表达式（空格分隔）")
        dpg.add_input_text(tag="input", multiline=True,  width=485,height=80)

        # --- 更新 5x5 小键盘网格 ---
        button_layout = [
            ["7", "8", "9", "+", "sin"],
            ["4", "5", "6", "-", "cos"],
            ["1", "2", "3", "*", "tan"],
            ["0", ".", " ", "/", "log"],
            ["pi", "e", "$", "C", "<-"],
        ]

        with dpg.table(
            header_row=False,
            policy=dpg.mvTable_SizingFixedFit,
            borders_innerH=True,
            borders_innerV=True,
        ):
            # 添加表格的列定义
            dpg.add_table_column(label="Column 1")
            dpg.add_table_column(label="Column 2")
            dpg.add_table_column(label="Column 3")
            dpg.add_table_column(label="Column 4")
            dpg.add_table_column(label="Column 5")

            # 按钮创建时，将 label 作为 user_data 传递
            for row in button_layout:
                with dpg.table_row():
                    for label in row:
                        btn_tag = f"btn_{label}"
                        dpg.add_button(
                            label=label,
                            width=90,
                            height=40,
                            callback=insert_to_input,
                            user_data=label,
                            tag=btn_tag,
                        )
                        dpg.bind_item_theme(btn_tag, button_theme)

        dpg.add_button(label="计算", callback=on_calculate)
        dpg.add_spacer(height=10)
        dpg.add_text("", tag="output")